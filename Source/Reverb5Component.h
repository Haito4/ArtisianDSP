#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class Reverb5Component : public juce::Component
{
public:
    Reverb5Component();
    ~Reverb5Component();
    
    void resized() override;
private:
    juce::Label reverbLabel;
};
