#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Jb_knobs.h"

class Comp2Component : public juce::Component,
                       public juce::Button::Listener,
                       public juce::Slider::Listener,
                       public juce::ValueTree::Listener
{
public:
    Comp2Component(ArtisianDSPAudioProcessor& processor) : audioProcessor(processor), tooltipWindow(this, 900)
    {
        // Comp Image
        compSvg = juce::Drawable::createFromImageData(BinaryData::compressor_svg, BinaryData::compressor_svgSize);
        addAndMakeVisible(compSvg.get());
        
        // Bypass
        if (audioProcessor.usingComp)
        {
            bypassLed.setImage(juce::ImageCache::getFromMemory(BinaryData::ledOn_png, BinaryData::ledOn_pngSize));
        }
        else
        {
            bypassLed.setImage(juce::ImageCache::getFromMemory(BinaryData::ledOff_png, BinaryData::ledOff_pngSize));
        }
        
        addAndMakeVisible(bypassLed);
        
        
        // Bypass Switch
        addAndMakeVisible(compToggleImage);
        compToggleImage.addListener(this);
        compToggleImage.setImages(false, true, true,
                                 juce::ImageCache::getFromMemory(BinaryData::fsup_png, BinaryData::fsup_pngSize), 1.0f, juce::Colours::transparentBlack,
                                 juce::ImageCache::getFromMemory(BinaryData::fsup_png, BinaryData::fsup_pngSize), 1.0f, juce::Colours::transparentBlack,
                                 juce::ImageCache::getFromMemory(BinaryData::fsdown_png, BinaryData::fsdown_pngSize), 1.0f, juce::Colours::transparentBlack);

        compToggleImage.setClickingTogglesState(true);
        compToggleAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "USING_COMP", compToggleImage);
        compToggleImage.setTooltip("Turns the compressor On/Off.");
        
        
        // Threshold
        addAndMakeVisible(thresholdKnob);
        thresholdKnob.setTooltip("Sets the level above which compression begins. Signals louder than this threshold will be reduced in volume.");
        thresholdKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
        thresholdKnob.setLookAndFeel(&pedalLookAndFeel);
        
        thresholdKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 15);
        thresholdKnob.setRange(-100.0, 6.0);
        thresholdKnob.setValue(1.0);
        thresholdKnob.setTextValueSuffix(" dB");
        
        thresholdKnob.addListener(this);
        thresholdAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "COMP_THRES", thresholdKnob);
        
        // Attack
        addAndMakeVisible(attackKnob);
        attackKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
        attackKnob.setTooltip("Adjusts how quickly the compressor responds to incoming signals. Lower values result in faster response times.");
        attackKnob.setLookAndFeel(&pedalLookAndFeel);
        
        attackKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 15);
        attackKnob.setRange(1.0, 100.0);
        attackKnob.setValue(50.0);
        attackKnob.setTextValueSuffix(" ms");
        
        attackKnob.addListener(this);
        attackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "COMP_ATTACK", attackKnob);
        
        // Release
        addAndMakeVisible(releaseKnob);
        releaseKnob.setTooltip("Controls how quickly the compressor stops compressing after the input signal drops below the threshold. Shorter release times can help retain transients.");
        releaseKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
        releaseKnob.setLookAndFeel(&pedalLookAndFeel);
        
        releaseKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 15);
        releaseKnob.setRange(1.0, 100.0);
        releaseKnob.setValue(50.0);
        releaseKnob.setTextValueSuffix(" ms");
        
        releaseKnob.addListener(this);
        releaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "COMP_RELEASE", releaseKnob);
        
        // Ratio
        addAndMakeVisible(ratioKnob);
        ratioKnob.setTooltip("Determines the amount of gain reduction applied once the input signal exceeds the threshold. Higher ratios result in more compression.");
        ratioKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
        ratioKnob.setLookAndFeel(&pedalLookAndFeel);
        
        ratioKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 15);
        ratioKnob.setRange(1.0, 8.0);
        ratioKnob.setValue(4.0);
        ratioKnob.setTextValueSuffix(": 1");
        
        ratioKnob.addListener(this);
        ratioAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "COMP_RATIO", ratioKnob);
        
        // Level
        addAndMakeVisible(levelKnob);
        levelKnob.setTooltip("Adjusts the overall output level of the compressed signal.");
        levelKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
        levelKnob.setLookAndFeel(&pedalLookAndFeel);
        
        levelKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 15);
        levelKnob.setRange(-20.f, 10.f);
        levelKnob.setValue(0.f);
        levelKnob.setTextValueSuffix(" dB");
        
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
        
        if (audioProcessor.usingComp)
        {
            bypassLed.setImage(juce::ImageCache::getFromMemory(BinaryData::ledOn_png, BinaryData::ledOn_pngSize));
        }
        else
        {
            bypassLed.setImage(juce::ImageCache::getFromMemory(BinaryData::ledOff_png, BinaryData::ledOff_pngSize));
        }
        bypassLed.setBounds(340, 433, 40, 40);
        
        

        juce::Rectangle<int> pedalBounds;
        pedalBounds.setBounds(250, 115, compSvg->getWidth() * 1.85, compSvg->getHeight() * 1.85);
        compSvg->setTransformToFit(pedalBounds.toFloat(), juce::RectanglePlacement::stretchToFit);

        
        compToggleImage.setBounds(328, 374, 91, 91);
        
        attackKnob.setBounds(268, 144, 60, 75);
        
        thresholdKnob.setBounds(330, 144, 60, 75);
        
        ratioKnob.setBounds(392, 144, 60, 75);
        
        releaseKnob.setBounds(268, 239, 61, 75);

        levelKnob.setBounds(392, 239, 60, 75);
    }
    
    void buttonClicked(juce::Button* button) override
    {

    }
    
    void buttonStateChanged(juce::Button* button) override
    {
        if (button == &compToggleImage)
        {
            if (compToggleImage.getToggleState())
            {
                bypassLed.setImage(juce::ImageCache::getFromMemory(BinaryData::ledOn_png, BinaryData::ledOn_pngSize));
            }
            else
            {
                bypassLed.setImage(juce::ImageCache::getFromMemory(BinaryData::ledOff_png, BinaryData::ledOff_pngSize));
            }
        }
    }
    
    
    virtual void sliderValueChanged (juce::Slider* slider) override
    {
    }
    
    void valueTreeRedirected(juce::ValueTree& treeWhichHasBeenChanged) override
    {
        
    }
private:
    ArtisianDSPAudioProcessor& audioProcessor;
    PedalLookAndFeel pedalLookAndFeel;
    
    juce::TooltipWindow tooltipWindow;
    
    
    // Pedal Image
    std::unique_ptr<juce::Drawable> compSvg;
    
    // Bypass Light
    juce::ImageComponent bypassLed;
    
    
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
    
    juce::Label compLabel;
};
