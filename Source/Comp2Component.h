#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class Comp2Component : public juce::Component
{
public:
    Comp2Component()
    {
        compLabel.setFont(20.f);
        compLabel.setJustificationType(juce::Justification::centred);
        compLabel.setText("Compressor", juce::dontSendNotification);
        addAndMakeVisible(compLabel);
    }
//    ~Comp2Component();
    
    virtual void resized() override
    {
        compLabel.setBounds(getLocalBounds());
    }
private:
    juce::Label compLabel;
};
