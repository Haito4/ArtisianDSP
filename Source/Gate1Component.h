#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

class Gate1Component : public juce::Component,
                       public juce::Button::Listener,
                       public juce::Slider::Listener
{
public:
    Gate1Component(ArtisianDSPAudioProcessor& processor)
     : audioProcessor(processor), tooltipWindow(this, 900)
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
        gateToggleImage.setTooltip("Turns the gate On/Off.");
        
        
        helloLabel.setFont(20.0f);
        helloLabel.setJustificationType(juce::Justification::centred);
        helloLabel.setText("Noise Gate", juce::dontSendNotification);
        addAndMakeVisible(helloLabel);
        
        
        addAndMakeVisible(thresholdSlider);
        thresholdSlider.setTooltip("Sets the level below which the gate closes. Signals quieter than this threshold will be affected by the gate.");
        thresholdSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
        thresholdSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 15);
        thresholdSlider.setRange(-100.0, 6.0);
        thresholdSlider.setValue(1.0);
        thresholdSlider.setTextValueSuffix(" dB");
        
        thresholdLabel.setText ("Threshold", juce::NotificationType::dontSendNotification);
        thresholdLabel.attachToComponent (&thresholdSlider, false);
        
        
        addAndMakeVisible(attackSlider);
        attackSlider.setTooltip("Controls how quickly the gate opens when the input signal exceeds the threshold. Lower values result in faster opening times.");
        attackSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
        attackSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 15);
        attackSlider.setRange(1.0, 100.0);
        attackSlider.setValue(50.0);
        attackSlider.setTextValueSuffix(" ms");
        
        attackLabel.setText ("Attack", juce::NotificationType::dontSendNotification);
        attackLabel.attachToComponent (&attackSlider, false);
        
        addAndMakeVisible(releaseSlider);
        releaseSlider.setTooltip("Controls how quickly the gate closes after the input signal falls below the threshold. Shorter release times result in faster closure.");
        releaseSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
        releaseSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 15);
        releaseSlider.setRange(1.0, 100.0);
        releaseSlider.setValue(50.0);
        releaseSlider.setTextValueSuffix(" ms");
        
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
    
    
    juce::TooltipWindow tooltipWindow;
    
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
