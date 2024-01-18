#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class Drive3Component : public juce::Component
{
public:
    Drive3Component();
    ~Drive3Component();
    
    void resized() override;
private:
    juce::Label driveLabel;
};
