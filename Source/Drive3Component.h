#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "Jb_knobs.h"

class Drive3Component : public juce::Component,
                        public juce::Button::Listener,
                        public juce::Slider::Listener
{
public:
    Drive3Component(ArtisianDSPAudioProcessor& processor) : audioProcessor(processor), tooltipWindow(this, 900)
    {
        // Overdrive Image
        odSvg = juce::Drawable::createFromImageData(BinaryData::tubescreamer_svg, BinaryData::tubescreamer_svgSize);
        addAndMakeVisible(odSvg.get());
        
        // Bypass Indicator
        if (audioProcessor.usingTS)
        {
            bypassLed.setImage(juce::ImageCache::getFromMemory(BinaryData::redLedOn_png, BinaryData::redLedOn_pngSize));
        }
        else
        {
            bypassLed.setImage(juce::ImageCache::getFromMemory(BinaryData::redLedOff_png, BinaryData::redLedOff_pngSize));
        }
        
        addAndMakeVisible(bypassLed);
        
        
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
        
        
        // Drive
        addAndMakeVisible(driveKnob);
        driveKnob.setLookAndFeel(&pedalLookAndFeel);
        driveKnob.setTooltip("Adjusts the amount of distortion applied to the signal. Higher values result in more aggressive distortion.");
        driveKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        driveKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 15);
        driveKnob.setRange(0.1f, 200.f);
        driveKnob.setValue(0.5);
        driveKnob.setColour(juce::Slider::textBoxTextColourId, juce::Colours::black);
        
        driveKnob.addListener(this);
        driveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "TS_DRIVE", driveKnob);
        
        
        // Tone
        addAndMakeVisible(toneKnob);
        toneKnob.setLookAndFeel(&pedalLookAndFeel);
        toneKnob.setTooltip("Alters the starting point of the low-cut filter. Higher values result in a tighter, brighter sound, while lower values result in warmer, muddier tone.");
        toneKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
        toneKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 15);
        toneKnob.setTextValueSuffix(" Hz");
        toneKnob.setRange(20.f, 700.f);
        toneKnob.setValue(20.f);
        toneKnob.setColour(juce::Slider::textBoxTextColourId, juce::Colours::black);
        
        toneKnob.addListener(this);
        toneAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "TS_TONE", toneKnob);
        
        
        // Level
        addAndMakeVisible(volumeKnob);
        volumeKnob.setLookAndFeel(&pedalLookAndFeel);
        volumeKnob.setTooltip("Adjusts the overall output level of the affected signal.");
        volumeKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        volumeKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 15);
        volumeKnob.setTextValueSuffix(" %");
        volumeKnob.setRange(0.f, 1.f);
        volumeKnob.setValue(1.f);
        volumeKnob.setColour(juce::Slider::textBoxTextColourId, juce::Colours::black);
        
        volumeKnob.addListener(this);
        volumeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "TS_LEVEL", volumeKnob);
    }
    
//    ~Drive3Component();
    
    virtual void resized() override
    {
        // Pedal Image
        juce::Rectangle<int> pedalBounds;
        pedalBounds.setBounds(250, 115, odSvg->getWidth() * 1.85, odSvg->getHeight() * 1.85);
        odSvg->setTransformToFit(pedalBounds.toFloat(), juce::RectanglePlacement::stretchToFit);
        
        // Bypass Indicator
        if (audioProcessor.usingTS)
            bypassLed.setImage(juce::ImageCache::getFromMemory(BinaryData::redLedOn_png, BinaryData::redLedOn_pngSize));
        else
            bypassLed.setImage(juce::ImageCache::getFromMemory(BinaryData::redLedOff_png, BinaryData::redLedOff_pngSize));
        bypassLed.setBounds(340, 185, 40, 40);
        
        // Bypass Button
        driveToggleImage.setBounds(324, 381, 100, 100);
        
        // Sliders
        volumeKnob.setBounds(257, 149, 85, 87);
        driveKnob.setBounds(377, 149, 85, 87);
        
        toneKnob.setBounds(327, 229, 67, 76);
    }
    
    void buttonClicked(juce::Button* button) override
    {
    }
    
    void buttonStateChanged(juce::Button* button) override
    {
        if (button == &driveToggleImage)
        {
            if (driveToggleImage.getToggleState())
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
    PedalLookAndFeel pedalLookAndFeel;
    
    juce::TooltipWindow tooltipWindow;
    
    // Pedal Image
    std::unique_ptr<juce::Drawable> odSvg;
    
    // Bypass Led
    juce::ImageComponent bypassLed;
    
    // Buttons, Sliders
    juce::ImageButton driveToggleImage;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> driveToggleAttachment;
    
    
    juce::Slider driveKnob,
                 toneKnob,
                 volumeKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> driveAttachment,
                                                                          toneAttachment,
                                                                          volumeAttachment;

};
