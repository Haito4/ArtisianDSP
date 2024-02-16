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
    inputGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 15);
    inputGainSlider.setRange(-15.0f, 15.0f);
    inputGainSlider.setValue(0.0f);
    inputGainSlider.addListener (this);
    
    // Input Gain Label
//    addAndMakeVisible(inputGainLabel);

    
    
    
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
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
//    setResizable(true, true);
//    float aspectRatio = 720 / 540;
    auto area = getLocalBounds();
    
    multiSceneComponent.setBounds(area);
    
    inputGainSlider.setBounds(10, 25, 100, 100);
    

}

void ArtisianDSPAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &inputGainSlider)
    {
//        auto& parameters = audioProcessor.getParameters();
//        auto* inputGain = parameters.getUnchecked(0);
//        inputGain->setValueNotifyingHost(inputGainSlider.getValue());
        
        // effectively mute at mininum value
        if (inputGainSlider.getValue() == -15.0f)
        {
            audioProcessor.oldInputGain = (float) -48.0f;
            
        }
        else
        {
            audioProcessor.oldInputGain = (float) inputGainSlider.getValue();
        }
        
        
        

    }
}
