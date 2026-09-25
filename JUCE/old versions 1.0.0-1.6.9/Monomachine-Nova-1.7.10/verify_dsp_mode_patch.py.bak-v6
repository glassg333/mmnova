#!/usr/bin/env python3
"""Static checks for the DSP MODE / filter modes patch (revision v5).

Компилятор не нужен: скрипт читает исходники и проверяет обещания правки.

  1. dsp/mnm/MnmRealFilter.hpp -- основной фильтр: реальные таблицы, устойчивый
     TPT-SVF, DC-блокер, порядок аргументов страницы FILT, живое отпускание ноты.
  2. dsp/mnm/MnmKernel.hpp -- дубль FilterCore/real_detail удалён, std::clamp исправлен.
  3. dsp/mnm/MnmFilterModes.hpp и режимы cascade/dual/raw -- УДАЛЕНЫ (v5),
     ни в NovaDSP.h, ни в реестре режимов их быть не должно.
  4. models/DspModes.hpp -- 5 режимов (mnm=0 old=1 dist2=2 fm2=3 bbox2=4),
     список режимов у каждого раздела, перевод старой нумерации.
  5. NovaDSP.h -- вызов основного фильтра с правильными ручками, резерв old на месте.
  6. PluginProcessor.cpp -- одно правило допустимости режима + миграция на схему 9.
  7. PluginEditor.cpp -- меню: основной режим первым, резерв отдельной группой.

Run:  python3 verify_dsp_mode_patch.py <path-to-Monomachine_Nova_FX/Source>
"""

from pathlib import Path
import re
import sys


def read(path: Path) -> str:
    return path.read_text(encoding="utf-8", errors="replace")


def balanced(text: str) -> bool:
    return text.count("{") == text.count("}")


