#pragma once
// DSP mode registry -- Monomachine Nova 1.6.3 (ревизия v6)
// -----------------------------------------------------------------------------
// ГЛАВНОЕ ПРАВИЛО: в каждом разделе свой короткий список режимов,
// первым всегда основной "mnm".
//
// ЧТО ИЗМЕНИЛОСЬ В v6
//   * Альтернативные движки dist2 / fm2 / bbox2 УДАЛЕНЫ вместе с кодом
//     (MnmDist2.hpp, MnmFm2.hpp, MnmBbox2.hpp выведены из сборки в *.bak-v6).
//     Осталось ровно два режима: mnm (основной) и old (прежнее приближение Nova).
//   * Новые режимы теперь есть ТОЛЬКО у фильтра: FILT mnm = реальный фильтр из дампа
//     прошивки (MnmRealFilter.hpp, таблицы kLP 258 коэффициентов, SVF $F528BD/$4A4DF0).
//     Все остальные разделы -- как в 1.6.0: их прежние алгоритмы получили режим old.
//   * Схема состояния 9 -> 10: старые индексы режимов (2/3/4 для новых и 5/6/7 для
//     прежних нумераций) сводятся к основному mnm, см. dspModeLegacyIndexToCurrent.
//
// Числа режимов:
// 0 mnm -- ОСНОВНОЙ: реальный фильтр из прошивки (FILT), ядра прошивки для AMP/DIST/DLY/SYNT
// 1 old -- РЕЗЕРВ: прежние приближения Nova (biquad-фильтр, tanh-дист, линейный дилей...)

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
inline const char* dspModeChoices() { return "mnm|old"; }
inline constexpr int dspModeMnm = 0;
inline constexpr int dspModeOld = 1;
inline constexpr int dspModeCount = 2;

// Версия схемы состояния: 9 -> 10 из-за удаления режимов dist2/fm2/bbox2.
inline constexpr int kDspModeSchemaVersion = 10;

inline const char* dspModeName(int mode) {
    switch (mode) {
        case 0: return "mnm";
        case 1: return "old";
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

// Перевод индексов режимов из состояний с прежними схемами (8 и меньше):
//   0 mnm -> mnm, 1 old -> old,
//   2/3/4 (cascade/dual/raw из старого патча) и 5/6/7 (dist2/fm2/bbox2) -> mnm.
// Так ни одно сохранённое состояние не остаётся с несуществующим режимом.
inline int dspModeLegacyIndexToCurrent(int legacy) noexcept {
    switch (legacy) {
        case 0: return dspModeMnm;
        case 1: return dspModeOld;
        default: return dspModeMnm;
    }
}

// false -> в меню остаются только основные (mnm) режимы, резерв полностью скрыт.
inline constexpr bool kShowBackupModes = true;

// -----------------------------------------------------------------------------
// Списки режимов ПО РАЗДЕЛАМ. Первым всегда "mnm" (основной режим раздела).
// Только эти списки решают, что можно включить в разделе: и меню, и аудио-поток
// читают их.
// -----------------------------------------------------------------------------
inline const char* dspSectionModeChoices(int section) {
    switch (section) {
        case DspSynt:    return "mnm|old";   // синт: FmCore прошивки, резерв -- прежний Nova FM
        case DspAmp:     return "mnm|old";   // амплитуда: kernel ADSR-таблицы
        case DspFilter:  return "mnm|old";   // фильтр: РЕАЛЬНЫЙ дамп прошивки, резерв -- прежнее приближение
        case DspDist:    return "mnm|old";   // дист: ALU-сатурация прошивки, резерв -- прежний bipolarDist
        case DspDelay:   return "mnm|old";   // дилей: m32 FX-DLY
        case DspRouting: return "mnm";       // цепочка проверена и она одна
        case DspChorus:  return "mnm";       // хорус: единственная реализация (ядро OS 1.32B)
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

// Совместимость с прежним кодом.
inline constexpr int dspModeFilter = DspFilter;

// -----------------------------------------------------------------------------
// Тексты (меню/тултипы). Константы резонанса -- реальные из прошивки:
// $F528BD = -0.084747577, $4A4DF0 = +0.581161499.
// -----------------------------------------------------------------------------
inline const char* dspModeProvenance(int section) {
    switch (section) {
        case DspSynt:   return "SYNT: mnm=восстановленное ядро OS1.32B (P:$4A8-$4F5, tone LP P:$144AC7). Резерв: old=прежний Nova FM";
        case DspAmp:    return "AMP: kernel P:$4A8-$4F5 ADSR FSM, таблицы Y:$141800/Y:$141880, LFO $791FD0=0.94628";
        case DspFilter: return "FILT: mnm=РЕАЛЬНЫЙ фильтр из дампа прошивки (kLP 258 коэффициентов P:$144AC7, SVF $F528BD=-0.0847476 $4A4DF0=+0.5811615, AD-огибающая BOFS/WOFS). Резерв: old=прежнее приближение";
        case DspDist:   return "DIST: mnm=ALU-сатурация прошивки bset #$14,sr (hardclip +-1.0). Резерв: old=прежний bipolarDist";
        case DspDelay:  return "DLY: mnm=m32 FX-DLY X:$114000 + P:$144C49. Резерв: old=линейная интерполяция";
        case DspRouting:return "ROUTE: mnm=проверенная цепочка synth->dist->srr->filt+eq->env->dsnd";
        case DspChorus: return "CHOR: mnm=нативное ядро OS1.32B 24-bit (как в 1.6.0), INP линейный, микс dry->wet по MIX";
        default:        return "";
    }
}

inline const char* dspModeProvenanceForMode(int section, int mode) {
    if (mode == dspModeMnm) {
        switch (section) {
            case DspSynt:   return "SYNT mnm (основной): восстановленное ядро OS1.32B (P:$4A8-$4F5, tone LP P:$144AC7)";
            case DspAmp:    return "AMP mnm (основной): kernel ADSR FSM Y:$141800/Y:$141880";
            case DspFilter: return "FILT mnm (ОСНОВНОЙ): реальный дамп -- kLP 258 коэффициентов + SVF $F528BD=-0.0847476 $4A4DF0=+0.5811615, Q 0.5..16, AD-огибающая BOFS/WOFS, при дефолте unity THRU";
            case DspDist:   return "DIST mnm (основной): ALU-сатурация прошивки bset #$14,sr";
            case DspDelay:  return "DLY mnm (основной): m32 FX-DLY X:$114000 + P:$144C49";
            case DspRouting:return "ROUTE mnm (основной): synth->dist->srr->filt+eq->env->dsnd (проверено)";
            case DspChorus: return "CHOR mnm (основной): нативное ядро OS1.32B 24-bit, линейный INP, микс dry->wet";
            default:        return "mnm (основной): восстановленная прошивка";
        }
    }
    if (mode == dspModeOld) return "old (резерв): прежнее приближение Nova (фильтр -- biquad, дист -- tanh/bipolar, дилей -- линейная интерполяция)";
    return "?";
}

}  // namespace monomachine
