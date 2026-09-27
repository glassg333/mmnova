// Direct regression coverage for the four appended MOD ENV matrix sources.
#include "models/modulation_matrix.hpp"

#include <array>
#include <cmath>
#include <cstdio>
#include <cstdlib>

namespace
{
void require(bool condition, const char* message)
{
    if (! condition) {
        std::fprintf(stderr, "MODENVMATRIX FAIL: %s\n", message);
        std::exit(1);
    }
}

bool near(float a, float b)
{
    return std::abs(a - b) < 1.0e-5f;
}
}

int main()
{
    using namespace monomachine;
    static_assert(static_cast<int>(ModSource::ModEnv1) == 32);
    static_assert(static_cast<int>(ModSource::ModEnv4) == 35);
    static_assert(ModulationMatrix::kTargetCount == 248);

    ModulationMatrix matrix;
    matrix.reset();
    const float envs[]{0.125f, 0.5f, 0.75f, 1.25f}; // fourth checks source clamp.
    matrix.setModEnvOutputs(envs, 4);

    // Targets 48..51 avoid the historical default routes and verify each new
    // direct source preserves its appended ID and unipolar value.
    for (int i = 0; i < 4; ++i)
        matrix.configureRouting(static_cast<size_t>(10 + i), true,
                                static_cast<ModSource>(32 + i),
                                static_cast<uint8_t>(48 + i), 63,
                                ModPolarity::Unipolar);

    std::array<float, 56> base{}, result{};
    matrix.evaluate(base, result);
    require(near(result[48], 0.125f * 63.0f), "MOD ENV1 did not route to matrix target");
    require(near(result[49], 0.5f * 63.0f), "MOD ENV2 did not route to matrix target");
    require(near(result[50], 0.75f * 63.0f), "MOD ENV3 did not route to matrix target");
    require(near(result[51], 63.0f), "MOD ENV4 did not clamp/rout to matrix target");

    // AUX encoding is source+one. MOD ENV4 (ID 35) therefore needs aux=36.
    matrix.configureRouting(20, true, ModSource::ModEnv2, 52, 63,
                            ModPolarity::Unipolar, 36, 63);
    matrix.evaluate(base, result);
    require(near(result[52], 0.5f * 63.0f), "MOD ENV4 AUX source+one encoding is wrong");

    // Four appended destinations must land in their dedicated ARP/P-LOCK
    // boundary output pairs rather than aliases of an existing DSP target.
    matrix.reset();
    matrix.setModEnvOutputs(envs, 4);
    matrix.configureRouting(0, true, ModSource::ModEnv1, 244, 63, ModPolarity::Unipolar);
    matrix.configureRouting(1, true, ModSource::ModEnv2, 245, 63, ModPolarity::Unipolar);
    matrix.configureRouting(2, true, ModSource::ModEnv3, 246, 63, ModPolarity::Unipolar);
    matrix.configureRouting(3, true, ModSource::ModEnv4, 247, 63, ModPolarity::Unipolar);
    std::array<float, 2> arpWindow{}, plockWindow{};
    matrix.evaluate(base, result, nullptr, nullptr, nullptr, nullptr, static_cast<float*>(nullptr), static_cast<float*>(nullptr),
                    nullptr, nullptr, nullptr, nullptr, arpWindow.data(), plockWindow.data());
    require(near(arpWindow[0], 0.125f * 63.0f) && near(arpWindow[1], 0.5f * 63.0f),
            "ARP START/END matrix destinations did not evaluate");
    require(near(plockWindow[0], 0.75f * 63.0f) && near(plockWindow[1], 63.0f),
            "P-LOCK START/END matrix destinations did not evaluate");
    require(ModulationMatrix::getDestinationName(244) == "ARP START"
            && ModulationMatrix::getDestinationName(247) == "P-LOCK END",
            "appended matrix destination names are not stable");

    matrix.reset();
    matrix.evaluate(base, result);
    require(near(result[48], 0.0f) && near(result[49], 0.0f)
            && near(result[50], 0.0f) && near(result[51], 0.0f),
            "MOD ENV reset did not restore neutral source values");

    std::puts("MODENVMATRIX source IDs 32..35, ARP/P-LOCK windows 244..247, reset PASS");
    return 0;
}
