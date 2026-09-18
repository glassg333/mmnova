#!/usr/bin/env python3
"""Static checks for the DSP MODE / filter modes patch (revision v6).

Компилятор не нужен: скрипт читает исходники и проверяет обещания правки.

  1. dsp/mnm/MnmRealFilter.hpp -- основной фильтр: реальные таблицы, устойчивый
     TPT-SVF, DC-блокер, порядок аргументов страницы FILT, живое отпускание ноты.
  2. dsp/mnm/MnmKernel.hpp -- дубль FilterCore/real_detail удалён, std::clamp исправлен.
  3. dsp/mnm/MnmFilterModes.hpp (cascade/dual/raw) и альтернативные движки
     MnmDist2.hpp / MnmFm2.hpp / MnmBbox2.hpp -- УДАЛЕНЫ (v5/v6).
  4. models/DspModes.hpp -- 2 режима (mnm=0 основной, old=1 резерв), свой список
     режимов у каждого раздела, схема состояния 10.
  5. NovaDSP.h -- основной фильтр с правильными ручками, резерв old на месте,
     никаких веток dist2/fm2/bbox2, хорус и FX как в 1.6.0 (линейный INP, без домешиваний).
  6. dsp/monomachine_bbox.hpp -- BBOX: старый закон RTIM (5...255 мс) с инверсией,
     интервал никогда не 0, сглаживание по сэмплам, рабочий кит = старый + снейр/томы.
  7. PluginProcessor.cpp -- одно правило допустимости режима + миграция на схему 10.
  8. PluginEditor.cpp -- меню: основной режим первым, резерв отдельной группой.

Run:  python3 verify_dsp_mode_patch.py <path-to-Monomachine_Nova_FX/Source>
"""

from pathlib import Path
import re
import sys


def read(path: Path) -> str:
    return path.read_text(encoding="utf-8", errors="replace")


def balanced(text: str) -> bool:
    return text.count("{") == text.count("}")


def without_comments(text: str) -> str:
    return "\n".join(line for line in text.splitlines() if not line.lstrip().startswith("//"))


