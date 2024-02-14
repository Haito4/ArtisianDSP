/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "MultiSceneComponent.h"

//==============================================================================
/**
*/
class ArtisianDSPAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                         public juce::Slider::Listener
{
public:
//    ArtisianDSPAudioProcessorEditor ();
    ArtisianDSPAudioProcessorEditor (ArtisianDSPAudioProcessor&);
    ~ArtisianDSPAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
//    void sliderValueChanged(juce::Slider* slider) override;
    juce::Slider inputGainSlider;
    
    void sliderValueChanged(juce::Slider* slider) override
    {
        if (slider == &inputGainSlider)
        {
            inputGainSlider.setValue (inputGainSlider.getValue(), juce::dontSendNotification);
        }
    }

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ArtisianDSPAudioProcessor& audioProcessor;
    
    MultiSceneComponent multiSceneComponent;
    
//    juce::AudioParameterFloat* inputGainParameter;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ArtisianDSPAudioProcessorEditor)
};
