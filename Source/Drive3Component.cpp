#include "Drive3Component.h"
#include "PluginProcessor.h"

Drive3Component::Drive3Component(ArtisianDSPAudioProcessor& processor) : audioProcessor(processor)
{
    //Text
    driveLabel.setFont(20.f);
    driveLabel.setJustificationType(juce::Justification::centred);
    driveLabel.setText("Overdrive", juce::dontSendNotification);
    addAndMakeVisible(driveLabel);
    
    //Knobs
    driveKnob = std::make_unique<juce::Slider>("Drive");
    addAndMakeVisible(driveKnob.get()); //need to fix to get it to show up
    driveKnob->setSliderStyle(juce::Slider::Rotary);
    driveKnob->setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 100);
    
    //Attachments
    driveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getState(), "drive", *driveKnob);
    
    driveKnob->setRange(0.0, 1.0);
}

void Drive3Component::resized()
{
    driveLabel.setBounds(getLocalBounds());
}

Drive3Component::~Drive3Component()
{
}
