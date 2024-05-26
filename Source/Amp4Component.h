#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Jb_knobs.h"

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
        ampToggleImage.setImages(false, true, true,
                                 juce::ImageCache::getFromMemory(BinaryData::psdown_png, BinaryData::psdown_pngSize), 1.0f, juce::Colours::transparentBlack,
                                 juce::ImageCache::getFromMemory(BinaryData::psdown_png, BinaryData::psdown_pngSize), 1.0f, juce::Colours::transparentBlack,
                                 juce::ImageCache::getFromMemory(BinaryData::psup_png, BinaryData::psup_pngSize), 1.0f, juce::Colours::transparentBlack);

        ampToggleImage.setClickingTogglesState(true);
        ampToggleAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "USING_AMP", ampToggleImage);
        
    
        
        // Input Gain
        addAndMakeVisible(inputGainKnob);
        inputGainKnob.setLookAndFeel(&afxLookAndFeel);
        
        inputGainKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        inputGainKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 20);
        inputGainKnob.setTextValueSuffix(" dB");
        inputGainKnob.setRange(0.f, 40.f);
        inputGainKnob.setValue(0.f);
        inputGainKnob.addListener(this);
        inputGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "AMP_INPUTGAIN", inputGainKnob);
        
        addAndMakeVisible(inputGainLabel);
        inputGainLabel.setText ("Input Gain", juce::dontSendNotification);
        inputGainLabel.attachToComponent(&inputGainKnob, false);
        
        
        // Preamp
        addAndMakeVisible(driveKnob);
        driveKnob.setLookAndFeel(&afxLookAndFeel);
        
        driveKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        driveKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 20);
        driveKnob.setTextValueSuffix(" %");
        driveKnob.setRange(0.f, 1.f);
        driveKnob.setValue(0.f);
        driveKnob.addListener(this);
        driveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "AMP_DRIVE", driveKnob);
        
        addAndMakeVisible(driveLabel);
        driveLabel.setText ("Drive", juce::dontSendNotification);
        driveLabel.attachToComponent(&driveKnob, false);
        
        
        // Tight
        addAndMakeVisible(tightToggleImage);
        tightToggleImage.addListener(this);
        tightToggleImage.setImages(false, true, true, juce::ImageCache::getFromMemory(BinaryData::dogreen_png, BinaryData::dogreen_pngSize), 0.5f, juce::Colours::green,
                                                      juce::ImageCache::getFromMemory(BinaryData::dohover_png, BinaryData::dohover_pngSize), 0.5f, juce::Colours::blue,
                                                      juce::ImageCache::getFromMemory(BinaryData::dored_png, BinaryData::dored_pngSize), 0.5f, juce::Colours::red);
        tightToggleImage.setClickingTogglesState(true);
        tightToggleAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "AMP_TIGHT", tightToggleImage);
        
        
        // Resonance
        addAndMakeVisible(resonanceKnob);
        resonanceKnob.setLookAndFeel(&afxLookAndFeel);
        
        resonanceKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        resonanceKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 20);
        resonanceKnob.setTextValueSuffix(" %");
        resonanceKnob.setRange(1.f, 10.f);
        resonanceKnob.setValue(5.f);
        resonanceKnob.addListener(this);
        resonanceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "AMP_RESONANCE", resonanceKnob);
        
        addAndMakeVisible(resonanceLabel);
        resonanceLabel.setText ("Resonance", juce::dontSendNotification);
        resonanceLabel.attachToComponent(&resonanceKnob, false);
        
        
        // Presence
        addAndMakeVisible(presenceKnob);
        presenceKnob.setLookAndFeel(&afxLookAndFeel);
        
        presenceKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        presenceKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 20);
        presenceKnob.setTextValueSuffix(" %");
        presenceKnob.setRange(0.5f, 1.5f);
        presenceKnob.setValue(1.f);
        presenceKnob.addListener(this);
        presenceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "AMP_PRESENCE", presenceKnob);
        
        addAndMakeVisible(presenceLabel);
        presenceLabel.setText ("Presence", juce::dontSendNotification);
        presenceLabel.attachToComponent(&presenceKnob, false);
        
        
        
        
        
        // Bass
        addAndMakeVisible(bassKnob);
        bassKnob.setLookAndFeel(&afxLookAndFeel);
        
        bassKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        bassKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 20);
        bassKnob.setTextValueSuffix(" Bass");
        bassKnob.setRange(0.f, 2.f);
        bassKnob.setValue(0.f);
        bassKnob.addListener(this);
        bassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "AMP_BASS", bassKnob);
        
        addAndMakeVisible(bassLabel);
        bassLabel.setText ("Bass", juce::dontSendNotification);
        bassLabel.attachToComponent(&bassKnob, false);
        
        
        // Mids
        addAndMakeVisible(midsKnob);
        midsKnob.setLookAndFeel(&afxLookAndFeel);
        
        midsKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        midsKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 20);
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
        trebleKnob.setLookAndFeel(&afxLookAndFeel);
        
        trebleKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        trebleKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 20);
        trebleKnob.setTextValueSuffix(" Treble");
        trebleKnob.setRange(0.f, 2.f);
        trebleKnob.setValue(0.f);
        trebleKnob.addListener(this);
        trebleAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "AMP_HI", trebleKnob);
        
        addAndMakeVisible(trebleLabel);
        trebleLabel.setText ("Treble", juce::dontSendNotification);
        trebleLabel.attachToComponent(&trebleKnob, false);
        
        // Master Level
        addAndMakeVisible(masterKnob);
        masterKnob.setLookAndFeel(&afxLookAndFeel);
        
        masterKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        masterKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 20);
        masterKnob.setTextValueSuffix(" %");
        masterKnob.setRange(0.f, 2.f);
        masterKnob.setValue(1.f);
        masterKnob.addListener(this);
        masterAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "AMP_MASTER", masterKnob);
        
        addAndMakeVisible(masterLabel);
        masterLabel.setText ("Master Vol", juce::dontSendNotification);
        masterLabel.attachToComponent(&masterKnob, false);
        
        
        // Output Gain
        addAndMakeVisible(outputGainKnob);
        outputGainKnob.setLookAndFeel(&afxLookAndFeel);
        
        outputGainKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        outputGainKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 20);
        outputGainKnob.setTextValueSuffix(" dB");
        outputGainKnob.setRange(-10.f, 10.f);
        outputGainKnob.setValue(0.f);
        outputGainKnob.addListener(this);
        outputGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "AMP_OUTPUTGAIN", outputGainKnob);
        
        addAndMakeVisible(outputGainLabel);
        outputGainLabel.setText ("Output Gain", juce::dontSendNotification);
        outputGainLabel.attachToComponent(&outputGainKnob, false);
        
        
    }
