#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class Gate1Component : public juce::Component
{
public:
    Gate1Component();
    ~Gate1Component();
    
    void processAudioBlock(juce::AudioBuffer<float>& buffer);
    void resized() override;
private:
    juce::Label gateLabel;
};
