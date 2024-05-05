#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class Amp4Component : public juce::Component
{
public:
    Amp4Component(ArtisianDSPAudioProcessor& processor) : audioProcessor(processor)
    {
        ampLabel.setFont(20.f);
        ampLabel.setJustificationType(juce::Justification::centred);
        ampLabel.setText("Amplifier", juce::dontSendNotification);
        addAndMakeVisible(ampLabel);
    }
//    ~Amp4Component();
    
    virtual void resized() override
    {
        ampLabel.setBounds(getLocalBounds());
    }
    
private:
    ArtisianDSPAudioProcessor& audioProcessor;
    
    juce::Label ampLabel;
};
