#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
apply_patch.py — установщик ПАКА 5 «фикс ручек FRQ, TUNE и режимов FM» для
Monomachine Nova (JUCE). Дата: 2026-09-28.

ЧТО ДЕЛАЕТ:
  1. Копирует 6 файлов в <Source>/dsp/mnm/  (звук FM-машин):
       MnmFm.hpp (v2), MnmFmStat.hpp (НОВЫЙ), MnmFmDsp.hpp, MnmFmPar.hpp,
       MnmFmDyn.hpp, MnmFmSineTable.h
  2. Патчит ЭКРАН (PluginEditor.cpp): цифры под 1FRQ/2FRQ/3FRQ — по каноническим
     таблицам (STAT/PAR: дроби 1/64..4; DYN: K/64 и (K/64)^2).
  3. Переименовывает режимы SYNT-секции (DspModes.hpp): mnm -> "mnm fix",
     old -> "new fix" (индексы не меняются, сохранённые состояния совместимы).
  4. Патчит СТАРЫЕ движки FM PAR / FM DYN (dsp/monomachine_fm_par.hpp,
     dsp/monomachine_fm_dynamic.hpp): биндинги FRQ по прошивке + TUNE ±2
     полутона (было ±12).

ПРАВИЛА:
  * Шаги 2-4 — опциональные: если шаблон не найден, шаг честно пропускается
    с пометкой (твоя версия дерева может отличаться), установка НЕ падает.
  * Повторный запуск безопасен: показывает "already identical"/"already applied".
  * Бэкапы: рядом с файлом создаётся <имя>.bak_fmfix5 (один раз).

ЗАПУСК:
  python3 apply_patch.py <путь к Source>
  (примет: .../Monomachine_Nova_Synth/Source  или  .../Monomachine_Nova_Synth
   или корень репозитория с папкой JUCE внутри)
