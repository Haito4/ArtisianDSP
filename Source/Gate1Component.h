#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class Gate1Component : public juce::Component
{
public:
    Gate1Component();
    ~Gate1Component();

    void resized() override;
private:
    juce::Label helloLabel;
};