def main() -> int:
    root = Path(sys.argv[1] if len(sys.argv) > 1 else ".").resolve()
    real = root / "dsp" / "mnm" / "MnmRealFilter.hpp"
    kernel = root / "dsp" / "mnm" / "MnmKernel.hpp"
    removed = root / "dsp" / "mnm" / "MnmFilterModes.hpp"
    modes = root / "models" / "DspModes.hpp"
    dsp = root / "NovaDSP.h"
    editor = root / "PluginEditor.cpp"
    processor = root / "PluginProcessor.cpp"

    for path in (real, kernel, modes, dsp, editor, processor):
        if not path.is_file():
            raise AssertionError(f"missing file: {path}")

    failures = []

    def check(ok: bool, message: str) -> None:
        print(("OK   " if ok else "FAIL ") + message)
        if not ok:
            failures.append(message)

    def present(text: str, needle: str, message: str) -> None:
        check(needle in text, message)

    def absent(text: str, needle: str, message: str) -> None:
        check(needle not in text, message)

    # 1. Основной фильтр
    text = read(real)
    check(balanced(text), "MnmRealFilter.hpp: скобки сбалансированы")
    present(text, "class RealFilterCore", "MnmRealFilter.hpp: определён RealFilterCore")
    present(text, "using FilterCore = RealFilterCore", "MnmRealFilter.hpp: FilterCore = реальный фильтр")
    present(text, "class MnmResoSVF", "MnmRealFilter.hpp: резонанс -- устойчивый TPT-SVF")
    present(text, "DcBlocker", "MnmRealFilter.hpp: DC-блокер на выходе активного фильтра")
    present(text, "kMaxQ", "MnmRealFilter.hpp: закон Q вынесен в константу kMaxQ")
    present(text, "float envAtk, float envDec, float bofs, float wofs",
            "MnmRealFilter.hpp: порядок аргументов = страница FILT (ATK, DEC, BOFS, WOFS)")
    present(text, "release() noexcept { released_ = true; }",
            "MnmRealFilter.hpp: отпускание ноты возвращает огибающую к нулю")
    absent(text, "bp1 += g1", "MnmRealFilter.hpp: старого Chamberlin больше нет")
    absent(text, "class FilterCore {", "MnmRealFilter.hpp: нет второго класса FilterCore")

    # 2. Ядро без дубля
    text = read(kernel)
    absent(text, "class FilterCore {", "MnmKernel.hpp: дубль класса FilterCore удалён")
    absent(text, "kRealSVF_x0_F528BD", "MnmKernel.hpp: дубль констант SVF удалён")
    absent(text, "namespace real_detail", "MnmKernel.hpp: дубль real_detail удалён")
    present(text, "class FilterCoreLegacy", "MnmKernel.hpp: резервный FilterCoreLegacy сохранён")
    absent(text, "std::clamp(attack,0,127)", "MnmKernel.hpp: std::clamp(float,0,127) исправлен")

    # 3. cascade/dual/raw удалены
    check(not removed.is_file(),
          "dsp/mnm/MnmFilterModes.hpp удалён из сборки (переименован в *.bak-v5)")
    def without_comments(t: str) -> str:
        return "\n".join(line for line in t.splitlines() if not line.lstrip().startswith("//"))

    for word in ("MnmFilterModes.hpp", "SvfCascadeFilter", "DualSectionFilter", "TruthfulRawFilter",
                 "cascadeFilter", "dualFilter", "rawFilter", "rawBlockCounter",
                 "dspModeCascade", "dspModeDual", "dspModeRaw", "filt_detail"):
        hits = [p.name for p in (dsp, modes, processor, editor, real, kernel) if word in without_comments(read(p))]
        if hits:
            check(False, f"{word} остался в коде: " + ", ".join(hits))
        else:
            check(True, f"{word} не встречается в коде DSP-части")

    # 4. Реестр режимов
    text = read(modes)
    present(text, '"mnm|old|dist2|fm2|bbox2"', "DspModes.hpp: список из 5 режимов")
    check(re.search(r"dspModeMnm\s*=\s*0\b", text) is not None, "dspModeMnm = 0")
    check(re.search(r"dspModeOld\s*=\s*1\b", text) is not None, "dspModeOld = 1")
    check(re.search(r"dspModeDist2\s*=\s*2\b", text) is not None, "dspModeDist2 = 2")
    check(re.search(r"dspModeFm2\s*=\s*3\b", text) is not None, "dspModeFm2 = 3")
    check(re.search(r"dspModeBbox2\s*=\s*4\b", text) is not None, "dspModeBbox2 = 4")
    check(re.search(r"dspModeCount\s*=\s*5\b", text) is not None, "dspModeCount = 5")
    present(text, "dspSectionModeChoices", "DspModes.hpp: список режимов на каждый раздел")
    present(text, "dspModeAllowedForSection", "DspModes.hpp: правило допустимости режима")
    present(text, "dspModeLegacyIndexToCurrent", "DspModes.hpp: перевод старой нумерации режимов")
    present(text, "kDspModeSchemaVersion = 9", "DspModes.hpp: версия схемы состояния = 9")
    present(text, "kShowBackupModes", "DspModes.hpp: резервные режимы скрываются флагом")
    table = re.search(r"dspSectionModeChoices\(int section\)\s*\{(.*?)\n\}", text, re.S)
    check(table is not None, "DspModes.hpp: найдена таблица режимов по разделам")
    body = table.group(1) if table else ""
    synt = re.search(r"case DspSynt:\s+return \"([^\"]+)\"", body)
    filt = re.search(r"case DspFilter:\s+return \"([^\"]+)\"", body)
    dist = re.search(r"case DspDist:\s+return \"([^\"]+)\"", body)
    route = re.search(r"case DspRouting:\s+return \"([^\"]+)\"", body)
    check(bool(synt) and "fm2" in synt.group(1) and "bbox2" in synt.group(1) and "dist2" not in synt.group(1),
          "DspModes.hpp: в SYNT только синтовые резервы (fm2/bbox2)")
    check(bool(filt) and filt.group(1) == "mnm|old",
          "DspModes.hpp: у FILT основной mnm и резерв old")
    check(bool(dist) and "dist2" in dist.group(1),
          "DspModes.hpp: в DIST есть свой резерв dist2")
    check(bool(route) and "|" not in route.group(1), "DspModes.hpp: у ROUTE альтернатив нет")

    # 5. Диспетчер фильтра
    text = read(dsp)
    present(text, "MnmRealFilter.hpp", "NovaDSP.h: подключён основной фильтр")
    call = re.search(r"mnmFilter\.setParameters\(([^;]*)\)", text)
    check(call is not None, "NovaDSP.h: вызов основного фильтра найден")
    if call:
        args = re.sub(r"\s+", "", call.group(1))
        check("params[20]-64" not in args and "params[21]-64" not in args,
              "NovaDSP.h: ATK/DEC не передаются как BOFS/WOFS (баг «по частотам» закрыт)")
        check(args == "fltBase,fltWidth,fltHpq,fltLpq,fltAtk,fltDec,fltBofs,fltWofs",
              "NovaDSP.h: порядок аргументов fltBase..fltWofs")
    present(text, "filter.processStereo(l,r,l,r", "NovaDSP.h: резервный режим old (MonomachineFilter) на месте")

    # 6. Аудио-поток
    text = read(processor)
    present(text, "dspModeAllowedForSection(i,idx)",
            "PluginProcessor.cpp: режим проверяется списком своего раздела")
    absent(text, "dspModeIsFilterOnly(idx)", "PluginProcessor.cpp: ad-hoc правил больше нет")
    present(text, "dspModeLegacyIndexToCurrent", "PluginProcessor.cpp: старая нумерация переводится")
    present(text, "state.setProperty(\"schema\",monomachine::kDspModeSchemaVersion,nullptr)",
            "PluginProcessor.cpp: схема состояния обновлена до 9")
    present(text, "static_cast<float>(monomachine::dspModeOld),nullptr",
            "PluginProcessor.cpp: старые состояния без режимов заполняются old (как в DeliveryTests)")

    # 7. Меню
    text = read(editor)
    bounds = re.search(r"dspModeButton\.setBounds\(([^)]*)\)", text)
    check(bounds is not None, "PluginEditor.cpp: dspModeButton позиционируется")
    if bounds:
        values = [int(v) for v in re.findall(r"-?\d+", bounds.group(1))]
        x, y, w = values[0], values[1], values[2]
        check(y != 58 or x >= 1014,
              f"PluginEditor.cpp: dspModeButton в ({x},{y}) не залезает на строку ARP")
        check(x >= 1014 or y != 58, "PluginEditor.cpp: dspModeButton вынесена из строки ARP")
    present(text, "dspModePrimaryForSection", "PluginEditor.cpp: в меню основной режим идёт первым")
    present(text, "dspModeAllowedForSection", "PluginEditor.cpp: меню предлагает только режимы раздела")
    present(text, "РЕЗЕРВ", "PluginEditor.cpp: резервные режимы помечены отдельной группой")

    if failures:
        print(f"\nFAIL: {len(failures)} check(s) failed", file=sys.stderr)
        return 1
    print("\nPASS static patch verification")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
