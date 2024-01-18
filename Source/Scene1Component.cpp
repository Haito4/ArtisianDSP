#include "Scene1Component.h"

Scene1Component::Scene1Component()
{
    helloLabel.setFont(20.0f);
    helloLabel.setJustificationType(juce::Justification::centred);
    helloLabel.setText("Hello World", juce::dontSendNotification);
    addAndMakeVisible(helloLabel);
}

void Scene1Component::resized()
{
    helloLabel.setBounds(getLocalBounds());
}

Scene1Component::~Scene1Component()
{
}