"""
import os, re, shutil, sys, hashlib

HERE = os.path.dirname(os.path.abspath(__file__))

MNM_FILES = ["MnmFm.hpp", "MnmFmStat.hpp", "MnmFmDsp.hpp", "MnmFmPar.hpp",
             "MnmFmDyn.hpp", "MnmFmSineTable.h"]

DISPLAY_HELPER = r"""
// === FM-FIX 5 (2026-09-28): канонические законы дисплея FRQ (OS 1.32) ===
// machine 8 = FM STAT, 9 = FM PAR: таблица Y:$141A80/2^20, 24 ступени 1/64..4,
// индекс n = ((K<<16)+$8000)*48>>24. machine 10 = FM DYN: 1FRQ = K/64
// (K=127 -> 2.0), 2FRQ = (K/64)^2 (K=127 -> 4.0).
#include <cstdio>
namespace monomachine {
inline const char* kFmFrqText(uint8_t machine, const char* label, int raw)
{
    static const char* const kRat[24] = {
        "1/64","1/32","1/16","3/32","1/8","5/32","3/16","1/4","5/16","3/8",
        "7/16","1/2","5/8","3/4","7/8","1","5/4","3/2","7/4","2",
        "5/2","3","7/2","4" };
    const int K = raw < 0 ? 0 : (raw > 127 ? 127 : raw);
    const bool frq = (label!=nullptr) && (label=="1FRQ"||label=="2FRQ"||label=="3FRQ");
    if (!frq) return nullptr;
    if (machine==8 || machine==9) {
        const int n = (((K<<16)+0x8000)*48)>>24;
        return kRat[n < 0 ? 0 : (n > 23 ? 23 : n)];
    }
    if (machine==10) {
        static char buf[16];
        const double w = (K<<16) > 0x7EFFFF ? (double)0x7FFFFF : (double)(K<<16);
        if (label[0]=='1') { snprintf(buf,sizeof(buf),"%.3f", w*(1.0/4194304.0)); return buf; }
        if (label[0]=='2') { const double x=w*(1.0/8388608.0); snprintf(buf,sizeof(buf),"%.3f", 4.0*x*x); return buf; }
    }
    return nullptr;
}
} // namespace monomachine
"""

DISPLAY_REPLACE = (
'if(machine>=8&&machine<=10&&(label=="1FRQ"||label=="2FRQ"||label=="3FRQ"))'
'{ // FM-FIX 5: таблица и законы прошивки (см. kFmFrqText)\n'
'            const char* fmT = monomachine::kFmFrqText(machine, label.toRawUTF8(), raw);\n'
'            if(fmT)return juce::String(fmT);\n'
'        }'
)

SYNT_CHOICES_OLD = re.compile(r'case DspSynt:\s*return "mnm\|old\|fma";')
SYNT_CHOICES_NEW = 'case DspSynt:   return "mnm fix|new fix|fma";  // FM-FIX 5: честные имена, индексы не менялись'

SYNT_TIP_OLD = re.compile(r'case DspSynt:\s*return "SYNT: mnm = FM\+ core[^"]*"')
SYNT_TIP_NEW = ('case DspSynt:   return "SYNT: \'mnm fix\' = бит-точные ядра FM+STAT/PAR/DYN из '
                'прошивки OS 1.32 (ручки 1/64..4 и DYN K/64, (K/64)^2, TUNE +-2 полутона, питч-слово '
                'A=2*Гц). \'new fix\' = прежние Nova-движки с теми же правильными биндингами ручек. '
                'У FM+STAT оба режима ведут к одному ядру (резерв был удалён в 1.6.5)";  // FM-FIX 5')

def sha256(p):
    h = hashlib.sha256()
    with open(p, "rb") as f:
        for chunk in iter(lambda: f.read(65536), b""):
            h.update(chunk)
    return h.hexdigest()

def find_source(arg):
    p = os.path.abspath(arg)
    if os.path.isdir(os.path.join(p, "dsp", "mnm")):
        return p
    if os.path.isdir(os.path.join(p, "Source", "dsp", "mnm")):
        return os.path.join(p, "Source")
    cand = os.path.join(p, "Source")
    if os.path.isdir(cand) and os.path.isdir(os.path.join(cand, "dsp", "mnm")):
        return cand
    # корень репозитория: JUCE/*/Monomachine_Nova_Synth/Source
    juce = os.path.join(p, "JUCE")
    if os.path.isdir(juce):
        for d in sorted(os.listdir(juce)):
            c = os.path.join(juce, d, "Monomachine_Nova_Synth", "Source")
            if os.path.isdir(os.path.join(c, "dsp", "mnm")):
                return c
    raise SystemExit("Не нашёл Source/dsp/mnm в: " + arg + "\n"
                     "Передай путь к .../Monomachine_Nova_Synth/Source")

def copy_mnm(src):
    print("== Шаг 1: звук FM-машин (dsp/mnm) ==")
    n = 0
    for f in MNM_FILES:
        srcf = os.path.join(HERE, "dsp", "mnm", f)
        dstf = os.path.join(src, "dsp", "mnm", f)
        if not os.path.exists(srcf):
            print(f"   !! в паке нет {f} — пропущено"); continue
        if os.path.exists(dstf) and sha256(srcf) == sha256(dstf):
            print(f"   {f}: already identical"); continue
        if not os.path.exists(dstf):
            print(f"   {f}: НОВЫЙ файл установлен"); n += 1
        else:
            bak = dstf + ".bak_fmfix5"
            if not os.path.exists(bak):
                shutil.copy2(dstf, bak)
            print(f"   {f}: обновлён (бэкап .bak_fmfix5)"); n += 1
        shutil.copy2(srcf, dstf)
    if n == 0:
        print("   (всё уже стояло)")

def text_patch(path, transforms, label):
    """transforms: list of (compiled_regex_or_substring, replacement, report_done, report_skip)."""
    if not os.path.exists(path):
        print(f"   !! {label}: файла нет ({path}) — пропущено")
        return
    text = open(path, encoding="utf-8").read()
    orig = text
    log = []
    for pat, rep, ok_msg, skip_msg in transforms:
        if callable(pat):
            text, msg = pat(text)
            log.append(msg)
        elif pat in text:
            if rep is None:
                log.append("   " + ok_msg + " (уже применено ранее)")
            else:
                text = text.replace(pat, rep, 1)
                log.append("   " + ok_msg)
        else:
            log.append("   " + skip_msg)
    if text != orig:
        bak = path + ".bak_fmfix5"
        if not os.path.exists(bak):
            shutil.copy2(path, bak)
        open(path, "w", encoding="utf-8").write(text)
        print(f"   {label}: ИЗМЕНЁН (бэкап .bak_fmfix5)")
    else:
        print(f"   {label}: без изменений")
    for l in log:
        print(l)

def patch_editor(src):
    print("== Шаг 2: экран (PluginEditor.cpp) ==")
    path = os.path.join(src, "PluginEditor.cpp")
    if not os.path.exists(path):
        print("   !! PluginEditor.cpp не найден — пропущено"); return
    text = open(path, encoding="utf-8").read()
    if "kFmFrqText" in text:
        print("   патч экрана уже стоит"); return
    pat = re.compile(r'if\s*\(\s*\(\s*machine\s*==\s*8\s*\|\|\s*machine\s*==\s*9\s*\)\s*&&[^;]*?getFmListedRatio[^;]*?;\s*')
    m = pat.search(text)
    if not m:
        print("   !! не нашли блок getFmListedRatio (machine==8||machine==9) — патч экрана ПРОПУЩЕН")
        print("      -> пришли себе этот файл, патч наложим вручную. Звук это не затрагивает.")
        return
    # вставка хелпера после последнего #include
    inc = list(re.finditer(r'^#include[^\n]*\n', text, re.M))[-1]
    text = text[:inc.end()] + DISPLAY_HELPER + text[inc.end():]
    # замена блока
    text = text[:m.start()] + DISPLAY_REPLACE + text[m.end():]
    bak = path + ".bak_fmfix5"
    if not os.path.exists(bak):
        shutil.copy2(path, bak)
    open(path, "w", encoding="utf-8").write(text)
    print("   PluginEditor.cpp: ИЗМЕНЁН (хелпер kFmFrqText + блок дисплея)")

def patch_modes(src):
    print("== Шаг 3: имена режимов SYNT (DspModes.hpp) ==")
    path = os.path.join(src, "models", "DspModes.hpp")
    if not os.path.exists(path):
        path = os.path.join(src, "DspModes.hpp")
        if not os.path.exists(path):
            print("   !! DspModes.hpp не найден — пропущено"); return
    text = open(path, encoding="utf-8").read()
    changed = False
    if '"mnm fix|new fix|fma"' in text:
        print("   имена уже переименованы")
    elif SYNT_CHOICES_OLD.search(text):
        text = SYNT_CHOICES_OLD.sub(SYNT_CHOICES_NEW, text, count=1)
        changed = True
        print("   список режимов SYNT: mnm fix|new fix|fma — OK")
    else:
        print("   !! не нашли 'mnm|old|fma' для DspSynt — имена не менялись")
    if SYNT_TIP_OLD.search(text):
        text = SYNT_TIP_OLD.sub(SYNT_TIP_NEW, text, count=1)
        changed = True
        print("   тултип SYNT обновлён — OK")
    if changed:
        bak = path + ".bak_fmfix5"
        if not os.path.exists(bak):
            shutil.copy2(path, bak)
        open(path, "w", encoding="utf-8").write(text)

def patch_old_engines(src):
    print("== Шаг 4: старые движки (monomachine_fm_par/dynamic.hpp) ==")
    for fname, label in (("monomachine_fm_par.hpp", "PAR old"),
                         ("monomachine_fm_dynamic.hpp", "DYN old")):
        path = os.path.join(src, "dsp", fname)
        if not os.path.exists(path):
            print(f"   !! {fname} не найден — пропущено"); continue
        text = open(path, encoding="utf-8").read()
        orig = text
        log = []
        # TUNE (обе машины, одинаковый шаблон)
        pat_tune = re.compile(r"m_tuneSemitones = \(static_cast<float>\(tune\) - 64\.0f\) / 64\.0f \* 12\.0f;")
        if "FM-FIX: закон прошивки" in text:
            log.append("   TUNE: уже исправлено")
        elif pat_tune.search(text):
            text = pat_tune.sub("(static_cast<float>(tune) - 64.0f) * (683.0f / 128.0f) * (12.0f / 2048.0f);"
                                " // FM-FIX: закон прошивки, +-2 полутона", text)
            log.append("   TUNE: закон прошивки (±2 полутона вместо ±12) — OK")
        else:
            log.append("   TUNE: шаблон не найден (пропущено)")
        if fname.endswith("_par.hpp"):
            for knob, member in (("frq1","m_ratioMod1"), ("frq2","m_ratioMod2"), ("frq3","m_ratioMod3")):
                pat = re.compile(member + r" = getFmListedRatio\(static_cast<uint8_t>\(%s / 4\)\);" % knob)
                if pat.search(text):
                    text = pat.sub(f"{member} = fmRatioExact({knob});", text)
                    log.append(f"   {knob}: fmRatioExact (1/64..4) — OK")
                elif "fmRatioExact(" + knob in text:
                    log.append(f"   {knob}: уже исправлено")
                else:
                    log.append(f"   {knob}: шаблон не найден (пропущено)")
            if "inline float fmRatioExact" not in text:
                helper = MARKER + r"""
