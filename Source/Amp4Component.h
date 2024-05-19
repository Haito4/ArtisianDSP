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
        
        
        
        
        // Preamp
        addAndMakeVisible(driveKnob);
        driveKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        driveKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 15);
        driveKnob.setTextValueSuffix(" Drive");
        driveKnob.setRange(0.f, 1.f);
        driveKnob.setValue(0.f);
        driveKnob.addListener(this);
        driveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "AMP_DRIVE", driveKnob);
        
        addAndMakeVisible(driveLabel);
        driveLabel.setText ("Gain", juce::dontSendNotification);
        driveLabel.attachToComponent(&driveKnob, false);
        
        
        // Bass
        addAndMakeVisible(bassKnob);
        bassKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        bassKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 15);
        bassKnob.setTextValueSuffix(" Base");
        bassKnob.setRange(0.f, 2.f);
        bassKnob.setValue(0.f);
        bassKnob.addListener(this);
        bassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "AMP_BASS", bassKnob);
        
        addAndMakeVisible(bassLabel);
        bassLabel.setText ("Bass", juce::dontSendNotification);
        bassLabel.attachToComponent(&bassKnob, false);
        
        
        // Mids
        addAndMakeVisible(midsKnob);
        midsKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        midsKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 15);
        midsKnob.setTextValueSuffix(" Mids");
        midsKnob.setRange(0.f, 2.f);
        midsKnob.setValue(1.f);
        midsKnob.addListener(this);
        midsAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "AMP_MIDS", midsKnob);
        
        addAndMakeVisible(midsLabel);
        midsLabel.setText ("Mids", juce::dontSendNotification);
        midsLabel.attachToComponent(&midsKnob, false);
        
        // Treble
        addAndMakeVisible(trebleKnob);
        trebleKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        trebleKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 15);
        trebleKnob.setTextValueSuffix(" treble");
        trebleKnob.setRange(0.f, 2.f);
        trebleKnob.setValue(0.f);
        trebleKnob.addListener(this);
        trebleAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "AMP_HI", trebleKnob);
        
        addAndMakeVisible(trebleLabel);
        trebleLabel.setText ("Treble", juce::dontSendNotification);
        trebleLabel.attachToComponent(&trebleKnob, false);
        
        // Master Level
        addAndMakeVisible(masterKnob);
        masterKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        masterKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 15);
        masterKnob.setTextValueSuffix(" Master");
        masterKnob.setRange(0.f, 2.f);
        masterKnob.setValue(1.f);
        masterKnob.addListener(this);
        masterAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "AMP_MASTER", masterKnob);
        
        addAndMakeVisible(masterLabel);
        masterLabel.setText ("Master Vol", juce::dontSendNotification);
        masterLabel.attachToComponent(&masterKnob, false);
        
    }
//    ~Amp4Component();
    
    virtual void resized() override
    {
        
        ampLabel.setBounds(290, 102, 190, 50);
        
        ampToggleImage.setBounds(640, 305, 50, 50);
        
        driveKnob.setBounds(102, 280, 100, 100);
        
        bassKnob.setBounds(206, 280, 100, 100);
        midsKnob.setBounds(310, 280, 100, 100);
        trebleKnob.setBounds(414, 280, 100, 100);
        
        masterKnob.setBounds(518, 280, 100, 100);
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
    
    
    
    juce::Slider masterKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> masterAttachment;
    
    
    juce::Slider driveKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> driveAttachment;
    
    juce::Slider bassKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> bassAttachment;
    
    juce::Slider midsKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> midsAttachment;
    
    juce::Slider trebleKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> trebleAttachment;
    
    juce::Slider resonanceKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> resonanceAttachment;
    
    juce::Slider presenceKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> presenceAttachment;
    
    
    juce::Label driveLabel, bassLabel, midsLabel, trebleLabel, masterLabel;
    
    
    juce::Label ampLabel;
};
