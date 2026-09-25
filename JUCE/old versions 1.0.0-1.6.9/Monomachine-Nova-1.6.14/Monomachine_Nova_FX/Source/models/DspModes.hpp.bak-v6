#pragma once
// DSP mode registry -- Monomachine Nova 1.6.2 (ревизия v5)
// -----------------------------------------------------------------------------
// ГЛАВНОЕ ПРАВИЛО: в каждом разделе -- свой короткий список режимов,
// первым всегда основной "mnm". Резервные режимы помечены и отделены.
//
// ЧТО ИЗМЕНИЛОСЬ В v5
//   * Удалены режимы cascade / dual / raw (индексы 2, 3, 4). Их принёс прошлый
//     патч DSP MODE вместе с файлом dsp/mnm/MnmFilterModes.hpp -- в исходной
//     1.6.x их не было, и ничто, кроме диспетчера фильтра, их не использовало.
//     Вместе с режимами удалены сами классы (SvfCascadeFilter, DualSectionFilter,
//     TruthfulRawFilter) и их ветки в NovaDSP.h.
//   * Нумерация режимов сдвинута: mnm=0, old=1, dist2=2, fm2=3, bbox2=4.
//     Старые состояния читаются: dspModeLegacyIndexToCurrent() переводит
//     прежние индексы (2/3/4 -> mnm, 5/6/7 -> 2/3/4), см. PluginProcessor (schema<9).
//   * Раскладка режимов по разделам: дист -- к дисту, фильтр -- к фильтру,
//     синт -- к синту, чтобы каждый можно было слушать и тестировать в своём разделе.
//
// Числа режимов:
// 0 mnm    -- ОСНОВНОЙ: реальный фильтр/синт/дист/дилей из прошивки OS 1.32B
// 1 old    -- прежние приближения Nova (biquad/tanh/линейный дилей) -- резерв
// 2 dist2  -- резерв DIST: DistortionSaturationBlock (HardClip/Soft/Fold + SRR)
// 3 fm2    -- резерв SYNT: FMPlusBlocks (DYN/STAT/PAR + таблица отношений)
// 4 bbox2  -- резерв SYNT: BBOX retrigger Nova_2KnobFade

#include <array>
#include <cstdint>
#include <cstring>

