#include "PluginEditor.h"

namespace {
class PixelValueLabel final : public juce::Label {
 void paint(juce::Graphics& g) override {
  g.fillAll(juce::Colours::black);
  if(!isBeingEdited()){g.setColour(juce::Colours::white);pixel::text(g,getText(),getLocalBounds(),14,true);}
 }
};
}
juce::Label* ChorusLookAndFeel::createSliderTextBox(juce::Slider&){
 auto* l=new PixelValueLabel();l->setJustificationType(juce::Justification::centred);
 l->setKeyboardType(juce::TextInputTarget::decimalKeyboard);
 l->setColour(juce::TextEditor::textColourId,juce::Colours::white);
 l->setColour(juce::TextEditor::backgroundColourId,juce::Colours::black);
 l->setColour(juce::TextEditor::outlineColourId,juce::Colours::white);
 return l;
}

void ChorusLookAndFeel::drawRotarySlider(juce::Graphics& g,int x,int y,int w,int h,float position,float start,float end,juce::Slider& slider){
 const int cx=x+w/2,cy=y+h/2;const float radius=float(std::min(w,h))/2.0f-8.0f;
 g.setColour(juce::Colours::white);
 for(int step=0;step<72;++step){
  const float angle=juce::MathConstants<float>::twoPi*float(step)/72.0f;
  const int px=int(std::round(float(cx)+std::sin(angle)*radius));
  const int py=int(std::round(float(cy)-std::cos(angle)*radius));
  g.fillRect((px/2)*2,(py/2)*2,3,3);
 }
 const float a=start+position*(end-start);
 for(int d=5;d<int(radius)-3;d+=2){const int px=int(std::round(float(cx)+std::sin(a)*float(d)));const int py=int(std::round(float(cy)-std::cos(a)*float(d)));g.fillRect((px/2)*2,(py/2)*2,3,3);}
 if(slider.isMouseOverOrDragging()){g.drawRect(x+2,y+2,w-4,h-4,1);}
}
void ChorusLookAndFeel::drawButtonBackground(juce::Graphics& g,juce::Button& b,const juce::Colour&,bool hover,bool down){
 const bool filled=b.getToggleState()||down;g.fillAll(filled?juce::Colours::white:juce::Colours::black);
 g.setColour(juce::Colours::white);g.drawRect(b.getLocalBounds(),hover?3:2);
}
void ChorusLookAndFeel::drawButtonText(juce::Graphics& g,juce::TextButton& b,bool,bool down){
 g.setColour(b.getToggleState()||down?juce::Colours::black:juce::Colours::white);
 pixel::text(g,b.getButtonText(),b.getLocalBounds(),14,true);
}
MonomachineNovaAudioProcessorEditor::MonomachineNovaAudioProcessorEditor(MonomachineNovaAudioProcessor& p)
 : AudioProcessorEditor(&p),processor(p){
 setLookAndFeel(&look);
 look.setColour(juce::Slider::textBoxTextColourId,juce::Colours::white);
 look.setColour(juce::Slider::textBoxBackgroundColourId,juce::Colours::black);
 look.setColour(juce::Slider::textBoxOutlineColourId,juce::Colours::black);
 look.setColour(juce::Slider::textBoxHighlightColourId,juce::Colours::darkgrey);
 for(int i=0;i<8;++i){
  auto& s=sliders[std::size_t(i)];addAndMakeVisible(s);
  s.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);s.setTextBoxStyle(juce::Slider::TextBoxBelow,false,64,22);
  s.setRange(0,127,1);s.setDoubleClickReturnValue(true,nova::ChorusCore::defaults[std::size_t(i)]);
  s.setRotaryParameters(juce::MathConstants<float>::pi*1.25f,juce::MathConstants<float>::pi*2.75f,true);
  s.setTooltip(juce::String(MonomachineNovaAudioProcessor::parameterNames[i])+" / 0-127. Double-click: default. Shift-drag: fine adjustment.");
  attachments[std::size_t(i)]=std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters,MonomachineNovaAudioProcessor::parameterIds[i],s);
 }
 addAndMakeVisible(initButton);initButton.onClick=[this]{processor.initialisePatch();};initButton.setTooltip("Reset all eight CHORUS parameters and clear its delay memory.");
 addAndMakeVisible(bypassButton);bypassButton.setClickingTogglesState(true);
 bypassAttachment=std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(p.parameters,"BYPASS",bypassButton);
 setSize(720,436);setResizable(false,false);startTimerHz(20);
}
MonomachineNovaAudioProcessorEditor::~MonomachineNovaAudioProcessorEditor(){stopTimer();setLookAndFeel(nullptr);}
void MonomachineNovaAudioProcessorEditor::resized(){
 initButton.setBounds(510,17,76,30);bypassButton.setBounds(598,17,104,30);
 for(int i=0;i<8;++i){const int col=i%4,row=i/4;sliders[std::size_t(i)].setBounds(18+col*171+40,138+row*126,90,92);}
}
void MonomachineNovaAudioProcessorEditor::timerCallback(){displayedPeak=std::max(processor.outputPeak.exchange(0.0f),displayedPeak*0.84f);repaint();}
void MonomachineNovaAudioProcessorEditor::paint(juce::Graphics& g){
 g.fillAll(juce::Colours::black);g.setColour(juce::Colours::white);
 pixel::text(g,"MONOMACHINE NOVA",{18,16,430,32},21);
 pixel::text(g,"NATIVE DSP / OS 1.32B",{18,51,470,22},7);
 g.fillRect(18,83,684,33);g.setColour(juce::Colours::black);
 pixel::text(g,"FX CHORUS",{28,84,340,31},21);
 pixel::text(g,"STEREO",{556,84,132,31},14,true);
 g.setColour(juce::Colours::white);
 for(int i=0;i<=4;++i)pixel::dotted(g,18+i*171,120,18+i*171,375);
 for(int row=0;row<=2;++row)pixel::dotted(g,18,120+row*126,702,120+row*126);
 for(int i=0;i<8;++i){const int col=i%4,row=i/4;pixel::text(g,MonomachineNovaAudioProcessor::parameterIds[i],{20+col*171,125+row*126,167,21},14,true);}
 g.fillRect(18,390,684,2);
 pixel::text(g,"24 BIT / 16 FRAME",{18,400,310,20},7);
 const auto rate=processor.getSampleRate();const auto info=rate>0?juce::String(rate/1000.0,1)+"K / "+juce::String(processor.getLatencySamples())+" SMP":"44.1K ENGINE";
 pixel::text(g,info,{313,400,210,20},7);
 pixel::text(g,"OUT",{562,400,40,20},7);
 const int lit=juce::jlimit(0,12,int(std::sqrt(displayedPeak)*12.0f));
 for(int i=0;i<12;++i){juce::Rectangle<int> bar(606+i*8,403,5,12);if(i<lit)g.fillRect(bar);else g.drawRect(bar,1);}
}
