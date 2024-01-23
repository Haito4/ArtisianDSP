#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class Comp2Component : public juce::Component
{
public:
    Comp2Component();
    ~Comp2Component();
    
    void processAudioBlock(juce::AudioBuffer<float>& buffer);
    void resized() override;
private:
    juce::Label compLabel;
};
