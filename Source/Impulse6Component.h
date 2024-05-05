#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class Impulse6Component : public juce::Component
{
public:
    Impulse6Component(ArtisianDSPAudioProcessor& processor) : audioProcessor(processor)
    {
        impulseLabel.setFont(20.f);
        impulseLabel.setJustificationType(juce::Justification::centred);
        impulseLabel.setText("Impulse Response", juce::dontSendNotification);
        addAndMakeVisible(impulseLabel);
    }
//    ~Impulse6Component();
    
    virtual void resized() override
    {
        impulseLabel.setBounds(getLocalBounds());
    }
private:
    ArtisianDSPAudioProcessor& audioProcessor;
    
    juce::Label impulseLabel;
};
