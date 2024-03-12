/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "MultiSceneComponent.h"
#include "VerticalMeter.h"


//==============================================================================
/**
*/
class RasterComponent  : public juce::Component,
                         public juce::Slider::Listener,
                         public juce::ComboBox::Listener,
                         public juce::Timer
{
public:
    RasterComponent (ArtisianDSPAudioProcessor& p, juce::AudioProcessorValueTreeState& vts);
    ~RasterComponent() override;

    //==============================================================================
    
    void timerCallback() override;
    void paint (juce::Graphics&) override;
    void resized() override;
    

    // Slider Object Decleration
    juce::Slider inputGainSlider;
    juce::Slider outputGainSlider;
    
    juce::Label inputGainLabel;
    juce::Label outputGainLabel;
    
    void sliderValueChanged(juce::Slider* slider) override;
    void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ArtisianDSPAudioProcessor& audioProcessor;
    
    juce::AudioProcessorValueTreeState& apvts;
    
    MultiSceneComponent multiSceneComponent;
    
    juce::ComboBox presetSelector;
    
    gui::VerticalMeter verticalInputMeterL, verticalInputMeterR;
    
    juce::Slider sliderThreshold;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderAttachmentThreshold;
    
    gui::VerticalMeter verticalOutputMeterL, verticalOutputMeterR;
    

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
    juce::ApplicationProperties applicationProperties;
};
