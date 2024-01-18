#include "Impulse6Component.h"

Impulse6Component::Impulse6Component()
{
    impulseLabel.setFont(20.f);
    impulseLabel.setJustificationType(juce::Justification::centred);
    impulseLabel.setText("Impulse Response", juce::dontSendNotification);
    addAndMakeVisible(impulseLabel);
}

void Impulse6Component::resized()
{
    impulseLabel.setBounds(getLocalBounds());
}

Impulse6Component::~Impulse6Component()
{
}
