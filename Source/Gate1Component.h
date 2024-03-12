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
        helloLabel.setFont(20.0f);
        helloLabel.setJustificationType(juce::Justification::centred);
        helloLabel.setText("Noise Gate", juce::dontSendNotification);
        addAndMakeVisible(helloLabel);
        
        gateToggle.setButtonText("Gate On/Off");
        gateToggle.addListener(this);
        addAndMakeVisible(gateToggle);
        
        
        thresholdSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
        thresholdSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 15);
        thresholdSlider.setRange(-96.0, 6.0); // Adjust the range to match your parameter
        thresholdSlider.setValue(1.0); // Set the initial value
        addAndMakeVisible(thresholdSlider);
        
        thresholdSlider.addListener(this);
        thresholdAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "THRESHOLD", thresholdSlider);


        
    }
//    ~Gate1Component();

    virtual void resized() override
    {
        auto bounds = getLocalBounds();
        auto height = bounds.getHeight();
        auto width = bounds.getWidth();
        
        helloLabel.setBounds(getLocalBounds());
        gateToggle.setBounds(450, 270, 100, 50);
        
        thresholdSlider.setBounds(width / 2, height / 2, 100, 100);
        
    }
        
    void buttonClicked(juce::Button* button) override
    {
        if (button == &gateToggle)
        {
            audioProcessor.usingGate = !audioProcessor.usingGate;
            
            juce::Logger::outputDebugString("usingGate: " + juce::String(audioProcessor.usingGate ? "true" : "false"));
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
    
    juce::TextButton gateToggle;
    
    
    juce::Slider thresholdSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> thresholdAttachment;
    
};
