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
    
    
    // Input Gain Knob
    addAndMakeVisible(inputGainSlider);
    inputGainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    inputGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 20);
    inputGainSlider.setRange(-48.0f, 0.0f);
    inputGainSlider.setValue(0.5f);
    inputGainSlider.addListener (this); // enables connection of slider to function
    
    
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
    auto area = getLocalBounds();
    
    multiSceneComponent.setBounds(area);
    
    inputGainSlider.setBounds(10, 25, 100, 100);
    
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void ArtisianDSPAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &inputGainSlider)
    {
        audioProcessor.inputGain = (float) inputGainSlider.getValue();
        
        
        // It does get value successfully, but need to figure out what to do with it..
//        std::cout << std::to_string(audioProcessor.inputGain);
    }
}


