#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

class Drive3Component : public juce::Component,
                        public juce::Button::Listener,
                        public juce::Slider::Listener
{
public:
    Drive3Component(ArtisianDSPAudioProcessor& processor) : audioProcessor(processor), tooltipWindow(this, 900)
    {
        // Toggle Button
        addAndMakeVisible(driveToggleImage);
        driveToggleImage.addListener(this);
        driveToggleImage.setImages(false, true, true,
                                 juce::ImageCache::getFromMemory(BinaryData::fsup_png, BinaryData::fsup_pngSize), 1.0f, juce::Colours::transparentBlack,
                                 juce::ImageCache::getFromMemory(BinaryData::fsup_png, BinaryData::fsup_pngSize), 1.0f, juce::Colours::transparentBlack,
                                 juce::ImageCache::getFromMemory(BinaryData::fsdown_png, BinaryData::fsdown_pngSize), 1.0f, juce::Colours::transparentBlack);
        driveToggleImage.setClickingTogglesState(true);
        driveToggleAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "USING_TS", driveToggleImage);
        driveToggleImage.setTooltip("Turns the overdrive On/Off.");
        
        // Text
        driveSceneLabel.setFont(20.f);
        driveSceneLabel.setJustificationType(juce::Justification::centred);
        driveSceneLabel.setText("Overdrive", juce::dontSendNotification);
        addAndMakeVisible(driveSceneLabel);
        
        // Knobs
        addAndMakeVisible(driveKnob);
        driveKnob.setTooltip("Adjusts the amount of distortion applied to the signal. Higher values result in more aggressive distortion.");
        driveKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        driveKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 15);
        driveKnob.setRange(0.1f, 200.f);
        driveKnob.setValue(0.5);
        
        driveLabel.setText ("Drive", juce::NotificationType::dontSendNotification);
        driveLabel.attachToComponent (&driveKnob, false);
        
        driveKnob.addListener(this);
        driveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "TS_DRIVE", driveKnob);
        
        
        
        
        addAndMakeVisible(toneKnob);
        toneKnob.setTooltip("Alters the starting point of the low-cut filter. Higher values result in a tighter, brighter sound, while lower values result in warmer, muddier tone.");
        toneKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
        toneKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 15);
        toneKnob.setTextValueSuffix(" Hz");
        toneKnob.setRange(20.f, 700.f);
        toneKnob.setValue(20.f);
        
        toneLabel.setText ("Tone", juce::NotificationType::dontSendNotification);
        toneLabel.attachToComponent (&toneKnob, false);
        
        toneKnob.addListener(this);
        toneAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "TS_TONE", toneKnob);
        
        addAndMakeVisible(volumeKnob);
        volumeKnob.setTooltip("Adjusts the overall output level of the affected signal.");
        volumeKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        volumeKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 15);
        volumeKnob.setTextValueSuffix(" %");
        volumeKnob.setRange(0.f, 1.f);
        volumeKnob.setValue(1.f);
        
        volumeLabel.setText ("Level", juce::NotificationType::dontSendNotification);
        volumeLabel.attachToComponent (&volumeKnob, false);
        
        driveKnob.addListener(this);
        volumeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "TS_LEVEL", volumeKnob);
    }
    
//    ~Drive3Component();
    
    virtual void resized() override
    {
        driveSceneLabel.setBounds(290, 350, 140, 50);
        
        driveKnob.setBounds(400, 130, 100, 100);
        toneKnob.setBounds(315, 230, 90, 90);
        volumeKnob.setBounds(220, 130, 100, 100);
        
        driveToggleImage.setBounds(326, 421, 100, 100);
    }
    
    void buttonClicked(juce::Button* button) override
    {
        
    }
    
    
    void sliderValueChanged(juce::Slider* slider) override
    {
        
    }

private:
    ArtisianDSPAudioProcessor& audioProcessor;
    
    juce::TooltipWindow tooltipWindow;
    
    // Buttons, Sliders
    juce::ImageButton driveToggleImage;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> driveToggleAttachment;
    
    
    juce::Slider driveKnob,
                 toneKnob,
                 volumeKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> driveAttachment,
                                                                          toneAttachment,
                                                                          volumeAttachment;
    juce::Label driveLabel,
                toneLabel,
                volumeLabel;
    
    // GUI
    juce::Label driveSceneLabel;

};
