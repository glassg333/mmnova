#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
LegacyEnvFixer.py — пак 11: линтер старых проектов Monomachine на типовые
поломки конвертов (см. JUCE_ADAPT_AND_FIX_RU.md, «8 типов поломок»).

Использование:
  python3 LegacyEnvFixer.py <путь к дереву проекта> [--md-terms]

  --md-terms : дополнительно предлагать замену термина «резонатор» → «резонанс»
               в .md/.txt (только отчёт; автоправку делает автор)

Возвращает код 0, если ничего критичного не найдено, 1 — есть находки.
Паттерны собраны из реальной истории исправлений реверса OS 1.32B
(канонический док FILTER_ENVELOPES_2026-09-30.md, раздел «История исправлений»).
"""
import os
import re
import sys

FINDINGS = []

RULES = [
    # (id, критичность, regex, объяснение, как чинить)
    ("B1-резонатор-термин", "info",
     r"\bрезонатор\w*", "термин «резонатор»",
     "Пиши «резонанс» / «блок резонанса» / «резонансный хвост». В OS нет "
     "отдельного «резонатора» — есть фильтр (срез) и его резонанс (стадия 2)."),

    ("B2-стадия2-кормит-делэй", "critical",
     r"(стади\w+|stage\s*2|L2|резонан\w+)\W{0,30}(пита\w+|корм\w+|feed\w+|пиш\w+|запис\w+|выход\w*\s*в)\W{0,30}(тап\w+|делэ\w+|delay)|"
     r"(тап\w+|delay\s*taps?)\W{0,30}(пита\w+|корм\w+|feed\w+)\W{0,30}(стади\w+|stage\s*2|резонан\w+)",
     "заявлена связь «стадия 2 → тапы делэя»",
     "Связи НЕТ (эксп. exp68: свип ATK/DEC/BOFS/WOFS не меняет тапы/рампы/эхо). "
     "Удали запись выходов стадии 2 в банк тапов делэя; делэй модулируют "
     "DBAS/DWID + env2 ($0B1E)."),

    ("B3-BOFS-как-CC86-87", "critical",
     r"(BOFS|WOFS)\W{0,30}(CC8[67]|DBAS|DWID)|((CC8[67]|DBAS|DWID))\W{0,30}(BOFS|WOFS)",
     "смешаны ячейки фильтра ($416/$417) и делэя ($41E/$41F)",
     "BOFS/WOFS = стартовые углы резонанса фильтра (CC78/79); DBAS/DWID = "
     "база/глубина модуляции тапов делэя (CC86/87). Развести трассировку."),

    ("B4-env2-как-фильтр", "critical",
     r"(env\s*2|ENV\s*2|\$418|\$41[89AB])\W{0,60}(фильтр|filter|BASE|WDTH|срез)|"
     r"(втор\w+\s+фильтр|second\s+filter)",
     "env2 подключён к фильтру / заявлен «второй фильтр»",
     "env2 = конверт глубины модуляции тапов делэя (P:$04A8–$04F4, выход Y:$4FF, "
     "единственный потребитель $0B1E). Второго фильтра в OS НЕТ; срез env-free "
     "(доказано бит-в-бит). Отвяжи env2 от фильтра, дефолт уровня = 1.0."),

    ("B5-нет-KILL-гейта", "warn",
     r"(стади\w*\s*2|резонан\w+|stage\s*2|resonan\w*)",
     "упомянут резонанс/стадия 2 — проверь наличие KILL-гейта",
     "При слове фазы AMP-конверта == 4 множитель затухания резонанса = 0 "
     "($0AC0–$0AC4). Если в порте этой проверки нет — хвост висит после "
     "снятия ноты. Вставь mnmfix::stage2DecayMultiplier (juce/MnmEnvLegacyFix.h)."),

    ("B6-атака-без-лимитера", "warn",
     r"(level|env\w*)\s*\+=\s*\w*(inc|attack|atk)\w*",
     "атака «level += inc» — проверь data-limiter",
     "При переполнении 24 бит прошивка сатурирует $7FFFFF и уходит в HOLD "
     "(фаза 1, счётчик := 1), НЕ заворачивает в отрицательные. См. "
     "mnmfix::ampAttackStep."),

    ("B7-REL-таблица-DEC", "warn",
     r"(release|REL)\W{0,40}(DEC|decay\s*table|таблиц\w*\s*(DEC|спада))",
     "REL использует таблицу DEC",
     "У AMP-конверта свои слова: DEC (фаза 2), REL (фаза 3), KILL — фикс "
     "слово $20 → idx 1 → ×0.904016/кадр. DEC=127 → ×1.0 (заморозка). "
     "См. mnmfix::decayIdxFromRateWord."),

    ("B8-DBAS-DWID-конверт", "critical",
     r"(DBAS|DWID)\W{0,50}(ADSR|attack|энвелоп|envelope|конверт)",
     "на DBAS/DWID повесили отдельный конверт",
     "У делэя нет своего ADSR: модуляция тапов = (env2·DWID)² c анти-зиппер "
     "рампой T5 ($0B1E–$0B32). Убери ADSR, вставь mnmfix::dwidRamp16 "
     "(точная арифметика — dsp/mnm/MnmTail.hpp, 8296/0)."),

    ("B9-DSP-пинг-понг", "warn",
     r"(ping.?pong|пинг.?понг)\W{0,50}(dsp|kernel|кадр)",
     "пинг-понг эха реализован в DSP",
     "Посыл в эхо — МОНО (T6: echo = (L+R)>>1); сторону двигает знак DSND "
     "в прологе кадра ($0284/$029C–$02A8) = мануал стр. 33. Пинг-понг — "
     "хост-уровень, не ядро."),
]


def scan_file(path, text, md_terms, is_code):
    # строки-исключения: описания самих поломок/фиксов, отрицания, таблицы фиксов
    suppress = re.compile(
        r"ENVLINT-OK|связи нет|нет связи|не подключ|НЕ подключ|НЕВЕРНО|НЕ «|"
        r"не «|удали|удалить|remove|ломк\w+|пролом|замена|фикс|FIX|\|\s*B\d|"
        r"второго фильтра|не он|это не",
        re.IGNORECASE)
    code_only = {"B5-нет-KILL-гейта", "B6-атака-без-лимитера",
                 "B7-REL-таблица-DEC", "B9-DSP-пинг-понг"}
    seen = set()
    lines = text.splitlines()
    for rid, sev, pat, what, howto in RULES:
        if rid == "B1-резонатор-термин" and not md_terms:
            continue
        if rid in code_only and not is_code:
            continue
        for m in re.finditer(pat, text, re.IGNORECASE | re.DOTALL):
            line_no = text.count("\n", 0, m.start()) + 1
            line = lines[line_no - 1] if line_no - 1 < len(lines) else ""
            if suppress.search(line):
                continue
            key = (path, line_no, rid)
            if key in seen:
                continue
            seen.add(key)
            snippet = re.sub(r"\s+", " ", m.group(0))[:90]
            FINDINGS.append((sev, path, line_no, rid, what, snippet, howto))


def main():
    args = [a for a in sys.argv[1:] if not a.startswith("--")]
    md_terms = "--md-terms" in sys.argv
    if not args:
        print(__doc__)
        return 2
    root = args[0]
    exts = (".h", ".hpp", ".cpp", ".cc", ".c", ".md", ".txt", ".json")
    n = 0
    for dirpath, _dirs, files in os.walk(root):
        for fn in files:
            if not fn.lower().endswith(exts):
                continue
            p = os.path.join(dirpath, fn)
            try:
                text = open(p, encoding="utf-8", errors="ignore").read()
            except OSError:
                continue
            is_code = fn.lower().endswith((".h", ".hpp", ".cpp", ".cc", ".c"))
            scan_file(p, text, md_terms, is_code)
            n += 1
    crit = [f for f in FINDINGS if f[0] == "critical"]
    warn = [f for f in FINDINGS if f[0] == "warn"]
    info = [f for f in FINDINGS if f[0] == "info"]
    print("LegacyEnvFixer: просканировано файлов: %d" % n)
    print("  critical: %d | warn: %d | info: %d" % (len(crit), len(warn), len(info)))
    for sev, p, line, rid, what, snip, howto in FINDINGS:
        print("\n[%s] %s:%d  (%s)\n  находка: %s\n  фрагмент: %r\n  фикс: %s"
              % (sev.upper(), p, line, rid, what, snip, howto))
    return 1 if crit else 0


if __name__ == "__main__":
    sys.exit(main())
