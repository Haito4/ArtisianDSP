/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Drive3Component.h"
#include "MultiSceneComponent.h"

//==============================================================================
ArtisianDSPAudioProcessorEditor::ArtisianDSPAudioProcessorEditor (ArtisianDSPAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), multiSceneComponent(p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    addAndMakeVisible(multiSceneComponent);
    setSize (720, 540);
}

ArtisianDSPAudioProcessorEditor::~ArtisianDSPAudioProcessorEditor()
{
}

//==============================================================================
void ArtisianDSPAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

//    g.setColour (juce::Colours::white);
//    g.setFont (15.0f);
//    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void ArtisianDSPAudioProcessorEditor::resized()
{
    multiSceneComponent.setBounds(getLocalBounds());
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
