#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "Jb_knobs.h"

class Gate1Component : public juce::Component,
                       public juce::Button::Listener,
                       public juce::Slider::Listener
{
public:
    Gate1Component(ArtisianDSPAudioProcessor& processor)
     : audioProcessor(processor), tooltipWindow(this, 900)
    {
        // Gate Image
        gateSvg = juce::Drawable::createFromImageData(BinaryData::noisegate_svg, BinaryData::noisegate_svgSize);
        addAndMakeVisible(gateSvg.get());
        
        // Bypass Indicator
        if (audioProcessor.usingGate)
        {
            bypassLed.setImage(juce::ImageCache::getFromMemory(BinaryData::redLedOn_png, BinaryData::redLedOn_pngSize));
        }
        else
        {
            bypassLed.setImage(juce::ImageCache::getFromMemory(BinaryData::redLedOff_png, BinaryData::redLedOff_pngSize));
        }
        
        addAndMakeVisible(bypassLed);
        
        
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
        
        
        // Threshold
        addAndMakeVisible(thresholdSlider);
        thresholdSlider.setLookAndFeel(&pedalLookAndFeel);
        thresholdSlider.setTooltip("Sets the level below which the gate closes. Signals quieter than this threshold will be affected by the gate.");
        thresholdSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
        thresholdSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 15);
        thresholdSlider.setRange(-100.0, 6.0);
        thresholdSlider.setValue(1.0);
        thresholdSlider.setTextValueSuffix(" dB");
        thresholdSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::black);
        
        // Attack
        addAndMakeVisible(attackSlider);
        attackSlider.setLookAndFeel(&pedalLookAndFeel);
        attackSlider.setTooltip("Controls how quickly the gate opens when the input signal exceeds the threshold. Lower values result in faster opening times.");
        attackSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
        attackSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 15);
        attackSlider.setRange(1.0, 100.0);
        attackSlider.setValue(50.0);
        attackSlider.setTextValueSuffix(" ms");
        attackSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::black);
        
        // Release
        addAndMakeVisible(releaseSlider);
        releaseSlider.setLookAndFeel(&pedalLookAndFeel);
        releaseSlider.setTooltip("Controls how quickly the gate closes after the input signal falls below the threshold. Shorter release times result in faster closure.");
        releaseSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
        releaseSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 15);
        releaseSlider.setRange(1.0, 100.0);
        releaseSlider.setValue(50.0);
        releaseSlider.setTextValueSuffix(" ms");
        releaseSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::black);
        
        
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
        // Pedal Image
        juce::Rectangle<int> pedalBounds;
        pedalBounds.setBounds(250, 115, gateSvg->getWidth() * 1.85, gateSvg->getHeight() * 1.85);
        gateSvg->setTransformToFit(pedalBounds.toFloat(), juce::RectanglePlacement::stretchToFit);
        
        // Bypass Indicator
        if (audioProcessor.usingGate)
            bypassLed.setImage(juce::ImageCache::getFromMemory(BinaryData::redLedOn_png, BinaryData::redLedOn_pngSize));
        else
            bypassLed.setImage(juce::ImageCache::getFromMemory(BinaryData::redLedOff_png, BinaryData::redLedOff_pngSize));
        bypassLed.setBounds(340, 360, 40, 40);
        
        // Bypass Button
        gateToggleImage.setBounds(324, 399, 100, 100);
        
        // Sliders
        thresholdSlider.setBounds(324, 203, 73, 90);
        attackSlider.setBounds(260, 135, 62, 77);
        releaseSlider.setBounds(399, 135, 62, 77);
        
    }
        
    void buttonClicked(juce::Button* button) override
    {
    }

    void buttonStateChanged(juce::Button* button) override
    {
        if (button == &gateToggleImage)
        {
            if (gateToggleImage.getToggleState())
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
    
    // Pedal Image
    std::unique_ptr<juce::Drawable> gateSvg;
    
    // Bypass Led
    juce::ImageComponent bypassLed;
    
    
    juce::TooltipWindow tooltipWindow;
    
    juce::ImageButton gateToggleImage;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> gateToggleAttachment;
    
    juce::Slider thresholdSlider,
                 attackSlider,
                 releaseSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> thresholdAttachment,
                                                                          attackAttachment,
                                                                          releaseAttachment;
};
