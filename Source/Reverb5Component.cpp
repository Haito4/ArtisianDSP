#include "Reverb5Component.h"

Reverb5Component::Reverb5Component()
{
    reverbLabel.setFont(20.f);
    reverbLabel.setJustificationType(juce::Justification::centred);
    reverbLabel.setText("Reverb", juce::dontSendNotification);
    addAndMakeVisible(reverbLabel);
}

void Reverb5Component::resized()
{
    reverbLabel.setBounds(getLocalBounds());
}

Reverb5Component::~Reverb5Component()
{
}
