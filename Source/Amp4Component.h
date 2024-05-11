#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class Amp4Component : public juce::Component,
                      public juce::Button::Listener,
                      public juce::Slider::Listener
{
public:
    Amp4Component(ArtisianDSPAudioProcessor& processor) : audioProcessor(processor)
    {
        ampLabel.setFont(20.f);
        ampLabel.setJustificationType(juce::Justification::centred);
        ampLabel.setText("Amplifier", juce::dontSendNotification);
        addAndMakeVisible(ampLabel);
        
        // Bypass
        addAndMakeVisible(ampToggleImage);
        ampToggleImage.addListener(this);
        ampToggleImage.setImages(false, true, true, juce::ImageCache::getFromMemory(BinaryData::dogreen_png, BinaryData::dogreen_pngSize), 0.5f, juce::Colours::green,
                                                      juce::ImageCache::getFromMemory(BinaryData::dohover_png, BinaryData::dohover_pngSize), 0.5f, juce::Colours::blue,
                                                      juce::ImageCache::getFromMemory(BinaryData::dored_png, BinaryData::dored_pngSize), 0.5f, juce::Colours::red);
        ampToggleImage.setClickingTogglesState(true);
        ampToggleAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "USING_AMP", ampToggleImage);
        
        // Drive
        addAndMakeVisible(driveKnob);
        driveKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        driveKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 15);
        driveKnob.setRange(0.0f, 100.f);
        driveKnob.setValue(0.5);
        driveKnob.addListener(this);
        driveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "AMP_DRIVE", driveKnob);
        
        
        
    }
//    ~Amp4Component();
    
    virtual void resized() override
    {
        
        ampLabel.setBounds(290, 350, 140, 50);
        
        ampToggleImage.setBounds(335, 450, 50, 50);
        
        driveKnob.setBounds(400, 130, 100, 100);
    }
    
    void buttonClicked(juce::Button* button) override
    {
    
    }
    
    void sliderValueChanged(juce::Slider* slider) override
    {
        
    }
    
private:
    ArtisianDSPAudioProcessor& audioProcessor;
    
    juce::ImageButton ampToggleImage;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> ampToggleAttachment;
    
    juce::Slider driveKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> driveAttachment;
    
    juce::Label ampLabel;
};
