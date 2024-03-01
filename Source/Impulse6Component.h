#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class Impulse6Component : public juce::Component
{
public:
    Impulse6Component()
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
    juce::Label impulseLabel;
};
