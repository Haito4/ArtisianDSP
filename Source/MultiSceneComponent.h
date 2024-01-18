#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Scene1Component.h"
#include "Scene2Component.h"
#include <memory>

class MultiSceneComponent : public juce::Component
{
public:
    MultiSceneComponent();
    ~MultiSceneComponent();

    void resized() ;

private:
    std::unique_ptr<Scene1Component> scene1;
    std::unique_ptr<Scene2Component> scene2;

    juce::TextButton sceneButton1;
    juce::TextButton sceneButton2;

    void showScene(int sceneIndex);
};
