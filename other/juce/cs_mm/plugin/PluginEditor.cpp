#include "PluginEditor.h"

CsMmAudioProcessorEditor::CsMmAudioProcessorEditor(CsMmAudioProcessor& p):AudioProcessorEditor(&p),processor(p)
{
    algorithmLabel.setText("ALGORITHM",juce::dontSendNotification);algorithmLabel.setColour(juce::Label::textColourId,juce::Colours::lightgrey);addAndMakeVisible(algorithmLabel);
    for(int i=0;i<csmm::CsMmVariantsDSP::variantCount;++i)algorithm.addItem(csmm::CsMmVariantsDSP::getVariantName(i),i+1);addAndMakeVisible(algorithm);algorithmAttachment=std::make_unique<ComboAttachment>(processor.parameters,"variant",algorithm);
    const std::array<juce::String,12> names{"FEEDBACK","DAMP","PHASE","DIFFUSION","CROSS MIX","MOTION","CHORUS MIX","CHORUS DEPTH","CHORUS RATE","DELAY 1 MS","DELAY 2 MS","SCAN"};
    const std::array<const char*,12> ids{"feedback","damp","phase","diffusion","crossMix","motion","chorusMix","chorusDepth","chorusRate","delay1","delay2","scan"};
    for(std::size_t i=0;i<12;++i){labels[i].setText(names[i],juce::dontSendNotification);labels[i].setColour(juce::Label::textColourId,juce::Colours::lightgrey);labels[i].setJustificationType(juce::Justification::centred);sliders[i].setSliderStyle(juce::Slider::LinearVertical);sliders[i].setTextBoxStyle(juce::Slider::TextBoxBelow,false,72,20);addAndMakeVisible(labels[i]);addAndMakeVisible(sliders[i]);attachments[i]=std::make_unique<SliderAttachment>(processor.parameters,ids[i],sliders[i]);}
    setSize(1220,340);
}
void CsMmAudioProcessorEditor::paint(juce::Graphics& g){g.fillAll(juce::Colour(0xff15191a));g.setColour(juce::Colour(0xff29322f));g.fillRoundedRectangle(getLocalBounds().toFloat().reduced(10),10);g.setColour(juce::Colours::white);g.setFont(juce::FontOptions(22,juce::Font::bold));g.drawText("CS MM / COMB + MONOMACHINE CHORUS",24,16,getWidth()-48,30,juce::Justification::centredLeft);}
void CsMmAudioProcessorEditor::resized(){auto a=getLocalBounds().reduced(24);a.removeFromTop(42);auto top=a.removeFromTop(42);algorithmLabel.setBounds(top.removeFromLeft(90));algorithm.setBounds(top);a.removeFromTop(12);const int w=a.getWidth()/12;for(std::size_t i=0;i<12;++i){auto col=a.removeFromLeft(w).reduced(4,0);labels[i].setBounds(col.removeFromTop(24));sliders[i].setBounds(col);}}
