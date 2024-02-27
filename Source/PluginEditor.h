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
class RasterComponent  : public juce::Component,
                         public juce::Slider::Listener,
                         public juce::ComboBox::Listener
{
public:
//    ArtisianDSPAudioProcessorEditor ();
    RasterComponent (ArtisianDSPAudioProcessor&);
    ~RasterComponent() override;

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

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ArtisianDSPAudioProcessor& audioProcessor;
    
    MultiSceneComponent multiSceneComponent;
    
    juce::ComboBox resizenator;
        
//    void updateResolution();
    
    int width{ 720 };
    
    int height{ 540 };
    


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RasterComponent)
};



// Root Wrapper

class WrappedRasterAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    WrappedRasterAudioProcessorEditor(ArtisianDSPAudioProcessor&);
    void resized() override;
    
private:
    static constexpr int originalWidth{ 720 };
    static constexpr int originalHeight{ 540 };
    
    RasterComponent rasterComponent;
};
