#pragma once
#include <JuceHeader.h>
#include "dsp/AudioAdapter.h"
#include <atomic>
#include <memory>

class MonomachineNovaAudioProcessor final : public juce::AudioProcessor {
public:
 MonomachineNovaAudioProcessor();
 ~MonomachineNovaAudioProcessor() override = default;
 void prepareToPlay(double,int) override;
 void releaseResources() override {}
 void processBlock(juce::AudioBuffer<float>&,juce::MidiBuffer&) override;
 void processBlockBypassed(juce::AudioBuffer<float>&,juce::MidiBuffer&) override;
 bool isBusesLayoutSupported(const BusesLayout&) const override;
 juce::AudioProcessorEditor* createEditor() override;
 bool hasEditor() const override { return true; }
 const juce::String getName() const override { return "MonomachineNova CHORUS"; }
 bool acceptsMidi() const override { return false; }
 bool producesMidi() const override { return false; }
 bool isMidiEffect() const override { return false; }
 double getTailLengthSeconds() const override { return 600.0; }
 int getNumPrograms() override { return 1; }
 int getCurrentProgram() override { return 0; }
 void setCurrentProgram(int) override {}
 const juce::String getProgramName(int) override { return "CHORUS"; }
 void changeProgramName(int,const juce::String&) override {}
 void getStateInformation(juce::MemoryBlock&) override;
 void setStateInformation(const void*,int) override;
 void initialisePatch();
 std::array<int,8> parameterValues() const noexcept;
 juce::AudioProcessorValueTreeState parameters;
 std::atomic<float> outputPeak{0.0f};
 inline static constexpr const char* parameterIds[8]={"DEL","DEP","SPD","MIX","FB","WID","LP","INP"};
 inline static constexpr const char* parameterNames[8]={"Delay","Depth","Speed","Mix","Feedback","Width","Low-pass","Input"};
private:
 static juce::AudioProcessorValueTreeState::ParameterLayout createParameters();
 void render(juce::AudioBuffer<float>&,bool);
 std::unique_ptr<nova::AudioAdapter> engine;
 std::array<std::atomic<float>*,8> raw{};
 std::atomic<float>* bypassValue=nullptr;
 std::atomic<bool> resetPending{false};
 JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MonomachineNovaAudioProcessor)
};
