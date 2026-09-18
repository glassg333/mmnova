#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""verify_fix_v6.py -- проверка дерева Monomachine-Nova-1.6.3 после правки v6.

Ничего не меняет, компилятор не нужен: читает исходники и проверяет обещания правки.

Запуск:  python verify_fix_v6.py "E:\\mm\\Monomachine-Nova-1.6.3"

Проверяет по каждому проекту (Synth и FX):
  1. dsp/monomachine_bbox.hpp -- старый закон RTIM (5...255 мс) с инверсией,
     интервал повтора не ноль, сглаживание по сэмплам, рабочий кит = старый кит
     + снейр/томы, старый кит рядом (generateLegacyKit).
  2. models/DspModes.hpp -- ровно два режима (mnm основной, old резерв), список
     режимов на каждый раздел, версия схемы состояния 10.
  3. NovaDSP.h -- альтернативных движков dist2/fm2/bbox2 нет, хорус и FX как в 1.6.0
     (линейный INP, без домешиваний), ревер не переполняет стек, фильтр на месте.
  4. MnmDist2.hpp / MnmFm2.hpp / MnmBbox2.hpp / MnmFilterModes.hpp -- выведены из сборки.
  5. PluginProcessor.cpp / PluginEditor.cpp -- правило режимов и меню.
  6. tests/DeliveryTests.cpp -- ожидания обновлены под v6.

