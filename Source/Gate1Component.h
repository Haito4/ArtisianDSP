#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

class Gate1Component : public juce::Component,
                       public juce::Button::Listener
{
public:
    Gate1Component(ArtisianDSPAudioProcessor& processor) : audioProcessor(processor)
    {
        helloLabel.setFont(20.0f);
        helloLabel.setJustificationType(juce::Justification::centred);
        helloLabel.setText("Noise Gate", juce::dontSendNotification);
        addAndMakeVisible(helloLabel);
        
        gateToggle.setButtonText("Gate On/Off");
        gateToggle.addListener(this);
        addAndMakeVisible(gateToggle);
        
    }
//    ~Gate1Component();

    virtual void resized() override
    {
        helloLabel.setBounds(getLocalBounds());
        gateToggle.setBounds(360, 270, 100, 50);
        
    }
        
    void buttonClicked(juce::Button* button) override
    {
        if (button == &gateToggle)
        {
            audioProcessor.usingGate = !audioProcessor.usingGate;
            
            juce::Logger::outputDebugString("usingGate: " + juce::String(audioProcessor.usingGate ? "true" : "false"));
        }
    };

private:
    ArtisianDSPAudioProcessor& audioProcessor;
    
    juce::Label helloLabel;
    
    juce::TextButton gateToggle;
};
