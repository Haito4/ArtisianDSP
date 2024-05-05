#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class Reverb5Component : public juce::Component
{
public:
    Reverb5Component(ArtisianDSPAudioProcessor& processor) : audioProcessor(processor)
    {
        reverbLabel.setFont(20.f);
        reverbLabel.setJustificationType(juce::Justification::centred);
        reverbLabel.setText("Reverb", juce::dontSendNotification);
        addAndMakeVisible(reverbLabel);
    }
//    ~Reverb5Component();
    
    virtual void resized() override
    {
        reverbLabel.setBounds(getLocalBounds());
    }
private:
    ArtisianDSPAudioProcessor& audioProcessor;
    
    juce::Label reverbLabel;
};
