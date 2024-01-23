#include "Gate1Component.h"

Gate1Component::Gate1Component()
{
    gateLabel.setFont(20.0f);
    gateLabel.setJustificationType(juce::Justification::centred);
    gateLabel.setText("Noise Gate", juce::dontSendNotification);
    addAndMakeVisible(gateLabel);
}

void Gate1Component::resized()
{
    gateLabel.setBounds(getLocalBounds());
}

Gate1Component::~Gate1Component()
{
}

void Gate1Component::processAudioBlock(juce::AudioBuffer<float>& buffer)
    {
        // Implement audio processing logic for Scene 1

    }
