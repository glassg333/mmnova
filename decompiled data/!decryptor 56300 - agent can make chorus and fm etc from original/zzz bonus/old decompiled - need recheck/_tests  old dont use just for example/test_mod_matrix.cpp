#include "../models/modulation_matrix.hpp"
#include <iostream>
#include <cassert>

int main() {
    std::cout << "Testing Monomachine Modulation Matrix & Param X/Y...\n";

    monomachine::ModulationMatrix matrix;
    matrix.reset();

    std::array<uint8_t, 32> baseParams{};
    baseParams.fill(64); // All parameters initialized to 64
    baseParams[16] = 20; // Filter Base = 20
    baseParams[13] = 40; // Amp Vol = 40

    std::array<uint8_t, 32> modParams{};

    // 1. Test Key Tracking modulation into Filter Base
    // Slot 0 has KeyTracking -> Filter Base (depth 32)
    matrix.setNoteAndVelocity(72, 100); // 1 octave above C4
    matrix.evaluate(baseParams, modParams);

    std::cout << "Original Filter Base: " << (int)baseParams[16] << ", Modulated: " << (int)modParams[16] << "\n";
    assert(modParams[16] > baseParams[16] && "Higher key should increase Filter Base via key tracking");

    // 2. Test Velocity modulation into Amp Volume
    // Slot 1 has Velocity -> Amp Vol (depth 32)
    matrix.setNoteAndVelocity(60, 127); // Maximum velocity
    matrix.evaluate(baseParams, modParams);
    std::cout << "Original Amp Vol: " << (int)baseParams[13] << ", Modulated: " << (int)modParams[13] << "\n";
    assert(modParams[13] > baseParams[13] && "Velocity should boost Amp Volume");

    // 3. Test Free X Parameter (Joystick X / Mod Wheel / Macro X)
    matrix.setParamX(127); // Full positive on X
    matrix.evaluate(baseParams, modParams);
    std::cout << "Original Filter Width: " << (int)baseParams[17] << ", Modulated with Param X: " << (int)modParams[17] << "\n";
    assert(modParams[17] > baseParams[17] && "Param X should modulate target");

    // 4. Test Free Y Parameter (Joystick Y / Aftertouch / Macro Y)
    matrix.setParamY(127); // Full positive on Y
    matrix.evaluate(baseParams, modParams);
    std::cout << "Original Synth Param C: " << (int)baseParams[2] << ", Modulated with Param Y: " << (int)modParams[2] << "\n";
    assert(modParams[2] > baseParams[2] && "Param Y should modulate target");

    // Destination names test
    std::string dName = monomachine::ModulationMatrix::getDestinationName(16);
    assert(dName == "FLTR_BASE");
    std::cout << "Destination name for param 16: " << dName << "\n";

    std::cout << "Modulation Matrix and X/Y parameters verified successfully!\n";
    return 0;
}
