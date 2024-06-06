#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
//#include "Utils.h"

class Comp2Component : public juce::Component,
                       public juce::Button::Listener,
                       public juce::Slider::Listener
{
public:
    Comp2Component(ArtisianDSPAudioProcessor& processor) : audioProcessor(processor)
    {
        // Bypass Switch
        addAndMakeVisible(compToggleImage);
        compToggleImage.addListener(this);
        compToggleImage.setImages(false, true, true,
                                 juce::ImageCache::getFromMemory(BinaryData::fsup_png, BinaryData::fsup_pngSize), 1.0f, juce::Colours::transparentBlack,
                                 juce::ImageCache::getFromMemory(BinaryData::fsup_png, BinaryData::fsup_pngSize), 1.0f, juce::Colours::transparentBlack,
                                 juce::ImageCache::getFromMemory(BinaryData::fsdown_png, BinaryData::fsdown_pngSize), 1.0f, juce::Colours::transparentBlack);

        compToggleImage.setClickingTogglesState(true);
        compToggleAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "USING_COMP", compToggleImage);
        
        
        // Threshold
        addAndMakeVisible(thresholdKnob);
        thresholdKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
        thresholdKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 15);
        thresholdKnob.setRange(-100.0, 6.0);
        thresholdKnob.setValue(1.0);
        thresholdKnob.setTextValueSuffix(" dB");
        
        thresholdLabel.setText ("Threshold", juce::NotificationType::dontSendNotification);
        thresholdLabel.attachToComponent (&thresholdKnob, false);
        
        thresholdKnob.addListener(this);
        thresholdAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "COMP_THRES", thresholdKnob);
        
        // Attack
        addAndMakeVisible(attackKnob);
        attackKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
        attackKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 15);
        attackKnob.setRange(1.0, 100.0);
        attackKnob.setValue(50.0);
        attackKnob.setTextValueSuffix(" ms");
        attackLabel.setText ("Attack", juce::NotificationType::dontSendNotification);
        attackLabel.attachToComponent (&attackKnob, false);
        
        attackKnob.addListener(this);
        attackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "COMP_ATTACK", attackKnob);
        
        // Release
        addAndMakeVisible(releaseKnob);
        releaseKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
        releaseKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 15);
        releaseKnob.setRange(1.0, 100.0);
        releaseKnob.setValue(50.0);
        releaseKnob.setTextValueSuffix(" ms");
        
        releaseLabel.setText ("Release", juce::NotificationType::dontSendNotification);
        releaseLabel.attachToComponent (&releaseKnob, false);
        
        releaseKnob.addListener(this);
        releaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "COMP_RELEASE", releaseKnob);
        
        // Ratio
        addAndMakeVisible(ratioKnob);
        ratioKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
        ratioKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 15);
        ratioKnob.setRange(1.0, 8.0);
        ratioKnob.setValue(1.0);
        ratioKnob.setTextValueSuffix(": 1");
        
        ratioLabel.setText ("Ratio", juce::NotificationType::dontSendNotification);
        ratioLabel.attachToComponent (&ratioKnob, false);
        
        ratioKnob.addListener(this);
        ratioAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "COMP_RATIO", ratioKnob);
        
        // Level
        addAndMakeVisible(levelKnob);
        levelKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
        levelKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 15);
        levelKnob.setRange(-20.f, 10.f);
        levelKnob.setValue(0.f);
        levelKnob.setTextValueSuffix(" dB");
        
        levelLabel.setText ("Level", juce::NotificationType::dontSendNotification);
        levelLabel.attachToComponent (&levelKnob, false);
        
        levelKnob.addListener(this);
        levelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "COMP_LEVEL", levelKnob);
        
        
        compLabel.setFont(20.f);
        compLabel.setJustificationType(juce::Justification::centred);
        compLabel.setText("Compressor", juce::dontSendNotification);
        addAndMakeVisible(compLabel);
    }
//    ~Comp2Component();
    
    virtual void resized() override
    {
//        compLabel.setBounds(getLocalBounds());
        
        compToggleImage.setBounds(335, 410, 91, 91);
        
        thresholdKnob.setBounds(315, 140, 90, 90);
        
        attackKnob.setBounds(215, 140, 90, 90);
        
        releaseKnob.setBounds(215, 260, 90, 90);
        
        ratioKnob.setBounds(415, 140, 90, 90);
        
        levelKnob.setBounds(415, 260, 90, 90);
    }
    
    void buttonClicked(juce::Button* button) override
    {
    }
    
    virtual void sliderValueChanged (juce::Slider* slider) override
    {
    }
private:
    ArtisianDSPAudioProcessor& audioProcessor;
    
    // Bypass
    juce::ImageButton compToggleImage;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> compToggleAttachment;
    
    
    // Sliders
    juce::Slider thresholdKnob, attackKnob, releaseKnob, ratioKnob, levelKnob;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> thresholdAttachment,
                                                                          attackAttachment,
                                                                          releaseAttachment,
                                                                          ratioAttachment,
                                                                          levelAttachment;
    juce::Label thresholdLabel,
                attackLabel,
                releaseLabel,
                ratioLabel,
                levelLabel;
    
    juce::Label compLabel;
};
