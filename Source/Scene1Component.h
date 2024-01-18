#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class Scene1Component : public juce::Component
{
public:
    Scene1Component();
    ~Scene1Component();

    void resized() override;
private:
    juce::Label helloLabel;
};
