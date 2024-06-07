#pragma once

#include <JuceHeader.h>

class HelpComponent : public juce::Component
{
public:
    HelpComponent()
    {
    }

    void paint(juce::Graphics& g) override
    {
        g.setColour(juce::Colours::black.withAlpha(0.6f));
        g.fillRect(0, 0, 720, 510);
        
        g.setColour(juce::Colours::white);
        g.drawRect(helpOverlayBounds, 2);
        g.drawText("Help Menu", helpOverlayBounds.reduced(10), juce::Justification::centred, true);
    }
    
    void resized() override
    {
        if (isVisible())
        {
            auto overlayWidth = getWidth() / 2;
            auto overlayHeight = getHeight() / 2;
            helpOverlayBounds = juce::Rectangle<int>((getWidth() - overlayWidth) / 2, (getHeight() - overlayHeight) / 2, overlayWidth, overlayHeight);
        }
    }
    
private:
    juce::Rectangle<int> helpOverlayBounds;
};
