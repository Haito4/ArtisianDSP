#include "MultiSceneComponent.h"

MultiSceneComponent::MultiSceneComponent()
    : scene1(std::make_unique<Scene1Component>()),
      scene2(std::make_unique<Scene2Component>())
{
    addAndMakeVisible(scene1.get());
    addAndMakeVisible(scene2.get());
    scene2->setVisible(false); // Initially hide Scene2

    sceneButton1.onClick = [this] { showScene(1); };
    sceneButton2.onClick = [this] { showScene(2); };

    sceneButton1.setButtonText("Scene 1");
    sceneButton2.setButtonText("Scene 2");

    addAndMakeVisible(sceneButton1);
    addAndMakeVisible(sceneButton2);
}

MultiSceneComponent::~MultiSceneComponent()
{
}

void MultiSceneComponent::resized()
{
    scene1->setBounds(getLocalBounds());
    scene2->setBounds(getLocalBounds());

    sceneButton1.setBounds(10, 10, 80, 30);
    sceneButton2.setBounds(100, 10, 80, 30);
}

void MultiSceneComponent::showScene(int sceneIndex)
{
    scene1->setVisible(sceneIndex == 1);
    scene2->setVisible(sceneIndex == 2);
}