def main() -> int:
    root = Path(sys.argv[1] if len(sys.argv) > 1 else ".").resolve()
    real = root / "dsp" / "mnm" / "MnmRealFilter.hpp"
    kernel = root / "dsp" / "mnm" / "MnmKernel.hpp"
    removed = root / "dsp" / "mnm" / "MnmFilterModes.hpp"
    bbox = root / "dsp" / "monomachine_bbox.hpp"
    modes = root / "models" / "DspModes.hpp"
    dsp = root / "NovaDSP.h"
    editor = root / "PluginEditor.cpp"
    processor = root / "PluginProcessor.cpp"

    for path in (real, kernel, bbox, modes, dsp, editor, processor):
        if not path.is_file():
            raise AssertionError(f"missing file: {path}")

    tests = root.parent / "tests" / "DeliveryTests.cpp"

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

    # 3. Мусорные режимы удалены
    check(not removed.is_file(), "dsp/mnm/MnmFilterModes.hpp удалён из сборки (*.bak-v5)")
    for name in ("MnmDist2.hpp", "MnmFm2.hpp", "MnmBbox2.hpp"):
        check(not (root / "dsp" / "mnm" / name).is_file(),
              f"dsp/mnm/{name} удалён из сборки (*.bak-v6)")

    live = [p for p in (dsp, modes, processor, editor, real, kernel, bbox) ]
    for word in ("MnmFilterModes.hpp", "SvfCascadeFilter", "DualSectionFilter", "TruthfulRawFilter",
                 "cascadeFilter", "dualFilter", "rawFilter", "rawBlockCounter",
                 "dspModeCascade", "dspModeDual", "dspModeRaw", "filt_detail",
                 "MnmDist2.hpp", "MnmFm2.hpp", "MnmBbox2.hpp",
                 "Dist2Core", "FMPlusBlock2", "Bbox2Core",
                 "dspModeDist2", "dspModeFm2", "dspModeBbox2",
                 "fm2Core", "bbox2Core", "dist2Core",
                 "dspModeIsDistOnly", "dspModeIsSyntOnly"):
        hits = [p.name for p in live if word in without_comments(read(p))]
        if hits:
            check(False, f"{word} остался в коде: " + ", ".join(hits))
        else:
            check(True, f"{word} не встречается в коде DSP-части")

    # 4. Реестр режимов
    text = read(modes)
    present(text, '"mnm|old"', "DspModes.hpp: список режимов = mnm|old (всего два)")
    check(re.search(r"dspModeMnm\s*=\s*0\b", text) is not None, "dspModeMnm = 0 (основной)")
    check(re.search(r"dspModeOld\s*=\s*1\b", text) is not None, "dspModeOld = 1 (резерв)")
    check(re.search(r"dspModeCount\s*=\s*2\b", text) is not None, "dspModeCount = 2")
    present(text, "dspSectionModeChoices", "DspModes.hpp: список режимов на каждый раздел")
    present(text, "dspModeAllowedForSection", "DspModes.hpp: правило допустимости режима")
    present(text, "dspModeLegacyIndexToCurrent", "DspModes.hpp: перевод старой нумерации режимов")
    present(text, "kDspModeSchemaVersion = 10", "DspModes.hpp: версия схемы состояния = 10")
    present(text, "kShowBackupModes", "DspModes.hpp: резервные режимы скрываются флагом")
    table = re.search(r"dspSectionModeChoices\(int section\)\s*\{(.*?)\n\}", text, re.S)
    check(table is not None, "DspModes.hpp: найдена таблица режимов по разделам")
    body = table.group(1) if table else ""
    for section, expect in (("DspSynt", "mnm|old"), ("DspAmp", "mnm|old"), ("DspFilter", "mnm|old"),
                            ("DspDist", "mnm|old"), ("DspDelay", "mnm|old"),
                            ("DspRouting", "mnm"), ("DspChorus", "mnm")):
        m = re.search(r"case %s:\s+return \"([^\"]+)\"" % section, body)
        check(bool(m) and m.group(1) == expect,
              f"DspModes.hpp: {section} -> {expect}")
    present(text, "case DspFilter: return \"FILT: mnm=РЕАЛЬНЫЙ фильтр",
            "DspModes.hpp: у FILT основной режим описан как реальный фильтр прошивки")

    # 5. Диспетчер
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
    present(text, "bbox.setParameters(b(0),b(1),b(4),b(5),b(2),b(3)>0,false)",
            "NovaDSP.h: BBOX получает ручки как в 1.6.0 (PTCH,ST/RT,START-позиция,SLOT,RND)")
    present(text, "bbox.setChromatic(b(6)>0)", "NovaDSP.h: хроматический режим BBOX на месте")
    present(text, "if(id!=15){const float g=p[7]/64.0f;",
            "NovaDSP.h: INP для FX -- линейный, как в 1.6.0 (без квадратичного «FIXED»)")
    present(text, "const float wet=norm(p[3]);l[i]=dl*(1-wet)+wl*wet;",
            "NovaDSP.h: хорус микшируется dry->wet по MIX (как в 1.6.0, без домешивания x1.5)")
    for word in ("inpGainQuad", "chorusMakeup", "reverbMakeup", "FIXED"):
        absent(without_comments(text), word, f"NovaDSP.h: нет остатков приёма «{word}»")

    # 6. BBOX
    text = read(bbox)
    check(balanced(text), "monomachine_bbox.hpp: скобки сбалансированы")
    present(text, "float retrigMs = 5.0f + ((127.0f - static_cast<float>(rtim)) / 127.0f) * 250.0f;",
            "BBOX: старый закон RTIM (5...255 мс) с инверсией")
    absent(text, "255.0f * std::pow((127.0f-static_cast<float>(rtim))/126.0f,2.0f)",
           "BBOX: прежняя формула с интервалом 0 мс удалена")
    present(text, "static constexpr size_t kDeclickSamples = 16;",
            "BBOX: сглаживание задано в сэмплах (kDeclickSamples)")
    present(text, "samplesToRestart", "BBOX: спад доходит до нуля к перезапуску (деклик)")
    present(text, "static const bool modernVoice[24]=", "BBOX: таблица голосов нового кита")
    present(text, "*0.125", "BBOX: STRT ограничен 1/8 активной длины (короткие удары целее)")
    present(text, "void generateLegacyKit()", "BBOX: полностью старый кит оставлен рядом")

    # 7. Аудио-поток
    text = read(processor)
    present(text, "dspModeAllowedForSection(i,idx)",
            "PluginProcessor.cpp: режим проверяется списком своего раздела")
    absent(text, "dspModeIsFilterOnly(idx)", "PluginProcessor.cpp: ad-hoc правил больше нет")
    present(text, "dspModeLegacyIndexToCurrent", "PluginProcessor.cpp: старая нумерация переводится")
    present(text, "state.setProperty(\"schema\",monomachine::kDspModeSchemaVersion,nullptr)",
            "PluginProcessor.cpp: схема состояния обновлена")
    present(text, "if(schema<10)", "PluginProcessor.cpp: миграция состояний до схемы 10")
    present(text, "static_cast<float>(monomachine::dspModeOld),nullptr",
            "PluginProcessor.cpp: старые состояния без режимов заполняются old (как в DeliveryTests)")

    # 8. Меню
    text = read(editor)
    bounds = re.search(r"dspModeButton\.setBounds\(([^)]*)\)", text)
    check(bounds is not None, "PluginEditor.cpp: dspModeButton позиционируется")
    if bounds:
        values = [int(v) for v in re.findall(r"-?\d+", bounds.group(1))]
        x, y = values[0], values[1]
        check(y != 58 or x >= 1014, "PluginEditor.cpp: dspModeButton не залезает на строку ARP")
    present(text, "dspModePrimaryForSection", "PluginEditor.cpp: в меню основной режим идёт первым")
    present(text, "dspModeAllowedForSection", "PluginEditor.cpp: меню предлагает только режимы раздела")
    present(text, "РЕЗЕРВ", "PluginEditor.cpp: резервные режимы помечены отдельной группой")

    # 9. Тесты
    if tests.is_file():
        text = read(tests)
        present(text, "RTIM 127 must be about 5 ms, not zero",
                "DeliveryTests.cpp: ожидание нового закона RTIM")
        present(text, "new snare/toms missing from the working kit",
                "DeliveryTests.cpp: ожидание рабочего кита (старый + снейр/томы)")
    else:
        print("..   tests/DeliveryTests.cpp не найден рядом с проектом -- проверки тестов пропущены")

    if failures:
        print(f"\nFAIL: {len(failures)} check(s) failed", file=sys.stderr)
        return 1
    print("\nPASS static patch verification")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