// Каноническая таблица STAT/PAR: Y:$141A80, 24 ступени, слово/2^20 = 1/64..4.
inline float fmRatioExact(uint8_t K) {
    static constexpr float kR[24] = {
        0.015625f, 0.03125f, 0.0625f, 0.09375f, 0.125f, 0.15625f,
        0.1875f, 0.25f, 0.3125f, 0.375f, 0.4375f, 0.5f,
        0.625f, 0.75f, 0.875f, 1.0f, 1.25f, 1.5f,
        1.75f, 2.0f, 2.5f, 3.0f, 3.5f, 4.0f };
    const int n = (((static_cast<int>(K) << 16) + 0x8000) * 48) >> 24;
    return kR[n < 0 ? 0 : (n > 23 ? 23 : n)];
}"""
                m = re.search(r"\ninline float getFmListedRatio\(uint8_t index\) \{.*?\n\}\n", text, re.S)
                if m:
                    text = text[:m.end()] + "\n" + helper + "\n" + text[m.end():]
                    log.append("   хелпер fmRatioExact вставлен — OK")
                else:
                    log.append("   !! нет места для хелпера fmRatioExact — пришли файл, наложим вручную")
            else:
                log.append("   хелпер fmRatioExact уже есть")
        else:
            pat1 = re.compile(r"m_ratioMod1Linear = \(static_cast<float>\(frq1\) / 16\.0f\);")
            if pat1.search(text):
                text = pat1.sub("m_ratioMod1Linear = (frq1 >= 127 ? 2.0f : static_cast<float>(frq1) / 64.0f); "
                                "// FM-FIX: K/64, K=127 -> 2.0", text)
                log.append("   1FRQ: K/64 (макс 2.0) — OK")
            elif "FM-FIX: K/64" in text:
                log.append("   1FRQ: уже исправлено")
            else:
                log.append("   1FRQ: шаблон не найден (пропущено)")
            pat2 = re.compile(r"float expVal = \(static_cast<float>\(frq2\) - 32\.0f\) / 24\.0f;\s*\n\s*m_ratioMod2Exp = std::pow\(2\.0f, expVal\);")
            if pat2.search(text):
                text = pat2.sub("float x2 = (frq2 >= 127 ? 2.0f : static_cast<float>(frq2) / 64.0f); "
                                "// FM-FIX: (K/64)^2, K=127 -> 4.0\n"
                                "        m_ratioMod2Exp = x2 * x2;", text)
                log.append("   2FRQ: (K/64)^2 (макс 4.0) — OK")
            elif "FM-FIX: (K/64)^2" in text:
                log.append("   2FRQ: уже исправлено")
            else:
                log.append("   2FRQ: шаблон не найден (пропущено)")
        if text != orig:
            bak = path + ".bak_fmfix5"
            if not os.path.exists(bak):
                shutil.copy2(path, bak)
            open(path, "w", encoding="utf-8").write(text)
            print(f"   {fname}: ИЗМЕНЁН")
        else:
            print(f"   {fname}: без изменений")
        for l in log:
            print(l)

MARKER = "// === FM-FIX 5 (2026-09-28) ==="

def main():
    if len(sys.argv) < 2:
        raise SystemExit(__doc__)
    src = find_source(sys.argv[1])
    print("Source:", src, "\n")
    copy_mnm(src)
    patch_editor(src)
    patch_modes(src)
    patch_old_engines(src)
    print("\nГОТОВО. Собери плагин и прогони чек-лист из 00_README_FIRST_RU.txt.")
    print("Повторный запуск скрипта безопасен (покажет 'already identical').")

if __name__ == "__main__":
    main()
