#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class Amp4Component : public juce::Component
{
public:
    Amp4Component();
    ~Amp4Component();
    
    void resized() override;
private:
    juce::Label ampLabel;
};
