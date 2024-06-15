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
#include "PresetPanel.h"
#include "HelpComponent.h"


//==============================================================================
/**
*/

class RasterComponent  : public juce::Component,
                         public juce::Slider::Listener,
                         public juce::Button::Listener,
                         public juce::ComboBox::Listener,
                         public juce::Timer
{
public:
    RasterComponent (ArtisianDSPAudioProcessor&);
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
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> outputGainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> inputGainAttachment;
    
    void sliderValueChanged(juce::Slider* slider) override;
    void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override;
    void buttonClicked (juce::Button* button) override;
private:
    ArtisianDSPAudioProcessor& audioProcessor;
    Gui::PresetPanel presetPanel;
    
    MultiSceneComponent multiSceneComponent;
    
    juce::ComboBox presetSelector;
    
    gui::VerticalMeter verticalInputMeterL, verticalInputMeterR;
    
    juce::Slider sliderThreshold;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderAttachmentThreshold;
    
    gui::VerticalMeter verticalOutputMeterL, verticalOutputMeterR;
    
    juce::TextButton helpButton {"?"};
    std::unique_ptr<HelpComponent> helpComponent = std::make_unique<HelpComponent>();

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


