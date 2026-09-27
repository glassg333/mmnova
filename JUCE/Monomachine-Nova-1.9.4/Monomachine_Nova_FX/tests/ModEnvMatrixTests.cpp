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

    matrix.reset();
    matrix.evaluate(base, result);
    require(near(result[48], 0.0f) && near(result[49], 0.0f)
            && near(result[50], 0.0f) && near(result[51], 0.0f),
            "MOD ENV reset did not restore neutral source values");

    std::puts("MODENVMATRIX source IDs 32..35, AUX 33..36, reset PASS");
    return 0;
}
