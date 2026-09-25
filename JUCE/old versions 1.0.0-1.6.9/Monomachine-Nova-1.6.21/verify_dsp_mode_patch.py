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
  9. Обещания 1.6.5: unity-дилей (wet отдельно, dry не удваивается), repitch-slew
     без кликов, раздельные FM-файлы (stat/par), FM+STAT old удалён, FM+PAR old
     с устойчивым полом огибающих, ROUTE скрыт из меню, папки-категории и
     описание цепочки, параметр dly_repitch, drag-and-release модуляция с
     кнопок LFO, /utf-8 в .jucer и CMake для MSVC.

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
    present(text, "kDspModeSchemaVersion = 11", "DspModes.hpp: версия схемы состояния = 11 (per-machine SYNT mode)")
    present(text, "dspMachineModeParamId", "DspModes.hpp: режим SYNT хранится по каждой машине (mode_synt_m<id>)")
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
        present(text, r'case DspFilter: return "FILT: mnm = the real filter',
            "DspModes.hpp: у FILT основной режим описан как реальный фильтр прошивки")
    absent(text, r'"FILT: mnm = ' + '\\xd0',
            "DspModes.hpp: русские \\xNN-строки описаний режимов убраны (кракозябры)")

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
    present(text, "BACKUP (not the primary engine):", "PluginEditor.cpp: резервные режимы помечены отдельной группой (1.6.8: текст на английском)")
    present(text, "ENCODING TEST: ", "PluginEditor.cpp: проверочная строка кодировки на месте")
    absent(text, "DSP MODE -- \\xd0", "PluginEditor.cpp: русские \\xNN-строки тултипа убраны")

    # 9. Тесты
    if tests.is_file():
        text = read(tests)
        present(text, "RTIM 127 must be about 5 ms, not zero",
                "DeliveryTests.cpp: ожидание нового закона RTIM")
        present(text, "new snare/toms missing from the working kit",
                "DeliveryTests.cpp: ожидание рабочего кита (старый + снейр/томы)")
    else:
        print("..   tests/DeliveryTests.cpp не найден рядом с проектом -- проверки тестов пропущены")

    # 10. Обещания 1.6.5
    delay = root / "dsp" / "mnm" / "MnmDelay.hpp"
    fmpar = root / "dsp" / "monomachine_fm_par.hpp"
    fmstat = root / "dsp" / "monomachine_fm_stat.hpp"
    fmold = root / "dsp" / "monomachine_fm_stat_par.hpp"
    data = root / "NovaData.h"
    header = root / "PluginProcessor.h"

    text = read(delay)
    present(text, "float send, bool pingPong,", "1.6.5 MnmDelay: send вместо mix в сигнатуре")
    present(text, "(pingPong ? 0.5f * (inL + inR) : inL) * send", "1.6.5 MnmDelay: в линию пишется вход*send (топология old, 1.6.6)")
    present(text, "outL = aL;", "1.6.5 MnmDelay: выход -- чистый wet, dry не удваивается")
    present(text, "smoothSamples", "1.6.5 MnmDelay: slewing длины линии (repitch без кликов)")

    text = read(dsp)
    present(text, "dsp/monomachine_fm_par.hpp", "1.6.5 NovaDSP: FM+PAR в отдельном файле")
    present(text, "dsp/monomachine_fm_stat.hpp", "1.6.5 NovaDSP: FM+STAT в отдельном файле")
    absent(text, "monomachine_fm_stat_par.hpp", "1.6.5 NovaDSP: совмещённый fm_stat_par не подключён")
    absent(text, "MonomachineFmStatic stat;", "1.6.5 NovaDSP: old-движок FM+STAT удалён")
    absent(text, "stat.processStereo", "1.6.5 NovaDSP: вызовов stat больше нет")
    present(text, "(syntMode==monomachine::dspModeMnm||id==8)&&(id==8||id==9||id==10)",
            "1.6.5 NovaDSP: машина 8 (FM+STAT) всегда на mnm")
    present(text, "mnmDelay.process(params[27],feedback,send,pingPong,std::max(repitchSlew,declickSlew),dryL,dryR,wetL,wetR)",
            "1.6.5 NovaDSP: wet добавляется один раз (unity thru)")
    present(text, "void setRepitch(float raw,float smoothRaw)", "1.6.5/1.6.8 NovaDSP: настройка скорости repitch (непрерывная шкала + антиклик)")

    check(fmpar.is_file(), "1.6.5 monomachine_fm_par.hpp существует")
    if fmpar.is_file():
        t2 = read(fmpar)
        check(balanced(t2), "1.6.5 fm_par: скобки сбалансированы")
        present(t2, "class MonomachineFmParallel", "1.6.5 fm_par: класс FM+PAR на месте")
        present(t2, "getFmListedRatio", "1.6.5 fm_par: таблица ratios на месте")
        present(t2, "kSustain = 0.45f", "1.6.5 fm_par: устойчивый пол 45% (1FRQ/1ENV слышны всю ноту)")
        present(t2, "0.25f * static_cast<float>(m_sampleRate)", "1.6.5 fm_par: огибающие 0.25/0.45/0.65 с вместо 20 мс")
    check(fmstat.is_file(), "1.6.5 monomachine_fm_stat.hpp существует")
    check(not fmold.is_file(), "1.6.5 совмещённый monomachine_fm_stat_par.hpp удалён (.bak-v7)")

    text = read(modes)
    present(text, "dspSectionVisibleInMenu", "1.6.5 DspModes: видимость разделов в меню")
    present(text, "return section != DspRouting;", "1.6.5 DspModes: ROUTE не выбирается")
    present(text, "dspSectionCategory", "1.6.5 DspModes: папки-категории разделов")
    present(text, "dspRouteDescription", "1.6.5 DspModes: подробное описание цепочки для наведения")

    text = read(data)
    present(text, "dly_repitch", "1.6.5/1.6.8 NovaData: параметр dly_repitch (c 1.6.8 непрерывный 0..3)")
    present(text, "dly_repitch_smooth", "1.6.8 NovaData: параметр dly_repitch_smooth (антиклик-доводка)")

    text = read(processor)
    present(text, "addRouteFromSource", "1.6.5 Processor: маршрут из drag-and-release")
    present(text, "chain.setRepitch", "1.6.5 Processor: repitch доходит до цепочки")
    if header.is_file():
        present(read(header), "void addRouteFromSource(int src, uint8_t target);",
                "1.6.5 Processor.h: объявлен addRouteFromSource")

    text = read(editor)
    present(text, "class ModSourceButton", "1.6.5 Editor: одинаковые кнопки LFO1/2/3")
    present(text, "lfo1Button", "1.6.5 Editor: LFO1 теперь кнопка")
    present(text, "beginModDrag", "1.6.5 Editor: модуляция прицелом перетаскиванием")
    present(text, "endModDrag", "1.6.5 Editor: отпустил на ручке -- маршрут назначен")
    present(text, "\"mseg_rate\",\"RATE\"", "1.6.5 Editor: MSEG-панель с быстрыми ручками")
    absent(text, "showLfo3", "1.6.5 Editor: старого переключателя showLfo3 нет")
    present(text, "BUILD 1.6.11", "1.6.11 Editor: маркер версии сборки (обновляется +1 каждым патчем)")
    present(text, "Monomachine Nova Synth / BUILD 1.6.11", "1.6.11 Editor: версия в About")

    core_tests = root.parent / "tests" / "MnmCoreTests.cpp"
    if core_tests.is_file():
        text = read(core_tests)
        present(text, "DLY send=0 wet is silent", "1.6.5 Tests: unity-проверка дилея")
        present(text, "repitch slew keeps the read pointer continuous", "1.6.5 Tests: repitch без кликов")

    cmake = root.parent / "CMakeLists.txt"
    if cmake.is_file():
        present(read(cmake), "/utf-8", "1.6.5 CMake: /utf-8 для MSVC")
    jucers = list(root.parent.glob("*.jucer"))
    check(bool(jucers) and all('extraCompilerFlags="/utf-8"' in read(j) for j in jucers),
          "1.6.5 jucer: extraCompilerFlags /utf-8 в конфигах VS")

    # ---- 11. обещания 1.6.6
    text = read(root / "dsp" / "mnm" / "MnmKernel.hpp")
    present(text, "holdPeak", "1.6.6 Kernel: hold пика огибающей для HOLD")
    text = read(root / "NovaDSP.h")
    present(text, "kernelEnv.holdPeak()", "1.6.6 Envelope: атака идёт во время hold, пик заморожен")
    present(text, "kernelEnv.setParameters(p[0],p[2],p[3],0.0f)", "1.6.6 Envelope: сустейн не следует за DEC")
    text = read(root / "dsp" / "mnm" / "MnmFm.hpp")
    present(text, "1.6.6: unipolar", "1.6.6 FM DYN: фидбек 0 = нет фидбека")
    text = read(root / "dsp" / "mnm" / "MnmDelay.hpp")
    present(text, "0.5f * (inL + inR)", "1.6.6 Delay: send по закону old DSP (ping-pong моно-микс)")
    text = read(root / "NovaData.h")
    present(text, "m.id==10) defaultMachine", "1.6.6: FM DYN по умолчанию при открытии")
    text = read(root / "PluginEditor.cpp")
    present(text, "class StepLane", "1.6.6 ARP: горизонтальный секвенсор с рисованием")
    present(text, "rebuildModeControls", "1.6.6: MODE-селекторы в заголовках страниц")
    present(text, "showRepitchMenu", "1.6.6: repitch дилея по ПКМ на DTIM")
    present(text, "struct RndButton", "1.6.6 ARP: рандом мгновенно, меню по ПКМ")
    text = read(root.parent / "tests" / "MnmCoreTests.cpp")
    present(text, "DLY 1.6.6 ping-pong send=0", "1.6.6 Tests: новая топология send")

    # ---- 12. обещания 1.6.8
    text = read(modes)
    present(text, 'return "MACHINES (OSC)"', "1.6.8 DspModes: категории меню на английском")
    absent(text, "return \"\\xd0", "1.6.8 DspModes: русских \\xNN-строк категорий больше нет")
    text = read(data)
    present(text, "if(section==monomachine::DspSynt)continue;", "1.6.8 NovaData: общий mode_synt больше не создаётся")
    present(text, "dspMachineModeParamId(m.id)", "1.6.8 NovaData: у каждой машины свой параметр mode_synt_m<id>")
    text = read(processor)
    present(text, "syntModeRaw[static_cast<size_t>(selectedMachine)]", "1.6.8 Processor: SYNT-режим читается из параметра текущей машины")
    present(text, "if(schema<11)", "1.6.8 Processor: миграция mode_synt -> mode_synt_m<id> (схема 11)")
    present(text, "if(slot>0)moveModRoute(slot,0);", "1.6.8 Processor: свежий маршрут LFO всплывает на первую строку матрицы")
    present(text, "if(column<1||column>5)return;", "1.6.8 Processor: колонка ON сортируется")
    text = read(editor)
    present(text, "void showDspModes(int section,juce::Point<int> screen", "1.6.8 Editor: меню режимов открывается у курсора (SRR)")
    present(text, "withTargetScreenArea", "1.6.8 Editor: попап привязан к точке курсора")
    present(text, "class RepitchSliderPanel", "1.6.8 Editor: repitch DTIM -- слайдер, а не список")
    present(text, "launchAsynchronously", "1.6.8 Editor: слайдер repitch в call-out боксе")
    present(text, "const juce::String lfoPanelName=\"LFO\"", "1.6.8 Editor: заголовок LFO-панели без цифры")
    present(text, 'names[]{"SYNT","",lfoPanelName,"FILT","EFFX","MSEG"}', "1.6.8 Editor: LFO наверху, MSEG внизу (панели поменялись)")
    present(text, "aimLfoDest", "1.6.8 Editor: прицел LFO выбирает внутренний PAGE/DEST")
    present(text, "if (c >= 1 && c <= 5) sortBy(c);", "1.6.8 Editor: сортировка матрицы по колонке ON")
    present(text, '"ON", "SOURCE", "DESTINATION", "DEPTH", "MODE"', "1.6.8 Editor: заголовок ON в матрице")
    present(text, "8 FAT SQUARE PADS", "1.6.8 Editor: степы ARP -- квадратные жирные пады")
    present(text, "processor.dspModeParamIdFor(monomachine::DspSynt)", "1.6.8 Editor: SYNT-селектор привязан к параметру выбранной машины")

    # ---- 13. обещания 1.6.8.1
    text = read(editor)
    present(text, "{if(panel==5)continue;", "1.6.8.1 Editor: пропускается только MSEG-панель -- LFO-панель снова с ручками")
    absent(text, "{if(panel==2||panel==5)continue;", "1.6.8.1 Editor: старый пропуск LFO-панели убран (пустая страница LFO)")
    present(text, "lfo1Button.setToggleState(lfoPageSel==0", "1.6.8.1 Editor: выбор LFO-страницы -- заливкой кнопки, без скобок")
    present(text, "void beginModDrag(int src,bool matrix)", "1.6.8.1 Editor: ЛКМ = прямой PAGE/DEST, ПКМ = матрица")
    present(text, "bool modDragMatrix", "1.6.8.1 Editor: флаг режима перетаскивания LFO")
    present(text, "lfo1Button.setBounds(975,97,90,24)", "1.6.8.1 Editor: три одинаковые LFO-кнопки, ровно в шапке панели")
    present(text, "addRow(rep, \"dly_repitch\"", "1.6.8.1 Editor: repitch -- непрерывный слайдер в call-out панели")
    present(text, "addRow(smo, \"dly_repitch_smooth\"", "1.6.8.1 Editor: SMOOTH-слайдер под REPITCH (антиклик)")
    present(text, "FOLDER: ", "1.6.8.1 Editor: DSP MODE -- плоский прокручиваемый список с метками FOLDER")
    absent(text, "menu.addSubMenu(title,sub)", "1.6.8.1 Editor: вложенные подменю DSP MODE убраны")
    text = read(dsp)
    present(text, "anchors[4]={0.0f,0.006f,0.030f,2.5f}", "1.6.8.1 NovaDSP: шкала repitch 0=OFF,1=FAST,2=MED(дефолт),3=~2.5с")
    present(text, "std::max(repitchSlew,declickSlew)", "1.6.8.1 NovaDSP: антиклик-доводка применяется к длине линии")
    text = read(data)
    absent(text, "OFF|FAST|MED|SLOW", "1.6.8.1 NovaData: список OFF/FAST/MED/SLOW больше не выбор -- шкала непрерывная")

    if failures:
        print(f"\nFAIL: {len(failures)} check(s) failed", file=sys.stderr)
        return 1
    print("\nPASS static patch verification")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
