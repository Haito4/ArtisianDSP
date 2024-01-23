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
class ArtisianDSPAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    ArtisianDSPAudioProcessorEditor(ArtisianDSPAudioProcessor& p);
    ~ArtisianDSPAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ArtisianDSPAudioProcessor& audioProcessor;
    MultiSceneComponent multiSceneComponent;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ArtisianDSPAudioProcessorEditor)
};
