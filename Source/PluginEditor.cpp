/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ArtisianDSPAudioProcessorEditor::ArtisianDSPAudioProcessorEditor(ArtisianDSPAudioProcessor& p) : AudioProcessorEditor (&p), audioProcessor(p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    
    addAndMakeVisible(multiSceneComponent);
    
    addAndMakeVisible(inputGainSlider);
    
    inputGainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    inputGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
    inputGainSlider.setRange(-48.0, 0.0);
    inputGainSlider.setValue(-1.0); // optimise
    inputGainSlider.addListener (this); // what connects slider to function
    
    
    setSize (720, 540);
}

ArtisianDSPAudioProcessorEditor::~ArtisianDSPAudioProcessorEditor()
{
}

//==============================================================================
void ArtisianDSPAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
//    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    
    
    g.fillAll(juce::Colours::black);
}

void ArtisianDSPAudioProcessorEditor::resized()
{
    multiSceneComponent.setBounds(getLocalBounds());
    
    inputGainSlider.setBounds(getLocalBounds());
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
