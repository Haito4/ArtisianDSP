#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

class Gate1Component : public juce::Component,
                       public juce::Button::Listener,
                       public juce::Slider::Listener
{
public:
    Gate1Component(ArtisianDSPAudioProcessor& processor)
     : audioProcessor(processor)
    {
        // Bypass Switch
        addAndMakeVisible(gateToggleImage);
        gateToggleImage.addListener(this);
        gateToggleImage.setImages(false, true, true,
                                 juce::ImageCache::getFromMemory(BinaryData::fsup_png, BinaryData::fsup_pngSize), 1.0f, juce::Colours::transparentBlack,
                                 juce::ImageCache::getFromMemory(BinaryData::fsup_png, BinaryData::fsup_pngSize), 1.0f, juce::Colours::transparentBlack,
                                 juce::ImageCache::getFromMemory(BinaryData::fsdown_png, BinaryData::fsdown_pngSize), 1.0f, juce::Colours::transparentBlack);

        gateToggleImage.setClickingTogglesState(true);
        gateToggleAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "USING_GATE", gateToggleImage);
        
        
        helloLabel.setFont(20.0f);
        helloLabel.setJustificationType(juce::Justification::centred);
        helloLabel.setText("Noise Gate", juce::dontSendNotification);
        addAndMakeVisible(helloLabel);
        
        
        thresholdSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
        thresholdSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 15);
        thresholdSlider.setRange(-100.0, 6.0);
        thresholdSlider.setValue(1.0);
        thresholdSlider.setTextValueSuffix(" dB");
        addAndMakeVisible(thresholdSlider);
        thresholdLabel.setText ("Threshold", juce::NotificationType::dontSendNotification);
        thresholdLabel.attachToComponent (&thresholdSlider, false);
        
        attackSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
        attackSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 15);
        attackSlider.setRange(1.0, 100.0);
        attackSlider.setValue(50.0);
        attackSlider.setTextValueSuffix(" ms");
        addAndMakeVisible(attackSlider);
        attackLabel.setText ("Attack", juce::NotificationType::dontSendNotification);
        attackLabel.attachToComponent (&attackSlider, false);
        
        releaseSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
        releaseSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 15);
        releaseSlider.setRange(1.0, 100.0);
        releaseSlider.setValue(50.0);
        releaseSlider.setTextValueSuffix(" ms");
        addAndMakeVisible(releaseSlider);
        releaseLabel.setText ("Release", juce::NotificationType::dontSendNotification);
        releaseLabel.attachToComponent (&releaseSlider, false);
        
        
        // APVTS Attachments
        
        thresholdSlider.addListener(this);
        thresholdAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "THRESHOLD", thresholdSlider);

        attackSlider.addListener(this);
        attackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "ATTACK", attackSlider);
        
        releaseSlider.addListener(this);
        releaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "RELEASE", releaseSlider);
        
    }
//    ~Gate1Component();

    virtual void resized() override
    {
        
        helloLabel.setBounds(280, 380, 160, 42);
        gateToggleImage.setBounds(332, 420, 100, 100);
        
        thresholdSlider.setBounds(250, 150, 100, 100);
        attackSlider.setBounds(370, 150, 100, 100);
        releaseSlider.setBounds(370, 270, 100, 100);
        
    }
        
    void buttonClicked(juce::Button* button) override
    {
        
    };

    void sliderValueChanged(juce::Slider* slider) override
    {
    
    }
    
private:
    ArtisianDSPAudioProcessor& audioProcessor;
    
    juce::Label helloLabel;
    
    juce::ImageButton gateToggleImage;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> gateToggleAttachment;
    
    juce::Slider thresholdSlider,
                 attackSlider,
                 releaseSlider;
    juce::Label thresholdLabel,
                attackLabel,
                releaseLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> thresholdAttachment,
                                                                          attackAttachment,
                                                                          releaseAttachment;
};
