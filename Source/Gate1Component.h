#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class Gate1Component : public juce::Component
{
public:
    Gate1Component()
    {
        helloLabel.setFont(20.0f);
        helloLabel.setJustificationType(juce::Justification::centred);
        helloLabel.setText("Noise Gate", juce::dontSendNotification);
        addAndMakeVisible(helloLabel);
    }
//    ~Gate1Component();

    virtual void resized() override
    {
        helloLabel.setBounds(getLocalBounds());
    }

private:
    juce::Label helloLabel;
};

