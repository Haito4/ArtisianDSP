#pragma once

#include <JuceHeader.h>

class HelpComponent : public juce::Component
{
public:
    HelpComponent()
    {
        addAndMakeVisible(helpMenuHeading);
        helpMenuHeading.setText("Help Menu", juce::dontSendNotification);
        helpMenuHeading.setJustificationType(juce::Justification::centred);
        helpMenuHeading.setFont(juce::Font(20.0f, juce::Font::bold));
        
        
        addAndMakeVisible(privacyHeading);
        privacyHeading.setText("Privacy Disclosure", juce::dontSendNotification);
        privacyHeading.setFont(juce::Font(20.0f));
        privacyHeading.setJustificationType(juce::Justification::left);
        
        addAndMakeVisible(privacyBody);
        privacyBody.setMultiLine(true);
        privacyBody.setReadOnly(true);
        privacyBody.setScrollbarsShown(true);
        privacyBody.setText("This program doesn't take your personal data.", false);
        privacyBody.setColour(juce::TextEditor::backgroundColourId, juce::Colours::transparentBlack);
        privacyBody.setColour(juce::TextEditor::outlineColourId, juce::Colours::transparentBlack);
        
        
        
        
        
        addAndMakeVisible(linksLabel);
        linksLabel.setText("External Links (will open in default browser)", juce::dontSendNotification);
        linksLabel.setJustificationType(juce::Justification::centred);
        linksLabel.setFont(juce::Font(20.0f));
        
        addAndMakeVisible(onlineHelpButton);
        onlineHelpButton.setButtonText("Go to Online Help");
        onlineHelpButton.onClick = [this](){
            juce::URL url("https://haito4.github.io");
            url.launchInDefaultBrowser();
        };
        
        addAndMakeVisible(viewCodeButton);
        viewCodeButton.setButtonText("View Source Code");
        viewCodeButton.onClick = [this](){
            juce::URL url("https://github.com/Haito4/ArtisianDSP/tree/6d8ce1b1b9851edd188b81b02a9eb14add4b9f76/Source");
            url.launchInDefaultBrowser();
        };
        
        
        
    }

    void paint(juce::Graphics& g) override
    {
        g.setColour(juce::Colours::black.withAlpha(0.6f));
        g.fillRect(0, 0, 720, 510);
        
        g.setColour(juce::Colours::white);
        g.drawRect(helpOverlayBounds, 2);
//        g.drawText("Help Menu", helpOverlayBounds.reduced(10), juce::Justification::centred, true);
    }
    
    void resized() override
    {
        if (isVisible())
        {
            auto overlayWidth = getWidth() / 1.1;
            auto overlayHeight = getHeight() / 1.1;
            helpOverlayBounds = juce::Rectangle<int>((getWidth() - overlayWidth) / 2, (getHeight() - overlayHeight) / 2, overlayWidth, overlayHeight);
        }
        
        
        helpMenuHeading.setBounds(260, 27, 200, 50);
        
        
        privacyHeading.setBounds(36, 380, 230, 50);
        privacyBody.setBounds(37, 430, 230, 50);
        
        linksLabel.setBounds(437, 385, 247, 50);
        viewCodeButton.setBounds(440, 433, 120, 50);
        onlineHelpButton.setBounds(563, 433, 120, 50);
    }
    
private:
    juce::Rectangle<int> helpOverlayBounds;
    
    juce::Label helpMenuHeading;
    juce::Label privacyHeading;
    juce::TextEditor privacyBody;
    
    
    juce::Label howToUseHeading;
    juce::TextEditor howToUseBody;
    
    
    juce::Label linksLabel;
    juce::TextButton onlineHelpButton;
    juce::TextButton viewCodeButton;
    
    
    
};
