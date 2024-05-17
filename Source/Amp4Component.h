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
        
        // Overdrive
        addAndMakeVisible(driveKnob);
        driveKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        driveKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 15);
        driveKnob.setTextValueSuffix(" Drive");
        driveKnob.setRange(0.f, 1.f);
        driveKnob.setValue(0.f);
        driveKnob.addListener(this);
        driveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "AMP_DRIVE", driveKnob);
        
        
        // Bass
        addAndMakeVisible(bassKnob);
        bassKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        bassKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 15);
        bassKnob.setTextValueSuffix(" Base");
        bassKnob.setRange(0.f, 2.f);
        bassKnob.setValue(0.f);
        bassKnob.addListener(this);
        bassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "AMP_BASS", bassKnob);
        
        // Mids
        addAndMakeVisible(midsKnob);
        midsKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        midsKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 15);
        midsKnob.setTextValueSuffix(" Mids");
        midsKnob.setRange(0.f, 2.f);
        midsKnob.setValue(1.f);
        midsKnob.addListener(this);
        midsAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "AMP_MIDS", midsKnob);
        
        // Treble
        addAndMakeVisible(trebleKnob);
        trebleKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        trebleKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 15);
        trebleKnob.setTextValueSuffix(" treble");
        trebleKnob.setRange(0.f, 2.f);
        trebleKnob.setValue(0.f);
        trebleKnob.addListener(this);
        trebleAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "AMP_HI", trebleKnob);
        
        
    }
//    ~Amp4Component();
    
    virtual void resized() override
    {
        
        ampLabel.setBounds(290, 350, 140, 50);
        
        ampToggleImage.setBounds(335, 450, 50, 50);
        
        driveKnob.setBounds(100, 300, 100, 100);
        
        bassKnob.setBounds(100, 130, 100, 100);
        
        midsKnob.setBounds(250, 130, 100, 100);
        
        trebleKnob.setBounds(400, 130, 100, 100);
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
    
    
    juce::Slider bassKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> bassAttachment;
    
    juce::Slider midsKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> midsAttachment;
    
    juce::Slider trebleKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> trebleAttachment;
    
    
    
    juce::Label ampLabel;
};