//    ~Amp4Component();
    
    virtual void resized() override
    {
        
        ampLabel.setBounds(290, 102, 190, 50);
        
        ampToggleImage.setBounds(640, 305, 50, 50);
        tightToggleImage.setBounds(640, 250, 50, 50);
        
        
        inputGainKnob.setBounds(102, 280, 100, 100);
        
//        driveKnob.setBounds(102, 280, 100, 100);
        
        
        resonanceKnob.setBounds(206, 180, 100, 100);
        presenceKnob.setBounds(310, 180, 100, 100);
        
        
        bassKnob.setBounds(206, 280, 100, 100);
        midsKnob.setBounds(310, 280, 100, 100);
        trebleKnob.setBounds(414, 280, 100, 100);
        
        masterKnob.setBounds(518, 280, 100, 100);
        outputGainKnob.setBounds(518, 180, 100, 100);
    }
    
    void buttonClicked(juce::Button* button) override
    {
    
    }
    
    void sliderValueChanged(juce::Slider* slider) override
    {
        
    }
    
private:
    ArtisianDSPAudioProcessor& audioProcessor;
    AfxLookAndFeel afxLookAndFeel;
    
    juce::ImageButton ampToggleImage;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> ampToggleAttachment;
    
    juce::ImageButton tightToggleImage;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> tightToggleAttachment;
    
    
    //-------
    
    juce::Slider inputGainKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> inputGainAttachment;
    
    juce::Slider driveKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> driveAttachment;
    
    //-------
    
    juce::Slider presenceKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> presenceAttachment;
    
    juce::Slider resonanceKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> resonanceAttachment;
    
    
    juce::Slider bassKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> bassAttachment;
    
    juce::Slider midsKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> midsAttachment;
    
    juce::Slider trebleKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> trebleAttachment;
    
    
    //-------
    
    juce::Slider masterKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> masterAttachment;
    
    juce::Slider outputGainKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> outputGainAttachment;
    
    
    juce::Label inputGainLabel, driveLabel, resonanceLabel, presenceLabel, bassLabel, midsLabel, trebleLabel, masterLabel, outputGainLabel;
    
    
    juce::Label ampLabel;
};
