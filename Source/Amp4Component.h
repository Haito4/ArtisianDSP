#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Jb_knobs.h"

class Amp4Component : public juce::Component,
                      public juce::Button::Listener,
                      public juce::Slider::Listener
{
public:
    Amp4Component(ArtisianDSPAudioProcessor& processor) : audioProcessor(processor), tooltipWindow(this, 900)
    {
        ampLabel.setFont(20.f);
        ampLabel.setJustificationType(juce::Justification::centred);
        ampLabel.setText("Amplifier", juce::dontSendNotification);
        addAndMakeVisible(ampLabel);
        
        
        // Amp Image
        ampSvg = juce::Drawable::createFromImageData(BinaryData::amplifier_svg, BinaryData::amplifier_svgSize);
        addAndMakeVisible(ampSvg.get());
        
        // Bypass Indicator
        if (audioProcessor.usingAmp)
        {
            bypassLed.setImage(juce::ImageCache::getFromMemory(BinaryData::redLedOn_png, BinaryData::redLedOn_pngSize));
        }
        else
        {
            bypassLed.setImage(juce::ImageCache::getFromMemory(BinaryData::redLedOff_png, BinaryData::redLedOff_pngSize));
        }
        addAndMakeVisible(bypassLed);
        
        // Bypass
        addAndMakeVisible(ampToggleImage);
        ampToggleImage.addListener(this);
        ampToggleImage.setImages(false, true, true,
                                 juce::ImageCache::getFromMemory(BinaryData::psdown_png, BinaryData::psdown_pngSize), 1.0f, juce::Colours::transparentBlack,
                                 juce::ImageCache::getFromMemory(BinaryData::psdown_png, BinaryData::psdown_pngSize), 1.0f, juce::Colours::transparentBlack,
                                 juce::ImageCache::getFromMemory(BinaryData::psup_png, BinaryData::psup_pngSize), 1.0f, juce::Colours::transparentBlack);

        ampToggleImage.setClickingTogglesState(true);
        ampToggleAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "USING_AMP", ampToggleImage);
        ampToggleImage.setTooltip("Turns the amplifier On/Off.");
    
        
        // Input Gain
        addAndMakeVisible(inputGainKnob);
        inputGainKnob.setTooltip("Adjusts the input signal level. Increase for more gain, decrease for a cleaner sound.");
        inputGainKnob.setLookAndFeel(&afxLookAndFeel);
        
        inputGainKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        inputGainKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
        inputGainKnob.setTextValueSuffix(" dB");
        inputGainKnob.setRange(0.f, 40.f);
        inputGainKnob.setValue(0.f);
        inputGainKnob.addListener(this);
        inputGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "AMP_INPUTGAIN", inputGainKnob);
        
        addAndMakeVisible(inputGainLabel);
        inputGainLabel.setText ("Input Gain", juce::dontSendNotification);
        inputGainLabel.attachToComponent(&inputGainKnob, false);
        
        
        // Tight
        addAndMakeVisible(tightToggleImage);
        tightToggleImage.addListener(this);
        tightToggleImage.setImages(false, true, true, juce::ImageCache::getFromMemory(BinaryData::dogreen_png, BinaryData::dogreen_pngSize), 0.5f, juce::Colours::green,
                                                      juce::ImageCache::getFromMemory(BinaryData::dohover_png, BinaryData::dohover_pngSize), 0.5f, juce::Colours::blue,
                                                      juce::ImageCache::getFromMemory(BinaryData::dored_png, BinaryData::dored_pngSize), 0.5f, juce::Colours::red);
        tightToggleImage.setClickingTogglesState(true);
        tightToggleAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "AMP_TIGHT", tightToggleImage);
        
        
        // Presence
        addAndMakeVisible(presenceKnob);
        presenceKnob.setTooltip("Adjusts the high-end clarity and sparkle. Increase for a sharper sound, decrease for a smoother one.");
        presenceKnob.setLookAndFeel(&afxLookAndFeel);
        
        presenceKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        presenceKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
        presenceKnob.setRange(0.5f, 1.5f);
        presenceKnob.setValue(1.f);
        presenceKnob.addListener(this);
        presenceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "AMP_PRESENCE", presenceKnob);
        
        addAndMakeVisible(presenceLabel);
        presenceLabel.setText ("Presence", juce::dontSendNotification);
        presenceLabel.attachToComponent(&presenceKnob, false);
        
        
        
        
        
        // Bass
        addAndMakeVisible(bassKnob);
        bassKnob.setTooltip("Controls the low-frequency response. Increase to add thickness, decrease for a tighter sound.");
        bassKnob.setLookAndFeel(&afxLookAndFeel);
        
        bassKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        bassKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
        bassKnob.setTextValueSuffix("");
        bassKnob.setRange(0.f, 2.f);
        bassKnob.setValue(0.f);
        bassKnob.addListener(this);
        bassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "AMP_BASS", bassKnob);
        
        addAndMakeVisible(bassLabel);
        bassLabel.setText ("Bass", juce::dontSendNotification);
        bassLabel.attachToComponent(&bassKnob, false);
        
        
        // Mids
        addAndMakeVisible(midsKnob);
        midsKnob.setTooltip("Controls the mid-frequency response. Increase to hear your guitar.");
        midsKnob.setLookAndFeel(&afxLookAndFeel);
        
        midsKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        midsKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
        midsKnob.setTextValueSuffix("");
        midsKnob.setRange(0.f, 2.f);
        midsKnob.setValue(1.f);
        midsKnob.addListener(this);
        midsAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "AMP_MIDS", midsKnob);
        
        addAndMakeVisible(midsLabel);
        midsLabel.setText ("Mids", juce::dontSendNotification);
        midsLabel.attachToComponent(&midsKnob, false);
        
        // Treble
        addAndMakeVisible(trebleKnob);
        trebleKnob.setTooltip("Controls the high-frequency response. Increase for more brightness and presence, decrease to reduce harshness.");
        trebleKnob.setLookAndFeel(&afxLookAndFeel);
        
        trebleKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        trebleKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
        trebleKnob.setTextValueSuffix("");
        trebleKnob.setRange(0.f, 2.f);
        trebleKnob.setValue(0.f);
        trebleKnob.addListener(this);
        trebleAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "AMP_HI", trebleKnob);
        
        addAndMakeVisible(trebleLabel);
        trebleLabel.setText ("Treble", juce::dontSendNotification);
        trebleLabel.attachToComponent(&trebleKnob, false);
        
        // Master Level
        addAndMakeVisible(masterKnob);
        masterKnob.setTooltip("Sets the final output volume level. Use it to match the amp's overall volume to the desired level.");
        masterKnob.setLookAndFeel(&afxLookAndFeel);
        
        masterKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        masterKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
        masterKnob.setRange(0.f, 2.f);
        masterKnob.setValue(1.f);
        masterKnob.addListener(this);
        masterAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "AMP_MASTER", masterKnob);
        
        addAndMakeVisible(masterLabel);
        masterLabel.setText ("Master Vol", juce::dontSendNotification);
        masterLabel.attachToComponent(&masterKnob, false);
        
        
        // Output Gain
        addAndMakeVisible(outputGainKnob);
        outputGainKnob.setTooltip("Controls the output level of the amplifier stage. Increase for more drive, decrease to reduce volume.");
        outputGainKnob.setLookAndFeel(&afxLookAndFeel);
        
        outputGainKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        outputGainKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
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
        ampSvg->setBounds(5, 245, 710, 260);
        
        // Bypass Indicator
        if (audioProcessor.usingAmp)
            bypassLed.setImage(juce::ImageCache::getFromMemory(BinaryData::redLedOn_png, BinaryData::redLedOn_pngSize));
        else
            bypassLed.setImage(juce::ImageCache::getFromMemory(BinaryData::redLedOff_png, BinaryData::redLedOff_pngSize));
        bypassLed.setBounds(24, 426, 40, 40);
        
        ampLabel.setBounds(290, 102, 190, 50);
        
        ampToggleImage.setBounds(645, 418, 50, 61);
