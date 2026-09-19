#pragma once
// DSP mode registry -- Monomachine Nova 1.6.5 (ревизия v7)
//
// ЧТО ИЗМЕНИЛОСЬ В v7 (по жалобам пользователя на 1.6.4):
//   * Раздел ROUTE убран из меню DSP MODE: проверенная цепочка одна
//     (synth->dist->srr->filt+eq->env->dsnd), переключатель ничего не менял
//     на слух («если так можно просто удалить mnm роутинг»). Параметр mode_route
//     оставлен в состоянии для совместимости, но в UI не показывается;
//     вместо него -- подробное описание цепочки при наведении (dspRouteDescription).
//   * У FM+STAT удалён резервный old-движок (пользователь разрешил): машина 8
//     всегда использует mnm-ядро; FM-машины разведены по отдельным файлам
//     (monomachine_fm_stat.hpp / monomachine_fm_par.hpp / monomachine_fm_dynamic.hpp).
//   * Тексты меню/тултипов переписаны: категории не перепутаны, каждая строка
//     начинается с имени своего раздела, без лишних подчёркиваний.
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
// Категории (папки) меню DSP MODE: чтобы было видно, к чему относится каждый
// раздел и какие машины/ручки он обслуживает (просьба пользователя:
// «дсп должны быть отсортированы по папкам, чтобы понимать зависимость»).
// -----------------------------------------------------------------------------
inline const char* dspSectionCategory(int section) {
    switch (section) {
        case DspSynt:    return "МАШИНЫ (OSC)";   // FM+ STAT/PAR/DYN и остальные осцилляторы
        case DspAmp:     return "АМПЛИТУДА";      // ручка DIST/VOL страницы AMP + огибающая
        case DspFilter:  return "ФИЛЬТР";         // страница FILT: BASE/WDTH/HPQ/LPQ/ATK/DEC/BOFS/WOFS
        case DspDist:    return "ДИСТОРШН/SRR";   // ручка DIST (страница AMP) + SRR (страница EFFX)
        case DspDelay:   return "ДИЛЕЙ";          // страница EFFX: DTIM/DSND/DFB/DBAS/DWID
        case DspRouting: return "ЦЕПЬ";           // порядок блоков (один, проверенный)
        case DspChorus:  return "ХОРУС";          // машина FX-CHORUS (15)
        default:         return "";
    }
}

// 1.6.5: ROUTE больше не выбирается в меню -- цепочка одна и проверена.
// Раздел остаётся в состоянии (mode_route) только для совместимости пресетов.
inline bool dspSectionVisibleInMenu(int section) { return section != DspRouting; }

// -----------------------------------------------------------------------------
// Тексты (меню/тултипы). Каждая строка начинается с имени СВОЕГО раздела,
// чтобы категории больше не могли «перепутаться». Константы резонанса --
// реальные из прошивки: $F528BD = -0.084747577, $4A4DF0 = +0.581161499.
// -----------------------------------------------------------------------------
inline const char* dspModeProvenance(int section) {
    switch (section) {
        case DspSynt:   return "SYNT: mnm = ядро FM+ из прошивки OS 1.32B (P:$4A8-$4F5, тон LP P:$144AC7). old = прежний Nova FM (у FM+STAT резерва нет, машина 8 всегда на mnm)";
        case DspAmp:    return "AMP: mnm = огибающая-автомат из прошивки, таблицы Y:$141800 (атака) и Y:$141880 (декей/релиз). old = прежняя аппроксимация Nova с кривыми";
        case DspFilter: return "FILT: mnm = настоящий фильтр из дампа прошивки (258 коэфф. P:$144AC7, SVF $F528BD/$4A4DF0, огибающая BOFS/WOFS, при дефолте прозрачен). old = прежний biquad";
        case DspDist:   return "DIST: mnm = насыщение как в прошивке (жёсткий клип +-1.0, 64 = прозрачно). old = прежний tanh/bipolar";
        case DspDelay:  return "DLY: mnm = линия m32 FX-DLY (буфер X:$114000, таблица времён P:$144C49), wet отдельным сигналом, DSND=0 прозрачно. old = линейная интерполяция, DSND влево = пинг-понг, вправо = стерео";
        case DspRouting:return "ROUTE: цепочка одна и проверена: synth->dist->srr->filt+eq->env->dsnd. Переключатель убран из меню в 1.6.5";
        case DspChorus: return "CHOR: mnm = нативное ядро OS 1.32B (24-bit), INP линейный, микс dry->wet по MIX";
        default:        return "";
    }
}

