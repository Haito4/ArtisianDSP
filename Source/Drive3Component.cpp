#include "Drive3Component.h"

Drive3Component::Drive3Component()
{
    //Text
    driveLabel.setFont(20.f);
    driveLabel.setJustificationType(juce::Justification::centred);
    driveLabel.setText("Overdrive", juce::dontSendNotification);
    addAndMakeVisible(driveLabel);
    
    //Knobs
    addAndMakeVisible(driveKnob);
    driveKnob.setSliderStyle(juce::Slider::Rotary);
    driveKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    driveKnob.setRange(0.0, 1.0);
    driveKnob.setValue(0.5);
    
    addAndMakeVisible(toneKnob);
    toneKnob.setSliderStyle(juce::Slider::Rotary);
    toneKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    toneKnob.setRange(0.0, 1.0);
    toneKnob.setValue(0.5);
    
    addAndMakeVisible(volumeKnob);
    volumeKnob.setSliderStyle(juce::Slider::Rotary);
    volumeKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    volumeKnob.setRange(0.0, 1.0);
    volumeKnob.setValue(0.5);
}

juce::AudioProcessorValueTreeState::ParameterLayout Drive3Component::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout driveLayout;
    
    driveLayout.add(std::make_unique<juce::AudioParameterFloat>("Drive",
                                                                "Drive",
                                                                 juce::NormalisableRange<float>(0.0, 1.0, 0.05, 1.f),
                                                                   0.0));
    return driveLayout;
}

void Drive3Component::resized()
{
    driveLabel.setBounds(getLocalBounds());
    
    driveKnob.setBounds(270, 270, 100, 100);
    
    toneKnob.setBounds(360, 270, 100, 100);
    
    volumeKnob.setBounds(450, 270, 100, 100);
}

Drive3Component::~Drive3Component()
{
}
