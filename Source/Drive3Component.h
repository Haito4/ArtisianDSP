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
        // Toggle Button
        addAndMakeVisible(driveToggleImage);
        driveToggleImage.addListener(this);
        driveToggleImage.setImages(false, true, true, juce::ImageCache::getFromMemory(BinaryData::dogreen_png, BinaryData::dogreen_pngSize), 0.5f, juce::Colours::green,
                                                      juce::ImageCache::getFromMemory(BinaryData::dohover_png, BinaryData::dohover_pngSize), 0.5f, juce::Colours::blue,
                                                      juce::ImageCache::getFromMemory(BinaryData::dored_png, BinaryData::dored_pngSize), 0.5f, juce::Colours::red);
        driveToggleImage.setClickingTogglesState(true);
        driveToggleAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "USING_TS", driveToggleImage);
        
        
        // Text
        driveLabel.setFont(20.f);
        driveLabel.setJustificationType(juce::Justification::centred);
        driveLabel.setText("Overdrive", juce::dontSendNotification);
        addAndMakeVisible(driveLabel);
        
        // Knobs
        addAndMakeVisible(driveKnob);
        driveKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        driveKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 15);
        driveKnob.setRange(0.1f, 200.f);
        driveKnob.setValue(0.5);
        driveKnob.addListener(this);
        driveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "TS_DRIVE", driveKnob);
        
        addAndMakeVisible(toneKnob);
        toneKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
        toneKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 15);
        toneKnob.setTextValueSuffix(" Hz");
        toneKnob.setRange(20.f, 700.f);
        toneKnob.setValue(20.f);
        toneKnob.addListener(this);
        toneAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "TS_TONE", toneKnob);
        
        addAndMakeVisible(volumeKnob);
        volumeKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        volumeKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 15);
        volumeKnob.setTextValueSuffix(" %");
        volumeKnob.setRange(0.f, 1.f);
        volumeKnob.setValue(1.f);
        driveKnob.addListener(this);
        volumeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "TS_LEVEL", volumeKnob);
    }
    
//    ~Drive3Component();
    
    virtual void resized() override
    {
        driveLabel.setBounds(290, 350, 140, 50);
        
        driveKnob.setBounds(400, 130, 100, 100);
        toneKnob.setBounds(315, 230, 90, 90);
        volumeKnob.setBounds(220, 130, 100, 100);
        
        driveToggleImage.setBounds(335, 450, 50, 50);
    }
    
    void buttonClicked(juce::Button* button) override
    {
        
    }
    
    
    void sliderValueChanged(juce::Slider* slider) override
    {
        
    }

private:
    ArtisianDSPAudioProcessor& audioProcessor;
    
    
    // Buttons, Sliders
    juce::ImageButton driveToggleImage;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> driveToggleAttachment;
    
    
    juce::Slider driveKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> driveAttachment;
    
    juce::Slider toneKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> toneAttachment;
    
    juce::Slider volumeKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> volumeAttachment;
    
    
    // GUI
    juce::Label driveLabel;

 
};
