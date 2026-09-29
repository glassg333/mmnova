
#pragma once
#include <utility>

#include "../Source/Engine/SynthEngine.h"
class ObxdAudioProcessor;


class ImageMenu : public ImageButton,
                                  public ScalableComponent
{
    juce::String img_name;
public:
    ImageMenu(juce::String nameImg, ObxdAudioProcessor* owner_)
        :  ScalableComponent(owner_), img_name(std::move(nameImg))
    {
	    ImageMenu::scaleFactorChanged();

        setOpaque(false);
	    Component::setVisible(true);
    }


    void scaleFactorChanged() override
    {
        const float scaleFactor = getScaleFactor();
        const bool isHighResolutionDisplay = getIsHighResolutionDisplay();

        const Image normalImage = getScaledImageFromCache(img_name, scaleFactor, isHighResolutionDisplay);
        const Image downImage = getScaledImageFromCache(img_name, scaleFactor, isHighResolutionDisplay);

        constexpr bool resizeButtonNowToFitThisImage = false;
        constexpr bool rescaleImagesWhenButtonSizeChanges = true;
        constexpr bool preserveImageProportions = true;

        setImages(resizeButtonNowToFitThisImage,
                  rescaleImagesWhenButtonSizeChanges,
                  preserveImageProportions,
                  normalImage,
                  1.0f, // menu transparency
                  Colour(),
                  normalImage,
                  1.0f, // menu hover transparency
                  Colour(),
                  downImage,
                  0.3f, // menu click transparency
                  Colour());

        repaint();
    }


protected:
};
