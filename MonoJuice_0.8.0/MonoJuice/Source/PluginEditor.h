#pragma once
#include "PluginProcessor.h"
#include "ui/PixelFont.h"
namespace theme {inline const juce::Colour bg{0xff101010},panel{0xff181818},ink{0xffd4d4d4},dim{0xff737373};}
class RackLookAndFeel final:public juce::LookAndFeel_V4 {
public:
 RackLookAndFeel();juce::Label* createSliderTextBox(juce::Slider&)override;juce::Label* createComboBoxTextBox(juce::ComboBox&)override;
 void drawRotarySlider(juce::Graphics&,int,int,int,int,float,float,float,juce::Slider&)override;
 void drawButtonBackground(juce::Graphics&,juce::Button&,const juce::Colour&,bool,bool)override;
 void drawToggleButton(juce::Graphics&,juce::ToggleButton&,bool,bool)override;
 void drawButtonText(juce::Graphics&,juce::TextButton&,bool,bool)override;
 void drawComboBox(juce::Graphics&,int,int,bool,int,int,int,int,juce::ComboBox&)override;
 void positionComboBoxText(juce::ComboBox&,juce::Label&)override;
};
class MonoJuiceAudioProcessorEditor final:public juce::AudioProcessorEditor,private juce::Timer {
public:
 explicit MonoJuiceAudioProcessorEditor(MonoJuiceAudioProcessor&);~MonoJuiceAudioProcessorEditor()override;
 void paint(juce::Graphics&)override;void resized()override;void showSlot(int);void showPage(int);int currentType()const{return type;}
private:
 void timerCallback()override;void bindControls();void refreshPreset();
 MonoJuiceAudioProcessor& processor;RackLookAndFeel look;
 using SliderAttachment=juce::AudioProcessorValueTreeState::SliderAttachment;using ButtonAttachment=juce::AudioProcessorValueTreeState::ButtonAttachment;using ComboAttachment=juce::AudioProcessorValueTreeState::ComboBoxAttachment;
 std::array<juce::Slider,model::count> sliders;std::array<std::unique_ptr<SliderAttachment>,model::count> attachments;
 std::array<juce::TextButton,5> tabs,enabled;std::array<juce::ComboBox,5> types;
 std::array<std::unique_ptr<ButtonAttachment>,5> enableAttachments;std::array<std::unique_ptr<ComboAttachment>,5> typeAttachments;
 juce::Slider masterFeedback;std::unique_ptr<SliderAttachment> masterAttachment;
 juce::ToggleButton clipper{"CLIPPER"},dcBlock{"DC BLOCK"};juce::TextButton globalPanic{"PANIC ALL"},negativeA{"NEG A"},negativeB{"NEG B"};
 std::unique_ptr<ButtonAttachment> clipAttachment,dcAttachment,negativeAAttachment,negativeBAttachment;
 juce::ComboBox modelMenu,odinRouting,chorusMode;std::unique_ptr<ComboAttachment> modelAttachment,routingAttachment,chorusModeAttachment;
 juce::TextButton randomise{"RANDOM"};juce::Random randomSource;
 juce::TextButton init{"INIT FX"},clear{"CLEAR FX"},freeze{"FREEZE"},metalPreset{"METAL CLOUD"},bypass{"BYPASS ALL"},moveLeft{"< MOVE"},moveRight{"MOVE >"},midSide{"MID/SIDE"},oversample{"16X OS"},spectralFreeze{"SPEC FREEZE"},panic{"PANIC!"},previousPage{"< PAGE"},nextPage{"PAGE >"};
 juce::ComboBox presetMenu,spectreWindow;std::unique_ptr<ComboAttachment> windowAttachment;
 std::unique_ptr<ButtonAttachment> freezeAttachment,bypassAttachment,midSideAttachment,oversampleAttachment,spectralFreezeAttachment;
 juce::TooltipWindow tooltip{this,650};int selected=1,type=2,page=0,variant=0;float peak=0;
 JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MonoJuiceAudioProcessorEditor)
};
