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
    
    setResizable(true, false);
    setResizeLimits(360, 260, 1280, 960);
    
    const float ratio = 3 / 4;
    getConstrainer ()->setFixedAspectRatio (ratio);
    
    addAndMakeVisible(multiSceneComponent);
    
    
    // Input Gain Knob
    addAndMakeVisible(inputGainSlider);
    inputGainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    inputGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 15);
    inputGainSlider.setRange(-15.0f, 15.0f);
    inputGainSlider.setValue(0.0f);
    inputGainSlider.addListener (this);
    
    // Input Gain Label
    addAndMakeVisible (inputGainLabel);
    inputGainLabel.setText ("Input", juce::dontSendNotification);
    inputGainLabel.attachToComponent (&inputGainSlider, false);
    inputGainLabel.setColour (juce::Label::textColourId, juce::Colours::ghostwhite);
    inputGainLabel.setJustificationType (juce::Justification::centredBottom);


    // Output Gain Knob
    addAndMakeVisible(outputGainSlider);
    outputGainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    outputGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 15);
    outputGainSlider.setRange(-15.0f, 15.0f);
    outputGainSlider.setValue(0.0f);
    outputGainSlider.addListener (this);
    
    // Output Gain Label
    addAndMakeVisible (outputGainLabel);
    outputGainLabel.setText ("Output", juce::dontSendNotification);
    outputGainLabel.attachToComponent (&outputGainSlider, false);
    outputGainLabel.setColour (juce::Label::textColourId, juce::Colours::ghostwhite);
    outputGainLabel.setJustificationType (juce::Justification::centredBottom);

    
    // Resize Combobox
    addAndMakeVisible(resizenator);
    resizenator.addListener(this);
    
    resizenator.addItem("Small", 1);
    resizenator.addItem("Medium", 2);
    resizenator.addItem("Large", 3);
    
    resizenator.setSelectedId(2);
    
    setSize(width, height);
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
    
    auto bounds = getLocalBounds();
    
    multiSceneComponent.setBounds(bounds);
    
    
    inputGainSlider.setBounds(40 * uiScaleFactor, 55 * uiScaleFactor, 100 * uiScaleFactor, 100 * uiScaleFactor);
    outputGainSlider.setBounds(575 * uiScaleFactor, 55 * uiScaleFactor, 100 * uiScaleFactor, 100 * uiScaleFactor);
    
    resizenator.setBounds(690 * uiScaleFactor, 496 * uiScaleFactor, 30 * uiScaleFactor, 24 * uiScaleFactor);

}

void ArtisianDSPAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &inputGainSlider)
    {
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

void ArtisianDSPAudioProcessorEditor::comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged)
{
    if (comboBoxThatHasChanged == &resizenator)
    {
        int selectedId = resizenator.getSelectedId();
        juce::String selectedText = resizenator.getItemText(selectedId - 1);
        juce::Logger::outputDebugString("Selected: " + selectedText);
        
        updateResolution();
        
        setSize(width, height);
    }
}

void ArtisianDSPAudioProcessorEditor::updateResolution()
{
    // Get the selected resolution
    int selectedId = resizenator.getSelectedId();
    // Set bounds based on selected resolution
    switch (selectedId)
    {
        case 1: // Small
            width = 360;
            height = 260;
            uiScaleFactor = 0.5;
            break;
        case 2: // Medium
            width = 720;
            height = 520;
            uiScaleFactor = 1;
            break;
        case 3: // Large
            width = 1080;
            height = 780;
            uiScaleFactor = 1.5;
            break;
        default:
            // Use default medium resolution
            width = 720;
            height = 520;
            uiScaleFactor = 1;
    }
}
