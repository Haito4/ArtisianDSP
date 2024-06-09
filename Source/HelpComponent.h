#pragma once

#include <JuceHeader.h>

class HelpComponent : public juce::Component
{
public:
    HelpComponent()
    {
        addAndMakeVisible(onlineHelpButton);
        onlineHelpButton.setButtonText("Go to Online Help");
        onlineHelpButton.onClick = [this](){
            juce::URL url("https://haito4.github.io/docs/");
            url.launchInDefaultBrowser();
        };
        
        
        
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
        onlineHelpButton.setBounds(563, 433, 120, 50);
        
        if (isVisible())
        {
            auto overlayWidth = getWidth() / 1.1;
            auto overlayHeight = getHeight() / 1.1;
            helpOverlayBounds = juce::Rectangle<int>((getWidth() - overlayWidth) / 2, (getHeight() - overlayHeight) / 2, overlayWidth, overlayHeight);
        }
    }
    
private:
    juce::Rectangle<int> helpOverlayBounds;
    
    juce::TextButton onlineHelpButton;
    
    
};
