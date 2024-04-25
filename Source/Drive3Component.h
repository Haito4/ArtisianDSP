#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

class Drive3Component : public juce::Component,
                        public juce::Button::Listener,
                        public juce::Slider::Listener
{
public:
    Drive3Component(ArtisianDSPAudioProcessor& processor) : audioProcessor(processor)
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
        toneKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
        toneKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 15);
        toneKnob.setTextValueSuffix(" Hz");
        toneKnob.setRange(20.f, 700.f);
        toneKnob.setValue(20.f);
        toneKnob.addListener(this);
        toneAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "TS_TONE", toneKnob);
        
        
        
        addAndMakeVisible(volumeKnob);
        volumeKnob.setSliderStyle(juce::Slider::Rotary);
        volumeKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
        volumeKnob.setRange(0.0, 1.0);
        volumeKnob.setValue(0.5);
    }
    
//    ~Drive3Component();
    
    virtual void resized() override
    {
        driveLabel.setBounds(290, 350, 140, 50);
        
        driveKnob.setBounds(400, 130, 100, 100);
        
        toneKnob.setBounds(315, 230, 90, 90);
        
        volumeKnob.setBounds(220, 130, 100, 100);
    }
    
    void buttonClicked(juce::Button* button) override
    {
        
    }
    
    
    void sliderValueChanged(juce::Slider* slider) override
    {
        
    }

private:
    ArtisianDSPAudioProcessor& audioProcessor;
    
    
    // Sliders
    juce::Slider driveKnob;
    
    
    juce::Slider toneKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> toneAttachment;
    
    
    juce::Slider volumeKnob;

    
    
    // GUI
    juce::Label driveLabel;

 
};
