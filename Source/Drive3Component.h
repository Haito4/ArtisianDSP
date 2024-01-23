#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class Drive3Component : public juce::Component
{
public:
    Drive3Component();
    ~Drive3Component();

    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    void resized() override;

private:
    // Parameters
    juce::Slider driveKnob;
    juce::Slider toneKnob;
    juce::Slider volumeKnob;

    // GUI
    juce::Label driveLabel;

 
};