//        tightToggleImage.setBounds(640, 250, 50, 50);
        
        
        inputGainKnob.setBounds(55, 420, 80, 80);
        
        
        bassKnob.setBounds(155, 420, 80, 80);
        midsKnob.setBounds(235, 420, 80, 80);
        trebleKnob.setBounds(315, 420, 80, 80);
        
        presenceKnob.setBounds(415, 420, 80, 80);
        
        outputGainKnob.setBounds(495, 420, 80, 80);
        
        masterKnob.setBounds(575, 420, 80, 80);
    }
    
    void buttonClicked(juce::Button* button) override
    {
    }
    
    void buttonStateChanged(juce::Button* button) override
    {
        if (button == &ampToggleImage)
        {
            if (ampToggleImage.getToggleState())
            {
                bypassLed.setImage(juce::ImageCache::getFromMemory(BinaryData::redLedOn_png, BinaryData::redLedOn_pngSize));
            }
            else
            {
                bypassLed.setImage(juce::ImageCache::getFromMemory(BinaryData::redLedOff_png, BinaryData::redLedOff_pngSize));
            }
        }
    }
    
    
    void sliderValueChanged(juce::Slider* slider) override
    {
        
    }
    
private:
    ArtisianDSPAudioProcessor& audioProcessor;
    AfxLookAndFeel afxLookAndFeel;
    
    juce::TooltipWindow tooltipWindow;
    
    
//    juce::Image ampSVG = BinaryData::amplifier_svg;
    std::unique_ptr<juce::Drawable> ampSvg;
    
    // Bypass Led
    juce::ImageComponent bypassLed;
    
    
    
    juce::Image ampSVG = juce::ImageCache::getFromMemory(BinaryData::amplifier_svg, BinaryData::amplifier_svgSize);
    
    juce::ImageButton ampToggleImage;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> ampToggleAttachment;
    
    juce::ImageButton tightToggleImage;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> tightToggleAttachment;
    
    
    //-------
    
    juce::Slider inputGainKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> inputGainAttachment;
    
    //-------
    
    juce::Slider presenceKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> presenceAttachment;
    
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
    
    
    juce::Label inputGainLabel, presenceLabel, bassLabel, midsLabel, trebleLabel, masterLabel, outputGainLabel;
    
    
    juce::Label ampLabel;
};
