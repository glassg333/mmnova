// Тесты основного фильтра (режим "mnm" = RealFilterCore из MnmRealFilter.hpp), ревизия v4.
//
// Проверяют ровно то, на что жаловались в баг-репорте:
//   1. THRU: BASE=0, WIDTH=127, Q=0 -> выход бит-в-бит равен входу (unity для FX)
//   2. LP-стадия совпадает с эталоном демо дампа (TEST 1, TONE=64) и с таблицей
//   3. HP-стадия (BASE=64) совпадает с эталоном демо дампа (TEST 2)
//   4. ПРОВОДКА РУЧЕК: ATK/DEC управляют временем огибающей, BOFS/WOFS -- глубиной
//      смещения среза (раньше они были перепутаны -- это баг "по частотам")
//   5. РЕЗОНАНС: Q=0 -> стадии нет; монотонный рост; подъём на срезе; затухание
//   6. УСТОЙЧИВОСТЬ: полный перебор параметров -- ни NaN, ни разноса
//   7. DC: при активном фильтре постоянная составляющая давится (дефолт THRU -- unity)
//   8. Отпускание ноты: огибающая возвращается к 0, THRU не «зависает»
//   9. Каналы независимы (состояние L не течёт в R)
//
// Эталонные значения амплитуд посчитаны аналитически по самой таблице прошивки
// (kLP_filter_coeffs) и совпадают с demo_output_sample.txt дампа.
// Ожидания по таблице берутся ИЗ ТАБЛИЦЫ, а не из магических чисел.
//
// Собирается без JUCE:
//   g++ -std=c++17 -O2 -I Source tests/MnmRealFilterTests.cpp -o build/MnmRealFilterTests

#include "dsp/mnm/MnmKernel.hpp"
#include "dsp/mnm/MnmRealFilter.hpp"

#include <cmath>
#include <cstdio>
#include <string>
#include <vector>

