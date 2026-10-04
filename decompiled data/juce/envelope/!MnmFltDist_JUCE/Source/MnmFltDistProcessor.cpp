// MnmFltDistProcessor.cpp — точка входа плагина (mnm_16, обвязка переписана)
#include "MnmFltDistProcessor.h"

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MnmFltDistProcessor();
}