namespace monomachine {

enum DspSection : int {
    DspSynt = 0,
    DspAmp,
    DspFilter,
    DspDist,
    DspDelay,
    DspRouting,
    DspChorus,
    DspSectionCount
};

inline const char* dspModeParamId(int section) {
    switch (section) {
        case DspSynt:   return "mode_synt";
        case DspAmp:    return "mode_amp";
        case DspFilter: return "mode_filt";
        case DspDist:   return "mode_dist";
        case DspDelay:  return "mode_dly";
        case DspRouting:return "mode_route";
        case DspChorus: return "mode_cho";
        default:        return "mode_unknown";
    }
}

inline const char* dspSectionLabel(int section) {
    switch (section) {
        case DspSynt:   return "SYNT";
        case DspAmp:    return "AMP";
        case DspFilter: return "FILT";
        case DspDist:   return "DIST";
        case DspDelay:  return "DLY";
        case DspRouting:return "ROUTE";
        case DspChorus: return "CHOR";
        default:        return "?";
    }
}

// Полный список режимов. Нужен параметру AudioParameterChoice (значение = индекс),
// поэтому порядок здесь = нумерация выше и меняться не должен.
inline const char* dspModeChoices() { return "mnm|old|dist2|fm2|bbox2"; }
inline constexpr int dspModeMnm   = 0;
inline constexpr int dspModeOld   = 1;
inline constexpr int dspModeDist2 = 2;
inline constexpr int dspModeFm2   = 3;
inline constexpr int dspModeBbox2 = 4;
inline constexpr int dspModeCount = 5;

// Версия схемы состояния: 8 -> 9 из-за перенумерации режимов.
inline constexpr int kDspModeSchemaVersion = 9;

inline const char* dspModeName(int mode) {
    switch (mode) {
        case 0: return "mnm";
        case 1: return "old";
        case 2: return "dist2";
        case 3: return "fm2";
        case 4: return "bbox2";
        default: return "?";
    }
}
inline const char* dspModeLabel(int mode) { return dspModeName(mode); }

inline int dspModeIndexByName(const char* name) {
    if (name == nullptr) return -1;
    for (int m = 0; m < dspModeCount; ++m)
        if (std::strcmp(dspModeName(m), name) == 0) return m;
    return -1;
}

// Перевод индексов режимов из состояния со схемой < 9 (до удаления cascade/dual/raw).
//   0 mnm -> mnm      1 old -> old
//   2 cascade, 3 dual, 4 raw -> mnm (режимы удалены)
//   5 dist2 -> dist2  6 fm2 -> fm2  7 bbox2 -> bbox2
inline int dspModeLegacyIndexToCurrent(int legacy) noexcept {
    switch (legacy) {
        case 0: return dspModeMnm;
        case 1: return dspModeOld;
        case 2: case 3: case 4: return dspModeMnm;   // cascade/dual/raw больше нет
        case 5: return dspModeDist2;
        case 6: return dspModeFm2;
        case 7: return dspModeBbox2;
        default: return dspModeMnm;
    }
}

// false -> в меню остаются только основные (mnm) режимы, резервные полностью скрыты.
inline constexpr bool kShowBackupModes = true;

// -----------------------------------------------------------------------------
// Списки режимов ПО РАЗДЕЛАМ. Первым всегда "mnm" (основной режим раздела).
// Только эти списки решают, что можно включить в разделе: и меню, и аудио-поток
// читают их (раньше правила были размазаны по трём ad-hoc условиям).
// -----------------------------------------------------------------------------
inline const char* dspSectionModeChoices(int section) {
    switch (section) {
        case DspSynt:    return "mnm|old|fm2|bbox2";  // синт: FmCore прошивки, резерв -- Nova FM и FMPlus/BBOX
        case DspAmp:     return "mnm|old";            // амплитуда: kernel ADSR-таблицы
        case DspFilter:  return "mnm|old";            // фильтр: реальный дамп, резерв -- прежнее приближение
        case DspDist:    return "mnm|old|dist2";      // дист: ALU-сатурация прошивки, резерв -- DistortionSaturationBlock
        case DspDelay:   return "mnm|old";            // дилей: m32 FX-DLY
        case DspRouting: return "mnm";                // цепочка проверена и она одна
        case DspChorus:  return "mnm";                // хорус: единственная реализация
        default:         return "mnm";
    }
}

inline int dspSectionModeCount(int section) {
    const char* list = dspSectionModeChoices(section);
    int n = 0;
    for (const char* p = list; *p != 0; ++p) if (*p == '|') ++n;
    return n + 1;
}

// i-й режим раздела (0 = основной mnm). Если индекса нет -- возвращает mnm.
inline int dspSectionModeAt(int section, int index) {
    const char* list = dspSectionModeChoices(section);
    int i = 0;
    const char* start = list;
    for (const char* p = list; ; ++p) {
        if (*p == '|' || *p == 0) {
            if (i == index) {
                std::array<char, 16> token{};
                const std::size_t len = static_cast<std::size_t>(p - start);
                const std::size_t n = len < token.size() - 1 ? len : token.size() - 1;
                for (std::size_t k = 0; k < n; ++k) token[k] = start[k];
                const int mode = dspModeIndexByName(token.data());
                return mode < 0 ? dspModeMnm : mode;
            }
            ++i;
            if (*p == 0) break;
            start = p + 1;
        }
    }
    return dspModeMnm;
}

// Единственное правило допустимости: режим должен быть в списке своего раздела.
inline bool dspModeAllowedForSection(int section, int mode) {
    const int n = dspSectionModeCount(section);
    for (int i = 0; i < n; ++i) if (dspSectionModeAt(section, i) == mode) return true;
    return false;
}

// Основной режим раздела -- всегда mnm и всегда доступен.
inline constexpr int dspModePrimaryForSection(int) { return dspModeMnm; }

// Совместимость: прежние вспомогательные предикаты (оставлены для внешнего кода,
// сами правила теперь целиком описаны списками выше).
inline constexpr int dspModeFilter = DspFilter;
inline bool dspModeIsDistOnly(int mode) noexcept { return mode == dspModeDist2; }
inline bool dspModeIsSyntOnly(int mode) noexcept { return mode == dspModeFm2 || mode == dspModeBbox2; }

// -----------------------------------------------------------------------------
// Тексты (меню/тултипы). Константы резонанса -- реальные из прошивки:
// $F528BD = -0.084747577, $4A4DF0 = +0.581161499.
// -----------------------------------------------------------------------------
inline const char* dspModeProvenance(int section) {
    switch (section) {
        case DspSynt:   return "SYNT: mnm=recovered OS1.32B FmCore (P:$4A8-$4F5, tone LP P:$144AC7). Резерв: old=Nova FM, fm2=FMPlusBlocks (ratio tbl P:$141A80), bbox2=Nova_2KnobFade retrig";
        case DspAmp:    return "AMP: kernel P:$4A8-$4F5 ADSR FSM, таблицы Y:$141800/Y:$141880, LFO $791FD0=0.94628";
        case DspFilter: return "FILT: mnm=РЕАЛЬНЫЙ фильтр из mnm_filter_full_dump_juce_real (kLP 258 coeffs P:$144AC7, SVF $F528BD=-0.0847476 $4A4DF0=+0.5811615, AD env BOFS/WOFS). Резерв: old=прежнее приближение";
        case DspDist:   return "DIST: mnm=ALU sat bset #$14,sr hardclip +-1.0 P:$1476A5. Резерв: old=tanh, dist2=DistortionSaturationBlock + SRR";
        case DspDelay:  return "DLY: mnm=m32 FX-DLY X:$114000 + P:$144C49 recip. Резерв: old=линейная интерполяция";
        case DspRouting:return "ROUTE: mnm=проверенная цепочка synth->dist->srr->filt+eq->env->dsndEQ (routing-100/exact/verified)";
        case DspChorus: return "CHOR: mnm=OS1.32B native core 24-bit Q23, 16-frame schedule, unity dry";
        default:        return "";
    }
}

inline const char* dspModeProvenanceForMode(int section, int mode) {
    if (mode == dspModeMnm) {
        switch (section) {
            case DspSynt:   return "SYNT mnm (основной): recovered OS1.32B FmCore (P:$4A8-$4F5, tone LP P:$144AC7)";
            case DspAmp:    return "AMP mnm (основной): kernel ADSR FSM Y:$141800/Y:$141880";
            case DspFilter: return "FILT mnm (ОСНОВНОЙ): реальный дамп -- kLP 258 coeffs + SVF $F528BD=-0.0847476 $4A4DF0=+0.5811615, Q 0.5..16 (порт), AD-огибающая BOFS/WOFS, при дефолте unity THRU";
            case DspDist:   return "DIST mnm (основной): ALU sat bset #$14,sr hardclip +-1.0";
            case DspDelay:  return "DLY mnm (основной): m32 FX-DLY X:$114000 + recip P:$144C49";
            case DspRouting:return "ROUTE mnm (основной): synth->dist->srr->filt+eq->env->dsndEQ (verified)";
            case DspChorus: return "CHOR mnm (основной): native OS1.32B core 24-bit, unity dry";
            default:        return "mnm (основной): recovered firmware";
        }
    }
    if (mode == dspModeOld) return "old (резерв): прежнее приближение Nova (фильтр -- biquad, дилей -- линейная интерполяция)";
    if (mode == dspModeDist2) return "dist2 (резерв DIST): DistortionSaturationBlock HardClip_SRSM/Overdrive/Fold + SRR";
    if (mode == dspModeFm2) return "fm2 (резерв SYNT): FMPlusBlocks DYN/STAT/PAR + таблица отношений";
    if (mode == dspModeBbox2) return "bbox2 (резерв SYNT): DproBboxBlock Nova_2KnobFade, repeat-sample, hold = repeat*4";
    return "?";
}

}  // namespace monomachine
