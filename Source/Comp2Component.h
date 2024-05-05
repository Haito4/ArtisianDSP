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
        // Toggle Button
        addAndMakeVisible(compToggle);
        compToggleText = usingCompValue ? "On" : "Off";
        juce::Logger::outputDebugString("compToggle Initial State: " + compToggleText);
        compToggle.setButtonText(compToggleText);
        compToggle.setClickingTogglesState(true);
        
        compToggle.addListener(this);
        compToggleAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "USING_COMP", compToggle);
        
        // Threshold
        addAndMakeVisible(thresholdKnob);
        thresholdKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
        thresholdKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 15);
        thresholdKnob.setRange(-100.0, 6.0);
        thresholdKnob.setValue(1.0);
        thresholdKnob.setTextValueSuffix(" dB");
        
        thresholdKnob.addListener(this);
        thresholdAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "COMP_THRES", thresholdKnob);
        
        // Attack
        addAndMakeVisible(attackKnob);
        attackKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
        attackKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 15);
        attackKnob.setRange(1.0, 100.0);
        attackKnob.setValue(50.0);
        attackKnob.setTextValueSuffix(" ms");
        
        attackKnob.addListener(this);
        attackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "COMP_ATTACK", attackKnob);
        
        // Release
        addAndMakeVisible(releaseKnob);
        releaseKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
        releaseKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 15);
        releaseKnob.setRange(1.0, 100.0);
        releaseKnob.setValue(50.0);
        releaseKnob.setTextValueSuffix(" ms");
        
        releaseKnob.addListener(this);
        releaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "COMP_RELEASE", releaseKnob);
        
        // Ratio
        addAndMakeVisible(ratioKnob);
        ratioKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
        ratioKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 15);
        ratioKnob.setRange(1.0, 8.0);
        ratioKnob.setValue(1.0);
        ratioKnob.setTextValueSuffix(": 1");
        
        ratioKnob.addListener(this);
        ratioAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "COMP_RATIO", ratioKnob);
        
        
        compLabel.setFont(20.f);
        compLabel.setJustificationType(juce::Justification::centred);
        compLabel.setText("Compressor", juce::dontSendNotification);
        addAndMakeVisible(compLabel);
    }
//    ~Comp2Component();
    
    virtual void resized() override
    {
//        compLabel.setBounds(getLocalBounds());
        
        compToggle.setBounds(335, 410, 50, 50);
        
        thresholdKnob.setBounds(315, 140, 90, 90);
        
        attackKnob.setBounds(215, 140, 90, 90);
        
        releaseKnob.setBounds(215, 230, 90, 90);
        
        ratioKnob.setBounds(415, 140, 90, 90);
    }
    
    void buttonClicked(juce::Button* button) override
    {
        if (button == &compToggle)
        {
            // just do an if statement
            
            usingCompValue = dynamic_cast<juce::AudioParameterBool*>(audioProcessor.apvts.getParameter("USING_COMP"))->get();
            compToggleText = usingCompValue ? "Off" : "On";
            compToggle.setButtonText(compToggleText);
            juce::Logger::outputDebugString("Comp state: " + compToggleText);
        }
    }
    
    virtual void sliderValueChanged (juce::Slider* slider) override
    {
        
    }
private:
    ArtisianDSPAudioProcessor& audioProcessor;
    
    // Toggle
    juce::String compToggleText;
    juce::TextButton compToggle;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> compToggleAttachment;
    bool usingCompValue { dynamic_cast<juce::AudioParameterBool*>(audioProcessor.apvts.getParameter("USING_COMP"))->get() };
    
    // Sliders
    juce::Slider thresholdKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> thresholdAttachment;
    
    juce::Slider attackKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attackAttachment;
    
    juce::Slider releaseKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> releaseAttachment;
    
    juce::Slider ratioKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> ratioAttachment;
    
    
    juce::Label compLabel;
};