namespace {

int failures = 0;

// Проверка "измеренное ~ ожидаемое с допуском".
void check(double measured, const std::string& what, double expected, double tol) {
    const bool pass = std::fabs(measured - expected) <= tol;
    if (!pass) ++failures;
    std::printf("  %s %-58s изм=%9.3f ожид=%9.3f допуск=%.3f\n",
                pass ? "OK  " : "FAIL", what.c_str(), measured, expected, tol);
}

void checkTrue(bool ok, const std::string& what) {
    if (!ok) ++failures;
    std::printf("  %s %s\n", ok ? "OK  " : "FAIL", what.c_str());
}

// Коэффициент прямо из таблицы прошивки.
double tableCoef(int tone) {
    return static_cast<double>(::mnm::kLP_filter_coeffs[tone]) / 8388608.0;
}

std::vector<float> makeSine(double hz, int n, float amp = 0.5f) {
    std::vector<float> v(static_cast<size_t>(n));
    for (int i = 0; i < n; ++i)
        v[static_cast<size_t>(i)] = static_cast<float>(amp * std::sin(2.0 * 3.14159265358979 * hz * i / 44100.0));
    return v;
}

// Прирост/спад в дБ на частоте hz для текущих параметров фильтра.
double gainDb(monomachine::mnm::FilterCore& f, double hz, bool doReset = true, float amp = 0.5f) {
    const int n = 44100;
    auto sig = makeSine(hz, n, amp);
    if (doReset) f.reset();
    double sum = 0.0;
    for (int i = 0; i < n; ++i) {
        const float y = f.process(0, sig[static_cast<size_t>(i)]);
        if (i >= n * 3 / 4) sum += static_cast<double>(y) * y;
    }
    const double outRms = std::sqrt(sum / (n / 4));
    const double inRms = amp / std::sqrt(2.0);
    return 20.0 * std::log10(outRms / inRms);
}

void testThru() {
    std::printf("\nTEST 1: THRU при дефолте (BASE=0 WIDTH=127 Q=0) -- unity для FX\n");
    monomachine::mnm::FilterCore f;
    f.setSampleRate(44100.0);
    f.setParameters(0.0f, 127.0f, 0.0f, 0.0f, 0.0f, 64.0f, 0.0f, 0.0f);
    checkTrue(f.bypassed(), "фильтр в режиме THRU (bypassed)");
    double maxErr = 0.0;
    for (int i = 0; i < 4096; ++i) {
        const float x = static_cast<float>(0.7 * std::sin(2.0 * 3.14159265358979 * 440.0 * i / 44100.0));
        const float l = f.process(0, x);
        const float r = f.process(1, x * 0.5f);
        maxErr = std::max(maxErr, static_cast<double>(std::fabs(l - x)));
        maxErr = std::max(maxErr, static_cast<double>(std::fabs(r - x * 0.5f)));
    }
    check(maxErr, "оба канала бит-в-бит равны входу", 0.0, 0.0);
}

void testLowpassReference() {
    std::printf("\nTEST 2: LP-стадия по реальной таблице (BASE=0, WIDTH=64) -- эталон демо дампа\n");
    monomachine::mnm::FilterCore f;
    f.setSampleRate(44100.0);
    f.setParameters(0.0f, 64.0f, 0.0f, 0.0f, 0.0f, 64.0f, 0.0f, 0.0f);
    check(static_cast<double>(f.lpCoef()), "коэффициент = kLP_filter_coeffs[64]/2^23",
          tableCoef(64), 1e-6);
    check(gainDb(f, 500.0), "усиление 500 Гц", -1.81, 0.15);
    check(gainDb(f, 1000.0), "усиление 1 кГц", -4.87, 0.15);
    check(gainDb(f, 5000.0), "усиление 5 кГц", -17.05, 0.2);
    check(gainDb(f, 10000.0), "усиление 10 кГц", -22.44, 0.25);
}

void testHighpassReference() {
    std::printf("\nTEST 3: HP-стадия по реальной таблице (BASE=64, WIDTH=127)\n");
    monomachine::mnm::FilterCore f;
    f.setSampleRate(44100.0);
    f.setParameters(64.0f, 127.0f, 0.0f, 0.0f, 0.0f, 64.0f, 0.0f, 0.0f);
    check(static_cast<double>(f.hpCoef()), "коэффициент = kLP_filter_coeffs[64]/2^23",
          tableCoef(64), 1e-6);
    check(gainDb(f, 100.0), "усиление 100 Гц", -17.36, 0.25);
    check(gainDb(f, 500.0), "усиление 500 Гц", -5.10, 0.2);
    check(gainDb(f, 1000.0), "усиление 1 кГц", -2.15, 0.2);
    check(gainDb(f, 5000.0), "усиление 5 кГц", -0.74, 0.2);
    check(gainDb(f, 10000.0), "усиление 10 кГц", -1.20, 0.2);
}

// Основной тест на перепутанную проводку: ATK/DEC меняют ВРЕМЯ, BOFS/WOFS -- ГЛУБИНУ.
void testKnobWiring() {
    std::printf("\nTEST 4: проводка ручек (ATK/DEC = время, BOFS/WOFS = глубина)\n");
    // (a) BOFS=0: любое ATK не должно двигать срез
    {
        monomachine::mnm::FilterCore a; a.setSampleRate(44100.0);
        a.setParameters(64.0f, 32.0f, 0.0f, 0.0f, 0.0f, 64.0f, 0.0f, 0.0f);   // atk=0, bofs=0
        const float ref = a.hpCoef();
        a.trigger();
        for (int i = 0; i < 44100; ++i) { a.process(0, 0.1f); a.process(1, 0.1f); }
        check(static_cast<double>(a.hpCoef()), "BOFS=0: срез не зависит от огибающей",
              static_cast<double>(ref), 1e-7);
        checkTrue(a.envelopeValue() > 0.99f, "огибающая дошла до 1.0 (ATK=0 -- мгновенно)");
    }
    // (b) BOFS=+32: срез обязан сместиться вверх вместе с огибающей (BASE 64 -> 96)
    {
        monomachine::mnm::FilterCore b; b.setSampleRate(44100.0);
        b.setParameters(64.0f, 32.0f, 0.0f, 0.0f, 0.0f, 64.0f, 32.0f, 0.0f);
        const float before = b.hpCoef();
        b.trigger();
        for (int i = 0; i < 44100; ++i) { b.process(0, 0.1f); b.process(1, 0.1f); }
        check(static_cast<double>(b.hpCoef()), "BOFS=+32: срез ушёл на таблицу [96]",
              tableCoef(96), 1e-6);
        checkTrue(b.hpCoef() > before + 0.05f, "срез вырос относительно положения без огибающей");
    }
    // (c) ATK управляет скоростью: 127 -> за секунду едва сдвинулось, 0 -> сразу
    {
        monomachine::mnm::FilterCore slow, fast;
        slow.setSampleRate(44100.0); fast.setSampleRate(44100.0);
        slow.setParameters(64.0f, 32.0f, 0.0f, 0.0f, 127.0f, 64.0f, 32.0f, 0.0f);
        fast.setParameters(64.0f, 32.0f, 0.0f, 0.0f, 0.0f,   64.0f, 32.0f, 0.0f);
        slow.trigger(); fast.trigger();
        for (int i = 0; i < 44100; ++i) { slow.process(0, 0.1f); fast.process(0, 0.1f); }
        checkTrue(slow.envelopeValue() < 0.05f && fast.envelopeValue() > 0.99f,
                  "ATK=127 -- медленно (за 1 с ~3%, полный подъём ~30 с), ATK=0 -- мгновенно");
    }
    // (d) DEC управляет скоростью спада после отпускания
    {
        monomachine::mnm::FilterCore d0, d127;
        d0.setSampleRate(44100.0); d127.setSampleRate(44100.0);
        d0.setParameters(64.0f, 32.0f, 0.0f, 0.0f, 0.0f, 0.0f, 32.0f, 0.0f); d0.trigger();
        for (int i = 0; i < 8000; ++i) d0.process(0, 0.1f);
        d127.setParameters(64.0f, 32.0f, 0.0f, 0.0f, 0.0f, 127.0f, 32.0f, 0.0f); d127.trigger();
        for (int i = 0; i < 8000; ++i) d127.process(0, 0.1f);
        d0.release(); d127.release();
        for (int i = 0; i < 44100; ++i) { d0.process(0, 0.1f); d127.process(0, 0.1f); }
        checkTrue(d0.envelopeValue() < 1e-3f && d127.envelopeValue() > 0.5f,
                  "DEC=0 -- спад быстрый, DEC=127 -- медленный");
    }
}

void testResonance() {
    std::printf("\nTEST 5: резонанс Q (0.5 .. 16, TPT-SVF -- устойчив)\n");
    const float base = 0.0f, width = 64.0f;   // срез LP по таблице ~695 Гц
    monomachine::mnm::FilterCore q0, q32, q64, q127, hpRes;
    for (auto* f : {&q0, &q32, &q64, &q127, &hpRes}) f->setSampleRate(44100.0);
    q0.setParameters(base, width, 0.0f, 0.0f, 0.0f, 64.0f, 0.0f, 0.0f);
    q32.setParameters(base, width, 0.0f, 32.0f, 0.0f, 64.0f, 0.0f, 0.0f);
    q64.setParameters(base, width, 0.0f, 64.0f, 0.0f, 64.0f, 0.0f, 0.0f);
    q127.setParameters(base, width, 0.0f, 127.0f, 0.0f, 64.0f, 0.0f, 0.0f);
    checkTrue(!q0.resonanceActive(), "Q=0 -> резонансной стадии нет вообще");
    checkTrue(q127.resonanceActive(), "Q=127 -> резонансная стадия включена");

    auto peak = [](monomachine::mnm::FilterCore& f) {
        double best = -200.0;
        for (double hz = 300.0; hz <= 1500.0; hz += 25.0) best = std::max(best, gainDb(f, hz, true, 0.2f));
        return best;
    };
    const double p0 = peak(q0), p32 = peak(q32), p64 = peak(q64), p127 = peak(q127);
    checkTrue(p32 > p0 + 1.0 && p64 > p32 + 1.0 && p127 > p64 + 1.0,
              "подъём на срезе растёт монотонно вместе с ручкой Q");
    check(p127 - p0, "подъём от Q=127 над Q=0, дБ", 24.0, 8.0);

    // Резонанс HP-секции (HPQ) -- подъём на частоте BASE
    monomachine::mnm::FilterCore hp0;
    hp0.setSampleRate(44100.0);
    hp0.setParameters(64.0f, 127.0f, 0.0f, 0.0f, 0.0f, 64.0f, 0.0f, 0.0f);
    hpRes.setParameters(64.0f, 127.0f, 127.0f, 0.0f, 0.0f, 64.0f, 0.0f, 0.0f);
    double h0 = -200.0, h1 = -200.0;
    for (double hz = 400.0; hz <= 1200.0; hz += 20.0) {
        h0 = std::max(h0, gainDb(hp0, hz, true, 0.2f));
        h1 = std::max(h1, gainDb(hpRes, hz, true, 0.2f));
    }
    check(h1 - h0, "подъём HPQ=127 на срезе BASE=64, дБ", 24.0, 8.0);

    // Автоколебания должны затухать: импульс, затем тишина.
    q127.reset();
    q127.process(0, 1.0f); q127.process(1, 1.0f);
    float tail = 0.0f;
    for (int i = 0; i < 44100 * 2; ++i) { const float y = q127.process(0, 0.0f); if (i > 44100) tail = std::max(tail, std::fabs(y)); }
    checkTrue(tail < 1e-3f, "после импульса колебание затухает (нет самовозбуждения)");
}

struct Sweep {
    long long combos = 0, badFinite = 0, badLevel = 0;
    float worst = 0.0f;
};

// Перебор всех комбинаций: ни NaN, ни разноса.
Sweep sweepStability() {
    const float bases[]   = {0.0f, 32.0f, 64.0f, 96.0f, 127.0f};
    const float widths[]  = {0.0f, 32.0f, 64.0f, 96.0f, 127.0f};
    const float qs[]      = {0.0f, 32.0f, 64.0f, 96.0f, 127.0f};
    const float times[]   = {0.0f, 64.0f, 127.0f};
    const float offsets[] = {-64.0f, 0.0f, 63.0f};
    Sweep s;
    monomachine::mnm::FilterCore f;
    f.setSampleRate(44100.0);
    const int n = 512;
    for (float base : bases) for (float width : widths) for (float hpq : qs) for (float lpq : qs)
    for (float atk : times) for (float dec : times) for (float bofs : offsets) for (float wofs : offsets) {
        f.setParameters(base, width, hpq, lpq, atk, dec, bofs, wofs);
        f.trigger();
        bool finite = true; float mx = 0.0f;
        for (int i = 0; i < n; ++i) {
            const float x = static_cast<float>(std::sin(2.0 * 3.14159265358979 * 220.0 * i / 44100.0)) * 0.9f;
            const float y = f.process(0, x);
            f.process(1, x);
            if (!std::isfinite(y)) { finite = false; break; }
            mx = std::max(mx, std::fabs(y));
        }
        ++s.combos;
        if (!finite) { ++s.badFinite; continue; }
        if (mx > monomachine::mnm::real_detail::kOutCeiling + 1e-3f) { ++s.badLevel; s.worst = std::max(s.worst, mx); }
    }
    return s;
}

// (a) Постоянная составляющая на ВХОДЕ: если в тракте есть HP-стадия (BASE > 0),
//     выход обязан уйти в ноль. THRU-дефолт сюда не попадает: он сознательно unity.
// (b) Нулевой по постоянной составляющей вход: фильтр не должен ГЕНЕРИРОВАТЬ смещение.
//     Прогон 2 с, чтобы успели затихнуть низкочастотные состояния (Q=16 при ~10 Гц
//     имеет постоянную времени ~0.5 с), среднее -- по последним 0.2 с.
struct DcReport { long long checked = 0; double worst = 0.0; };

DcReport dcFromConstantInput() {
    const float bases[]  = {32.0f, 64.0f, 96.0f, 127.0f};
    const float widths[] = {0.0f, 64.0f, 127.0f};
    const float qs[]     = {0.0f, 64.0f, 127.0f};
    DcReport r;
    monomachine::mnm::FilterCore f;
    f.setSampleRate(44100.0);
    for (float base : bases) for (float width : widths) for (float hpq : qs) for (float lpq : qs) {
        f.setParameters(base, width, hpq, lpq, 0.0f, 64.0f, 0.0f, 0.0f);
        f.trigger();
        float y = 0.0f;
        for (int i = 0; i < 88200; ++i) y = f.process(0, 1.0f);   // 2 с на входе +1.0
        if (f.bypassed()) continue;
        ++r.checked;
        r.worst = std::max(r.worst, static_cast<double>(std::fabs(y)));
    }
    return r;
}

DcReport dcFromZeroMeanInput() {
    const float bases[]   = {0.0f, 64.0f, 127.0f};
    const float widths[]  = {0.0f, 64.0f, 127.0f};
    const float qs[]      = {0.0f, 127.0f};
    const float offsets[] = {-64.0f, 0.0f, 63.0f};
    DcReport r;
    monomachine::mnm::FilterCore f;
    f.setSampleRate(44100.0);
    const int n = 88200;                       // 2 с
    for (float base : bases) for (float width : widths) for (float hpq : qs) for (float lpq : qs)
    for (float bofs : offsets) for (float wofs : offsets) {
        f.setParameters(base, width, hpq, lpq, 0.0f, 64.0f, bofs, wofs);
        f.trigger();
        double sum = 0.0;
        for (int i = 0; i < n; ++i) {
            const float x = static_cast<float>(0.9 * std::sin(2.0 * 3.14159265358979 * 220.5 * i / 44100.0));
            const float y = f.process(0, x);
            if (i >= n - 8820) sum += y;       // последние 0.2 с
        }
        ++r.checked;
        r.worst = std::max(r.worst, std::fabs(sum / 8820.0));
    }
    return r;
}

void testStabilityAndDc() {
    std::printf("\nTEST 6: перебор параметров -- нет разноса, нет NaN\n");
    const Sweep s = sweepStability();
    std::printf("  перебрано комбинаций: %lld\n", s.combos);
    checkTrue(s.badFinite == 0, "ни одного NaN/Inf за весь перебор");
    checkTrue(s.badLevel == 0, "выход ни разу не вышел за ограничитель (нет разноса)");
    std::printf("\nTEST 7: постоянная составляющая\n");
    const DcReport dcIn = dcFromConstantInput();
    std::printf("  (a) проверено состояний с HP-стадией: %lld\n", dcIn.checked);
    checkTrue(dcIn.checked > 0, "перебор действительно включал состояния с HP-стадией");
    check(dcIn.worst, "худший остаток +1.0 DC на выходе (BASE>0, дБ-шкала линейная)", 0.0, 0.01);
    const DcReport dcGen = dcFromZeroMeanInput();
    std::printf("  (b) проверено комбинаций: %lld\n", dcGen.checked);
    check(dcGen.worst, "худшее смещение, которое фильтр добавил сам", 0.0, 0.02);
}

void testRelease() {
    std::printf("\nTEST 8: отпускание ноты -- огибающая возвращается к 0\n");
    monomachine::mnm::FilterCore f;
    f.setSampleRate(44100.0);
    f.setParameters(64.0f, 64.0f, 0.0f, 0.0f, 0.0f, 0.0f, 60.0f, 0.0f);
    const float ref = f.hpCoef();
    f.trigger();
    for (int i = 0; i < 22050; ++i) { f.process(0, 0.1f); f.process(1, 0.1f); }
    checkTrue(f.envelopeValue() > 0.5f, "во время ноты огибающая поднята");
    f.release();
    for (int i = 0; i < 44100 * 3; ++i) { f.process(0, 0.1f); f.process(1, 0.1f); }
    check(static_cast<double>(f.envelopeValue()), "после отпускания огибающая = 0", 0.0, 1e-4);
    check(static_cast<double>(f.hpCoef()), "срез вернулся к положению без огибающей",
          static_cast<double>(ref), 1e-6);

    monomachine::mnm::FilterCore g;
    g.setSampleRate(44100.0);
    g.setParameters(0.0f, 127.0f, 0.0f, 0.0f, 0.0f, 64.0f, 0.0f, 0.0f);
    g.trigger();
    for (int i = 0; i < 22050; ++i) g.process(0, 0.1f);
    g.release();
    for (int i = 0; i < 44100 * 3; ++i) g.process(0, 0.1f);
    checkTrue(g.bypassed(), "после отпускания фильтр снова в THRU (в v3 модуляция зависала)");
}

void testChannels() {
    std::printf("\nTEST 9: независимость каналов\n");
    monomachine::mnm::FilterCore f;
    f.setSampleRate(44100.0);
    f.setParameters(64.0f, 32.0f, 100.0f, 100.0f, 0.0f, 64.0f, 0.0f, 0.0f);
    double maxRight = 0.0, maxLeft = 0.0;
    for (int i = 0; i < 8192; ++i) {
        const float x = (i == 0) ? 1.0f : 0.0f;
        const float l = f.process(0, x);
        const float r = f.process(1, 0.0f);
        maxLeft = std::max(maxLeft, static_cast<double>(std::fabs(l)));
        maxRight = std::max(maxRight, static_cast<double>(std::fabs(r)));
    }
    check(maxRight, "правый канал молчит при импульсе только в левом", 0.0, 1e-12);
    checkTrue(maxLeft > 0.1, "левый канал обработал импульс");
}

} // namespace

int main() {
    std::printf("=== Тесты основного фильтра (режим mnm), ревизия v4 ===\n");
    testThru();
    testLowpassReference();
    testHighpassReference();
    testKnobWiring();
    testResonance();
    testStabilityAndDc();
    testRelease();
    testChannels();
    if (failures == 0) {
        std::printf("\nВСЕ ТЕСТЫ РЕАЛЬНОГО ФИЛЬТРА ПРОЙДЕНЫ\n");
        return 0;
    }
    std::printf("\nПРОВАЛЕНО ПРОВЕРОК: %d\n", failures);
    return 1;
}
