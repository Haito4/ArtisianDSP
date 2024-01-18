#include "Drive3Component.h"

Drive3Component::Drive3Component()
{
    driveLabel.setFont(20.f);
    driveLabel.setJustificationType(juce::Justification::centred);
    driveLabel.setText("Overdrive", juce::dontSendNotification);
    addAndMakeVisible(driveLabel);
}

void Drive3Component::resized()
{
    driveLabel.setBounds(getLocalBounds());
}

Drive3Component::~Drive3Component()
{
}