Итог: "ИТОГ: ВСЁ НА МЕСТЕ" или "ИТОГ: НЕ ГОТОВО, проблем: N".
"""

from pathlib import Path
import sys

OK = "OK  "
BAD = "!!  "

problems = []
notes = []


def read(path: Path) -> str:
    return path.read_text(encoding="utf-8", errors="replace")


def without_comments(text: str) -> str:
    return "\n".join(line for line in text.splitlines() if not line.lstrip().startswith("//"))


def check(cond: bool, message: str) -> None:
    print("  " + (OK if cond else BAD) + message)
    if not cond:
        problems.append(message)


def check_file(path: Path, what: str) -> str:
    if not path.is_file():
        print("  " + BAD + "нет файла: " + str(path) + " (" + what + ")")
        problems.append("нет файла " + str(path))
        return ""
    return read(path)


def main() -> int:
    root = Path(sys.argv[1] if len(sys.argv) > 1 else ".").resolve()
    if not root.is_dir():
        print("нет папки: " + str(root))
        return 2
    projects = [p for p in ("Monomachine_Nova_Synth", "Monomachine_Nova_FX") if (root / p).is_dir()]
    if not projects:
        print("в папке нет проектов Monomachine_Nova_Synth / Monomachine_Nova_FX: " + str(root))
        return 2

    print("=== проверка правки v6: " + str(root) + " ===")
    print("проекты: " + ", ".join(projects))

    for proj in projects:
        print("\n--- " + proj + " ---")
        src = root / proj / "Source"

        # 1. BBOX
        text = check_file(src / "dsp" / "monomachine_bbox.hpp", "движок BBOX")
        if text:
            check("float retrigMs = 5.0f + ((127.0f - static_cast<float>(rtim)) / 127.0f) * 250.0f;" in text,
                  "BBOX: RTIM = старая формула 5...255 мс с инверсией (127 = самый частый повтор)")
            check("255.0f * std::pow((127.0f-static_cast<float>(rtim))/126.0f,2.0f)" not in text,
                  "BBOX: формулы с нулевым интервалом больше нет")
            check("static constexpr size_t kDeclickSamples = 16;" in text,
                  "BBOX: сглаживание по СЭМПЛАМ (kDeclickSamples = 16)")
            check("samplesToRestart" in text,
                  "BBOX: спад доходит ровно до нуля к перезапуску (щелчка нет)")
            check("static const bool modernVoice[24]=" in text,
                  "BBOX: таблица голосов нового кита (снейр/том 1/том 2)")
            check("*0.125" in text, "BBOX: STRT ограничен 1/8 активной длины (короткие удары целее)")
            check("void generateLegacyKit()" in text, "BBOX: полностью старый кит оставлен рядом")
            check("kMaxSlots = 24" in text, "BBOX: 24 слота на месте")
            check("kDeclickSamples = 8" not in text, "BBOX: прежняя длина сглаживания не осталась")

        # 2. Реестр режимов
        text = check_file(src / "models" / "DspModes.hpp", "реестр режимов")
        if text:
            check('return "mnm|old";' in text, "DspModes: всего два режима (mnm|old)")
            check("dspModeCount = 2" in text, "DspModes: dspModeCount = 2")
            check("kDspModeSchemaVersion = 10" in text, "DspModes: версия схемы состояния = 10")
            check("dspSectionModeChoices" in text, "DspModes: список режимов на каждый раздел")
            check("dspModeAllowedForSection" in text, "DspModes: правило допустимости режима")
            check("dspModeLegacyIndexToCurrent" in text, "DspModes: перевод прежних индексов режимов")
            for word in ("dspModeDist2", "dspModeFm2", "dspModeBbox2", "dspModeIsDistOnly", "dspModeIsSyntOnly"):
                check(word not in without_comments(text), "DspModes: нет остатков режима " + word)

        # 3. Диспетчер
        text = check_file(src / "NovaDSP.h", "диспетчер DSP")
        if text:
            code = without_comments(text)
            check("MnmRealFilter.hpp" in text, "NovaDSP: подключён реальный фильтр (основной режим mnm)")
            check("fltAtk,fltDec,fltBofs,fltWofs" in text,
                  "NovaDSP: ручки страницы FILT передаются в правильном порядке")
            check("filter.processStereo(l,r,l,r" in text, "NovaDSP: резервный фильтр old на месте")
            check("bbox.setParameters(b(0),b(1),b(4),b(5),b(2),b(3)>0,false)" in text,
                  "NovaDSP: BBOX получает ручки как в 1.6.0")
            check("bbox.setChromatic(b(6)>0)" in text, "NovaDSP: хроматический режим BBOX на месте")
            check("if(id!=15){const float g=p[7]/64.0f;" in text,
                  "NovaDSP: INP линейный, как в 1.6.0 (квадратичного закона нет)")
            check("const float wet=norm(p[3]);l[i]=dl*(1-wet)+wl*wet;" in text,
                  "NovaDSP: хорус микшируется dry->wet по MIX, без домешивания x1.5")
            check("rvDryL" in text and "std::array<float,32> dryL" not in text,
                  "NovaDSP: буферы сухого сигнала ревера -- под размер блока (падение закрыто)")
            for word in ("inpGainQuad", "chorusMakeup", "reverbMakeup",
                         "fm2Core", "bbox2Core", "dist2Core", "FMPlusBlock2", "Bbox2Core", "Dist2Core"):
                check(word not in code, "NovaDSP: нет остатков " + word)
            for name in ("MnmDist2.hpp", "MnmFm2.hpp", "MnmBbox2.hpp"):
                check(name not in text, "NovaDSP: заголовок " + name + " больше не подключается")

        # 4. Удалённые файлы
        for name, bak in (("MnmDist2.hpp", "MnmDist2.hpp.bak-v6"),
                          ("MnmFm2.hpp", "MnmFm2.hpp.bak-v6"),
                          ("MnmBbox2.hpp", "MnmBbox2.hpp.bak-v6"),
                          ("MnmFilterModes.hpp", "MnmFilterModes.hpp.bak-v5")):
            mnm = src / "dsp" / "mnm"
            check(not (mnm / name).is_file(), "dsp/mnm/" + name + " выведен из сборки")
            if (mnm / bak).is_file():
                print("  " + OK + "страховочная копия на месте: " + bak)
            else:
                notes.append("нет копии " + bak + " (это не ошибка, но бэкапа нет)")

        # 5. Аудио-поток и меню
        text = check_file(src / "PluginProcessor.cpp", "аудио-поток")
        if text:
            check("dspModeAllowedForSection(i,idx)" in text, "Processor: режим проверяется списком своего раздела")
            check("dspModeLegacyIndexToCurrent" in text, "Processor: прежние индексы режимов переводятся")
            check("if(schema<10)" in text, "Processor: миграция состояний до схемы 10")
            check('state.setProperty("schema",monomachine::kDspModeSchemaVersion,nullptr)' in text,
                  "Processor: версия схемы пишется в состояние")
        text = check_file(src / "PluginEditor.cpp", "интерфейс")
        if text:
            check("dspModePrimaryForSection" in text, "Editor: основной режим в меню идёт первым")
            check("dspModeAllowedForSection" in text, "Editor: меню предлагает только режимы раздела")
            check("РЕЗЕРВ" in text, "Editor: резерв помечен отдельной группой")

        # 6. Тесты
        tests = root / proj / "tests" / "DeliveryTests.cpp"
        if tests.is_file():
            text = read(tests)
            check("RTIM 127 must be about 5 ms, not zero" in text, "DeliveryTests: ожидание нового закона RTIM")
            check("new snare/toms missing from the working kit" in text, "DeliveryTests: ожидание рабочего кита")

    # Патч-скрипт рядом
    for verifier in (root / "verify_dsp_mode_patch.py",):
        if verifier.is_file():
            text = read(verifier)
            check("revision v6" in text, "verify_dsp_mode_patch.py обновлён под v6")
            check('"mnm|old"' in text, "verify_dsp_mode_patch.py знает про два режима")
        else:
            notes.append("verify_dsp_mode_patch.py рядом с деревом не найден")

    print("\n=== прочее ===")
    backups6 = sorted(root.rglob("*.bak-v6"))
    print("  " + OK + "бэкапов *.bak-v6 сохранено: " + str(len(backups6)))
    if notes:
        print("\nзаметки (не ошибки):")
        for n in notes:
            print("  - " + n)

    if problems:
        print("\nИТОГ: НЕ ГОТОВО, проблем: " + str(len(problems)))
        for p in problems:
            print("   - " + p)
        return 1
    print("\nИТОГ: ВСЁ НА МЕСТЕ.")
    print("      BBOX: старый кит (кик/клэп/колокол/лазер) + новые снейр и два тома,")
    print("      старый закон RTIM с инверсией и деклик по сэмплам, звук не пропадает на 100...127.")
    print("      Хорус и FX -- как в 1.6.0; в меню остались только mnm (основной) и old (резерв);")
    print("      новые режимы -- только у фильтра (FILT mnm = реальный фильтр из дампа).")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
