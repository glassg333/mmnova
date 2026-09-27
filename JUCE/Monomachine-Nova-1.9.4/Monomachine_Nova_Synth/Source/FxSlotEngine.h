#pragma once
#include <array>
#include <memory>
#include "NovaDSP.h"

namespace nova {
// ===== 1.8.4: одно ленивое ядро пользовательского FX-слота =================
// Это намеренно НЕ новая реализация эффектов. Каждый занятый слот держит один
// родной MachineEngine и вызывает его render(..., false), как P2. Поэтому
// FX-THRU / REVERB / CHORUS / DYNAMIX / RINGMOD / PHASER / FLANGER проходят
// через тот же код, параметры и порядок, что в оригинальном FX-списке.
//
// «Ленивый»: пока machineId==0, core == nullptr. Нулевой слот не выделяет
// MachineEngine и process() возвращается до любой DSP-работы.
class FxSlotEngine {
public:
    void prepare(double sampleRate) noexcept {
        sr = sampleRate > 0.0 ? sampleRate : 44100.0;
        if (core != nullptr) {
            core->prepare(sr);
            core->set(machineId, params);
        }
    }

    // Вызывается аудиопотоком только после атомарного снимка управления слотом.
    // p8 всегда указывает на восемь нормальных панельных значений 0..127.
    void setState(int newMachineId, bool newOn, const std::array<float, 8>& p8) noexcept {
        const bool machineChanged = newMachineId != machineId;
        params = p8;

        if (newMachineId == 0) {
            machineId = 0;
            on = false;
            core.reset(); // пустой слот снова не держит ядро / буферы
            return;
        }

        if (machineChanged)
            core.reset(); // не оставляем старую машину в памяти «на всякий случай»

        machineId = newMachineId;
        on = newOn;

        // Выключенный, но выбранный слот не обязан иметь ядро до первого ON.
        if (!on)
            return;

        if (core == nullptr) {
            core = std::make_unique<MachineEngine>();
            core->prepare(sr);
        }
        core->set(machineId, params);
    }

    void process(float* l, float* r, int n) noexcept {
        if (machineId == 0 || !on || core == nullptr || n <= 0)
            return;
        core->render(l, r, n, false); // точный родной FX-путь MachineEngine
    }

    // PANIC/CLEAR TAILS preserves the selected slot and its controls, but must
    // not leave a CHORUS/reverb/etc. core carrying audio into the next pass.
    void clear() noexcept { if (core != nullptr) core->clear(); }

    int latencyFrames() const noexcept {
        return (machineId == 15 && on && core != nullptr) ? core->fxLatency() : 0;
    }

    int id() const noexcept { return machineId; }
    bool enabled() const noexcept { return on; }

private:
    double sr = 44100.0;
    int machineId = 0;
    bool on = false;
    std::array<float, 8> params{{64,64,64,64,64,64,64,64}};
    std::unique_ptr<MachineEngine> core;
};
} // namespace nova
