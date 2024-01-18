#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class Impulse6Component : public juce::Component
{
public:
    Impulse6Component();
    ~Impulse6Component();
    
    void resized() override;
private:
    juce::Label impulseLabel;
};
