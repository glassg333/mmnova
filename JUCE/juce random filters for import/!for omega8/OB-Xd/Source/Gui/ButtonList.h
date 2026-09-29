
#pragma once
#include <utility>

#include "../Source/Engine/SynthEngine.h"
class ObxdAudioProcessor;

class ButtonList final : public ComboBox, public ScalableComponent
{
    juce::String img_name;
public:
	ButtonList (juce::String nameImg, const int fh, ObxdAudioProcessor *owner) : ComboBox("cb"), ScalableComponent(owner), img_name(std::move(nameImg))
	{
		ButtonList::scaleFactorChanged();
		count = 0;
		h2 = fh;
		w2 = kni.getWidth();
	}

    void scaleFactorChanged() override
    {
        kni = getScaledImageFromCache(img_name, getScaleFactor(), getIsHighResolutionDisplay());
        repaint();
    }
// Source: https://git.iem.at/audioplugins/IEMPluginSuite/-/blob/master/resources/customComponents/ReverseSlider.h
public:
    class ButtonListAttachment final : public juce::AudioProcessorValueTreeState::ComboBoxAttachment
    {
        RangedAudioParameter* parameter = nullptr;
        ButtonList* buttonListToControl = nullptr;
    public:
        ButtonListAttachment (juce::AudioProcessorValueTreeState& stateToControl,
                              const juce::String& parameterID,
                              ButtonList& buttonListToControl) : AudioProcessorValueTreeState::ComboBoxAttachment (stateToControl, parameterID, buttonListToControl), buttonListToControl(&buttonListToControl)
        {
            parameter = stateToControl.getParameter (parameterID);
            buttonListToControl.setParameter (parameter);
        }

        void updateToSlider() const {
	        const float val = parameter->getValue();
            buttonListToControl->setValue(val, NotificationType::dontSendNotification);
        }

        virtual ~ButtonListAttachment() = default;
    };

    void setParameter (const AudioProcessorParameter* p)
    {
        if (parameter == p)
            return;

        parameter = p;
        repaint();
    }
    
	void addChoice (const String& name)
	{
		addItem (name, ++count);
	}
    
	float getValue() const {
		return ((getSelectedId() - 1) / static_cast<float>(count - 1));
	}
    
	void setValue (const float val, const NotificationType notify)
	{
		setSelectedId (static_cast<int>(val * (count - 1) + 1), notify);
	}
    
    void paintOverChildren (Graphics& g) override
	{
		int ofs = getSelectedId() - 1;
        g.drawImage(kni, 0, 0, getWidth(), getHeight(), 0, h2 * ofs * getScaleInt(), w2 * getScaleInt() , h2* getScaleInt());
	}

private:
    int count;
    Image kni;
    int w2, h2;
    const AudioProcessorParameter* parameter {nullptr};
};
