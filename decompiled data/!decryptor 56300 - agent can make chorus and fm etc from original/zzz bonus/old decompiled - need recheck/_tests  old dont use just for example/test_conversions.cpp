#include "../models/parameter_conversions.hpp"
#include <iostream>
#include <cassert>

int main() {
    std::cout << "Testing Monomachine Parameter Conversions...\n";

    // Pitch
    float c4 = monomachine::ParameterConversions::midiNoteToFrequency(60, 64);
    assert(std::abs(c4 - 261.63f) < 0.1f);

    // FM Ratios
    float r1 = monomachine::ParameterConversions::fm1FrqToRatio(16);
    assert(std::abs(r1 - 1.0f) < 0.001f);

    float r2 = monomachine::ParameterConversions::fm2FrqToRatio(42);
    assert(std::abs(r2 - 1.334f) < 0.05f);

    // LCD Formatters matching gui.jpg
    std::string lcd1Frq = monomachine::ParameterConversions::formatLcdValue("1FRQ", 16);
    std::string lcd2Frq = monomachine::ParameterConversions::formatLcdValue("2FRQ", 42);
    std::string lcdTrig = monomachine::ParameterConversions::formatLcdValue("TRIG", 0);
    std::string lcdWave = monomachine::ParameterConversions::formatLcdValue("WAVE", 0);
    std::string lcdMult = monomachine::ParameterConversions::formatLcdValue("MULT", 0);

    std::cout << "1FRQ LCD: " << lcd1Frq << "\n";
    std::cout << "2FRQ LCD: " << lcd2Frq << "\n";
    std::cout << "TRIG LCD: " << lcdTrig << "\n";
    std::cout << "WAVE LCD: " << lcdWave << "\n";
    std::cout << "MULT LCD: " << lcdMult << "\n";

    assert(lcd1Frq == "1.0");
    assert(lcd2Frq == "1.33");
    assert(lcdTrig == "FREE");
    assert(lcdWave == "TRI");
    assert(lcdMult == "1x");

    std::cout << "Parameter conversions verified successfully!\n";
    return 0;
}
