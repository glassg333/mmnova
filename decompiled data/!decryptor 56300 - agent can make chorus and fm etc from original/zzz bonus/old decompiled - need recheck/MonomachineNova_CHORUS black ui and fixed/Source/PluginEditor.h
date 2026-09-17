#pragma once
#include "PluginProcessor.h"
#include "ui/PixelFont.h"

class ChorusLookAndFeel final : public juce::LookAndFeel_V4 {
public:
 juce::Label* createSliderTextBox(juce::Slider&) override;
 void drawRotarySlider(juce::Graphics&,int,int,int,int,float,float,float,juce::Slider&) override;
 void drawButtonBackground(juce::Graphics&,juce::Button&,const juce::Colour&,bool,bool) override;
 void drawButtonText(juce::Graphics&,juce::TextButton&,bool,bool) override;
};
class MonomachineNovaAudioProcessorEditor final : public juce::AudioProcessorEditor,private juce::Timer {
public:
 explicit MonomachineNovaAudioProcessorEditor(MonomachineNovaAudioProcessor&);
 ~MonomachineNovaAudioProcessorEditor() override;
 void paint(juce::Graphics&) override;
 void resized() override;
private:
 void timerCallback() override;
 MonomachineNovaAudioProcessor& processor;
 ChorusLookAndFeel look;
 std::array<juce::Slider,8> sliders;
 std::array<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>,8> attachments;
 juce::TextButton initButton{"INIT"},bypassButton{"BYPASS"};
 std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> bypassAttachment;
 juce::TooltipWindow tooltip{this,700};
 float displayedPeak=0;
 JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MonomachineNovaAudioProcessorEditor)
};
