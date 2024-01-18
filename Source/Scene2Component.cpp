#include "Scene2Component.h"

Scene2Component::Scene2Component()
{
    helloLabel2.setText("Hello World 2", juce::dontSendNotification);
    addAndMakeVisible(helloLabel2);
}

void Scene2Component::resized()
{
    helloLabel2.setBounds(getLocalBounds());
}

Scene2Component::~Scene2Component()
{
}
