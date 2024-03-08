#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

class Gate1Component : public juce::Component,
                       public juce::Button::Listener
{
public:
    Gate1Component()
    {
        helloLabel.setFont(20.0f);
        helloLabel.setJustificationType(juce::Justification::centred);
        helloLabel.setText("Noise Gate", juce::dontSendNotification);
        addAndMakeVisible(helloLabel);
        
        
//        addAndMakeVisible(gateToggle);
        
    }
//    ~Gate1Component();

    virtual void resized() override
    {
        helloLabel.setBounds(getLocalBounds());
//        gateToggle.setBounds(getLocalBounds());
    }
        
    void buttonClicked(juce::Button* button) override
    {
        if (button == &gateToggle)
        {
            
        }
    };

private:
//    ArtisianDSPAudioProcessor& audioProcessor;
    
    juce::Label helloLabel;
    
    juce::ToggleButton gateToggle;
};








class GateProcessor  : public ProcessorBase
{
public:
    GateProcessor()
    {
        // audio processing goes here
    }

    void prepareToPlay (double sampleRate, int samplesPerBlock) override
    {
        // preprocessing
    }

    void processBlock (juce::AudioSampleBuffer& buffer, juce::MidiBuffer&) override
    {
        
    }

    void reset() override
    {
        // can reset the state of the dsp::Oscillator object by overriding the reset() function of the AudioProcessor and calling the same function onto it
        // oscillator.reset();
    }

    const juce::String getName() const override { return "Gate"; }

private:

};
