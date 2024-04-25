#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

class Gate1Component : public juce::Component,
                       public juce::Button::Listener,
                       public juce::Slider::Listener
{
public:
    Gate1Component(ArtisianDSPAudioProcessor& processor)
     : audioProcessor(processor)
    {
        // Gate On/Off Switch
        gateToggleText = usingGateValue ? "On" : "Off";
        juce::Logger::outputDebugString("gateToggle Initial State: " + gateToggleText);
        gateToggle.setButtonText(gateToggleText);
        gateToggle.setClickingTogglesState(true);
        addAndMakeVisible(gateToggle);
        gateToggle.addListener(this);
        gateToggleAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "USING_GATE", gateToggle);
        
        
        helloLabel.setFont(20.0f);
        helloLabel.setJustificationType(juce::Justification::centred);
        helloLabel.setText("Noise Gate", juce::dontSendNotification);
        addAndMakeVisible(helloLabel);
        
        
        thresholdSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
        thresholdSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 15);
        thresholdSlider.setRange(-100.0, 6.0); // Adjust the range to match your parameter
        thresholdSlider.setValue(1.0); // Set the initial value
        thresholdSlider.setTextValueSuffix(" dB");
        addAndMakeVisible(thresholdSlider);
        
        attackSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
        attackSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 15);
        attackSlider.setRange(1.0, 100.0);
        attackSlider.setValue(50.0);
        attackSlider.setTextValueSuffix(" ms");
        addAndMakeVisible(attackSlider);
        
        releaseSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
        releaseSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 15);
        releaseSlider.setRange(1.0, 100.0);
        releaseSlider.setValue(50.0);
        releaseSlider.setTextValueSuffix(" ms");
        addAndMakeVisible(releaseSlider);
        
        
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
        
        helloLabel.setBounds(280, 118, 160, 42);
        gateToggle.setBounds(335, 400, 50, 50);
        
        thresholdSlider.setBounds(250, 150, 100, 100);
        attackSlider.setBounds(370, 150, 100, 100);
        releaseSlider.setBounds(370, 270, 100, 100);
        
    }
        
    void buttonClicked(juce::Button* button) override
    {
        if (button == &gateToggle)
        {
            usingGateValue = dynamic_cast<juce::AudioParameterBool*>(audioProcessor.apvts.getParameter("USING_GATE"))->get();
            gateToggleText = usingGateValue ? "Off" : "On";
            gateToggle.setButtonText(gateToggleText);
            juce::Logger::outputDebugString("Gate state: " + gateToggleText);
        }
    };

    void sliderValueChanged(juce::Slider* slider) override
    {
        if (slider == &thresholdSlider)
        {
//            juce::Logger::outputDebugString(apvts.getRawParameterValue("THRESHOLD"));
        }
    }
    
private:
    ArtisianDSPAudioProcessor& audioProcessor;
    
    
    juce::Label helloLabel;
    
    
    juce::String gateToggleText;
    juce::TextButton gateToggle;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> gateToggleAttachment;
    bool usingGateValue { dynamic_cast<juce::AudioParameterBool*>(audioProcessor.apvts.getParameter("USING_GATE"))->get() };
    
    
    
    juce::Slider thresholdSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> thresholdAttachment;
    
    
    juce::Slider attackSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attackAttachment;
    
    juce::Slider releaseSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> releaseAttachment;
    
};