inline const char* dspModeProvenanceForMode(int section, int mode) {
    if (mode == dspModeMnm) {
        switch (section) {
            case DspSynt:    return "SYNT mnm (основной): ядро FM+ из прошивки OS 1.32B (P:$4A8-$4F5, тон LP P:$144AC7)";
            case DspAmp:     return "AMP mnm (основной): огибающая-автомат прошивки, таблицы Y:$141800/Y:$141880";
            case DspFilter:  return "FILT mnm (основной): настоящий дамп прошивки, 258 коэфф. + SVF $F528BD/$4A4DF0, Q 0.5..16, огибающая BOFS/WOFS, при дефолте прозрачен";
            case DspDist:    return "DIST mnm (основной): насыщение прошивки bset #$14,sr (жёсткий клип +-1.0)";
            case DspDelay:   return "DLY mnm (основной): линия m32 FX-DLY X:$114000 + P:$144C49, wet отдельно, при DSND=0 прозрачна";
            case DspRouting: return "ROUTE (единственный): synth->dist->srr->filt+eq->env->dsnd (проверено)";
            case DspChorus:  return "CHOR mnm (основной): нативное ядро OS 1.32B 24-bit, линейный INP, микс dry->wet";
            default:         return "mnm (основной): восстановленная прошивка";
        }
    }
    if (mode == dspModeOld) {
        switch (section) {
            case DspSynt:    return "SYNT old (резерв): прежний Nova FM. Внимание: у FM+STAT резерв удалён в 1.6.5, машина 8 остаётся на mnm";
            case DspAmp:     return "AMP old (резерв): прежняя огибающая Nova с кривыми ATK/DEC/REL";
            case DspFilter:  return "FILT old (резерв): прежний biquad-фильтр Nova";
            case DspDist:    return "DIST old (резерв): прежний tanh/bipolar дист";
            case DspDelay:   return "DLY old (резерв): линейная интерполяция; DSND влево = пинг-понг, вправо = стерео";
            case DspChorus:  return "CHOR old (резерв): не используется, ядро одно";
            default:         return "old (резерв): прежнее приближение Nova";
        }
    }
    return "?";
}

// -----------------------------------------------------------------------------
// 1.6.5: подробное описание цепочки для наведения на строчку роутинга
// (тултип кнопки DSP MODE и справка в меню). Объясняет, КАК каждый блок
// влияет на звук и почему при ручках «по умолчанию» FX не меняет источник.
// -----------------------------------------------------------------------------
inline const char* dspRouteDescription() {
    return
        "ЦЕПЬ (одна, проверена, как в прошивке OS 1.32B):\n"
        "1) SYNTH/машина -> источник звука (FM+, SWAVE, SID, FX-машины...).\n"
        "2) DIST: насыщение. Ручка DIST=64 полностью прозрачна, ниже 64 ослабляет драйв, выше добавляет перегруз.\n"
        "3) SRR: понижение частоты дискретизации (ручка SRR=0 выключена) -> зернистость при повышении.\n"
        "4) FILT+EQ: фильтр прошивки (BASE/WDTH/HPQ/LPQ + огибающая BOFS/WOFS), затем пик-EQ (EQF/EQG). При BASE=0/WDTH=127/Q=0 и EQG=0 прозрачен.\n"
        "5) ENV: амплитудная огибающая (ATK/HOLD/DEC/REL). В FX-версии при LATCH держится открытой, при MIDI gate следует за нотами.\n"
        "6) DSND: посыл в дилей ПОСЛЕ фильтра и огибающей. DSND=0 посыл выключен, выход = сухой сигнал без изменений.\n"
        "Итог: при DIST=64, SRR=0, фильтре/EQ по умолчанию и DSND=0 цепочка unity, то есть не меняет громкость и тембр источника.\n"
        "Порядок важен: дист до фильтра (перегруз режется фильтром), огибающая после фильтра (огибающая не модулирует срез), дилей в конце (повторы не проходят через дист).";
}

}  // namespace monomachine

