#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class Scene2Component : public juce::Component
{
public:
    Scene2Component();
    ~Scene2Component();
    
    void resized() override;
private:
    juce::Label helloLabel2;
};
