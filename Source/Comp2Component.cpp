#include "Comp2Component.h"

Comp2Component::Comp2Component()
{
    compLabel.setFont(20.f);
    compLabel.setJustificationType(juce::Justification::centred);
    compLabel.setText("Compressor", juce::dontSendNotification);
    addAndMakeVisible(compLabel);
}

void Comp2Component::resized()
{
    compLabel.setBounds(getLocalBounds());
}

Comp2Component::~Comp2Component()
{
}
