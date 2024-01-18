#include "Gate1Component.h"

Gate1Component::Gate1Component()
{
    helloLabel.setFont(20.0f);
    helloLabel.setJustificationType(juce::Justification::centred);
    helloLabel.setText("Noise Gate", juce::dontSendNotification);
    addAndMakeVisible(helloLabel);
}

void Gate1Component::resized()
{
    helloLabel.setBounds(getLocalBounds());
}

Gate1Component::~Gate1Component()
{
}

