#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Jb_knobs.h"

class Reverb5Component : public juce::Component,
                         public juce::Button::Listener,
                         public juce::Slider::Listener
{
public:
    Reverb5Component(ArtisianDSPAudioProcessor& processor) : audioProcessor(processor), tooltipWindow(this, 900)
    {
        // Reverb Image
        reverbSvg = juce::Drawable::createFromImageData(BinaryData::reverb_svg, BinaryData::reverb_svgSize);
        addAndMakeVisible(reverbSvg.get());
        
        
        // Bypass Led
        if (audioProcessor.usingVerb)
        {
            bypassLed.setImage(juce::ImageCache::getFromMemory(BinaryData::ledOn_png, BinaryData::ledOn_pngSize));
        }
        else
        {
            bypassLed.setImage(juce::ImageCache::getFromMemory(BinaryData::ledOff_png, BinaryData::ledOff_pngSize));
        }
        addAndMakeVisible(bypassLed);
        
        
        // Bypass
        addAndMakeVisible(reverbToggleImage);
        reverbToggleImage.addListener(this);
        reverbToggleImage.setImages(false, true, true,
                                    juce::ImageCache::getFromMemory(BinaryData::fsup_png, BinaryData::fsup_pngSize), 1.0f, juce::Colours::transparentBlack,
                                    juce::ImageCache::getFromMemory(BinaryData::fsup_png, BinaryData::fsup_pngSize), 1.0f, juce::Colours::transparentBlack,
                                    juce::ImageCache::getFromMemory(BinaryData::fsdown_png, BinaryData::fsdown_pngSize), 1.0f, juce::Colours::transparentBlack);
        reverbToggleImage.setClickingTogglesState(true);
        reverbToggleAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "USING_VERB", reverbToggleImage);
        reverbToggleImage.setTooltip("Turns the reverb On/Off.");
        
        
        // Size
        addAndMakeVisible(sizeKnob);
        sizeKnob.setLookAndFeel(&pedalLookAndFeel);
        sizeKnob.setTooltip("Adjusts the size of the room in which the reverb occurs. Increase for longer decay times, decrease for shorter ones.");
        sizeKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        
        sizeKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 15);
        sizeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "VERB_SIZE", sizeKnob);
        
        
        // Damping
        addAndMakeVisible(dampKnob);
        dampKnob.setLookAndFeel(&pedalLookAndFeel);
        dampKnob.setTooltip("Controls the rate at which high frequencies decay. Lower values result in a brighter sound.");
        dampKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        
        dampKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 15);
        dampAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "VERB_DAMPING", dampKnob);
        
        
        // Width
        addAndMakeVisible(widthKnob);
        widthKnob.setLookAndFeel(&pedalLookAndFeel);
        widthKnob.setTooltip("Controls the stereo width of the reverb. Increase for more ambiance.");
        widthKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        
        widthKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 15);
        widthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "VERB_WIDTH", widthKnob);
        
        // Blend
        addAndMakeVisible(dwKnob);
        dwKnob.setLookAndFeel(&pedalLookAndFeel);
        dwKnob.setTooltip("Adjusts the balance between the dry (original) and wet (reverberated) signals. Increase to emphasize the reverb effect, decrease to retain more of the original signal.");
        dwKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        
        dwKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 15);
        dwAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "VERB_DRYWET", dwKnob);
        
    }
//    ~Reverb5Component();
    
    virtual void resized() override
    {
        juce::Rectangle<int> pedalBounds;
        pedalBounds.setBounds(160, 170, reverbSvg->getWidth() * 1.5, reverbSvg->getHeight() * 1.5);
        reverbSvg->setTransformToFit(pedalBounds.toFloat(), juce::RectanglePlacement::stretchToFit);
        
        reverbToggleImage.setBounds(324, 392, 100, 100);
        
        // Indicator
        if (audioProcessor.usingVerb)
            bypassLed.setImage(juce::ImageCache::getFromMemory(BinaryData::ledOn_png, BinaryData::ledOn_pngSize));
        else
            bypassLed.setImage(juce::ImageCache::getFromMemory(BinaryData::ledOff_png, BinaryData::ledOff_pngSize));
        bypassLed.setBounds(340, 349, 40, 40);
        
        sizeKnob.setBounds(181, 271, 80, 95);
        dampKnob.setBounds(270, 191, 80, 95);
        widthKnob.setBounds(380, 191, 80, 95);
        dwKnob.setBounds(459, 275, 80, 95);
        
    }
    
    void buttonClicked(juce::Button* button) override
    {
    }
    
    void buttonStateChanged(juce::Button* button) override
    {
        if (button == &reverbToggleImage)
        {
            if (reverbToggleImage.getToggleState())
            {
                bypassLed.setImage(juce::ImageCache::getFromMemory(BinaryData::ledOn_png, BinaryData::ledOn_pngSize));
            }
            else
            {
                bypassLed.setImage(juce::ImageCache::getFromMemory(BinaryData::ledOff_png, BinaryData::ledOff_pngSize));
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
    
    // Bypass Light
    juce::ImageComponent bypassLed;
    
    // Pedal Image
    std::unique_ptr<juce::Drawable> reverbSvg;
    
    
    juce::ImageButton reverbToggleImage;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> reverbToggleAttachment;
    
    juce::Slider sizeKnob,
                 dampKnob,
                 widthKnob,
                 dwKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>  sizeAttachment,
                                                                           dampAttachment,
                                                                           widthAttachment,
                                                                           dwAttachment;
    
};
