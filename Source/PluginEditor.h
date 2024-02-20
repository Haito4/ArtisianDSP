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
                                         public juce::Slider::Listener,
                                         public juce::ComboBox::Listener
{
public:
//    ArtisianDSPAudioProcessorEditor ();
    ArtisianDSPAudioProcessorEditor (ArtisianDSPAudioProcessor&);
    ~ArtisianDSPAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    

    // Slider Object Decleration
    juce::Slider inputGainSlider;
    juce::Slider outputGainSlider;
    
    juce::Label inputGainLabel;
    juce::Label outputGainLabel;
    
    void sliderValueChanged(juce::Slider* slider) override;
    void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override;
    
    float uiScaleFactor{ 1 };
    
    
    
    
//    juce::String getTextFromValue (double value) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ArtisianDSPAudioProcessor& audioProcessor;
    
    MultiSceneComponent multiSceneComponent;
    
    juce::ComboBox resizenator;
        
    void updateResolution();
    
    int width{ 720 };
    
    int height{ 540 };
    
    
    
    
    
    
    
    
//    juce::AudioParameterFloat* inputGainParameter;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ArtisianDSPAudioProcessorEditor)
};
