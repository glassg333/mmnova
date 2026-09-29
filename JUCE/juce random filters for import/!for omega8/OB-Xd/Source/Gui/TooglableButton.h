
#pragma once
#include <utility>

#include "../Source/Engine/SynthEngine.h"
#include "../Components/ScaleComponent.h"
class ObxdAudioProcessor;

class TooglableButton final : public ImageButton, public ScalableComponent
{
    juce::String img_name;
public:
	TooglableButton (juce::String name, ObxdAudioProcessor *owner) : ImageButton(),ScalableComponent(owner), img_name(std::move(name))
	{
        scaleFactorChanged();
		width = kni.getWidth();
		height = kni.getHeight();
		w2 = width;
		h2 = height / 2;
		this->setClickingTogglesState (true);
	}
    void scaleFactorChanged() override
    {
        kni = getScaledImageFromCache(img_name, getScaleFactor(), getIsHighResolutionDisplay());
        repaint();
    }
    ~TooglableButton() override= default;
// Source: https://git.iem.at/audioplugins/IEMPluginSuite/-/blob/master/resources/customComponents/ReverseSlider.h
public:
    class ToggleAttachment final : public juce::AudioProcessorValueTreeState::ButtonAttachment
    {
        RangedAudioParameter* parameter = nullptr;
        TooglableButton* buttonToControl = nullptr;
    public:
        ToggleAttachment (juce::AudioProcessorValueTreeState& stateToControl,
                          const juce::String& parameterID,
                          TooglableButton& buttonToControl) : AudioProcessorValueTreeState::ButtonAttachment (stateToControl, parameterID, buttonToControl), buttonToControl(&buttonToControl)
        {
            parameter = stateToControl.getParameter (parameterID);
        }

        void updateToSlider() const {
	        const float val = parameter->getValue();
            DBG("Toggle Parameter: " << parameter->name << " Val: " << val);
            buttonToControl->setToggleState(val, NotificationType::dontSendNotification);
        }

        ~ToggleAttachment() = default;
    };

	void paintButton (Graphics& g, bool /*isMouseOverButton*/, bool /*isButtonDown*/) override
	{
        int offset = 0;
        
        //if (toogled)
        if (getToggleState())
        {
            offset = 1;
        }
        
		g.drawImage(kni, 0, 0, getWidth(), getHeight(), 0, offset * h2 * getScaleInt() , w2 * getScaleInt(), h2 * getScaleInt());
	}

private:
	Image kni;
	int width, height, w2, h2;
};
