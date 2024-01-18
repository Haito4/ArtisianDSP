#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Scene1Component.h"
#include "Scene2Component.h"

class MultiSceneComponent : public Component
{
public:
    MultiSceneComponent();
    ~MultiSceneComponent();

    void resized() override;

private:
    ScopedPointer<Scene1Component> scene1;
    ScopedPointer<Scene2Component> scene2;

    TextButton sceneButton1;
    TextButton sceneButton2;

    void showScene(int sceneIndex);
};
