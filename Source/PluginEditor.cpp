/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
RasterComponent::RasterComponent(ArtisianDSPAudioProcessor& p) : audioProcessor(p)
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
}

RasterComponent::~RasterComponent()
{
}

//==============================================================================
void RasterComponent::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
    
    g.setColour(juce::Colour (26, 26, 26));
    g.fillRect(getBounds().removeFromBottom(32));
    
    
}

void RasterComponent::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    auto bounds = getLocalBounds();
    
    multiSceneComponent.setBounds(bounds);
    
    
    inputGainSlider.setBounds(40, 55, 100, 100);
    outputGainSlider.setBounds(575, 55, 100, 100);
    
    resizenator.setBounds(690, 496, 30, 24);

}



// Wrapper Implementation
WrappedRasterAudioProcessorEditor::WrappedRasterAudioProcessorEditor(ArtisianDSPAudioProcessor& p)
: AudioProcessorEditor(p),
  rasterComponent(p)
{
    addAndMakeVisible(rasterComponent);
    
    juce::PropertiesFile::Options options;
    options.applicationName = ProjectInfo::projectName;
    options.commonToAllUsers = true;
    options.filenameSuffix = "settings";
    options.osxLibrarySubFolder = "Application Support";
    applicationProperties.setStorageParameters(options);
    
    
    
    // Only execute if the constainer is not a null pointer
    if (auto* constrainer = getConstrainer())
    {
        constrainer->setFixedAspectRatio(static_cast<double> (originalWidth) / static_cast<double> (originalHeight));
        constrainer->setSizeLimits(originalWidth / 2, originalHeight / 2, originalWidth * 1.5, originalHeight * 1.5);
        
    }
    
    
    auto sizeRatio{ 1.0 };
    if (auto* properties = applicationProperties.getCommonSettings(true))
    {
        sizeRatio = properties->getDoubleValue("sizeRatio", 1.0);
    }
    
    setResizable(true, true);
    //Set Inital Size
    setSize(static_cast<int> (originalWidth * sizeRatio), static_cast<int> (originalHeight * sizeRatio));
}

void WrappedRasterAudioProcessorEditor::resized()
{
    // When the wrapper component's size is changed, scale the raster component
    const auto scaleFactor = static_cast<float> (getWidth()) / originalWidth;
    if (auto* properties = applicationProperties.getCommonSettings(true))
    {
        properties->setValue("sizeRatio", scaleFactor);
    }
    
    rasterComponent.setTransform(juce::AffineTransform::scale(scaleFactor));
    rasterComponent.setBounds(0, 0, originalWidth, originalHeight);
}




// Value Changes
void RasterComponent::sliderValueChanged(juce::Slider* slider)
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

void RasterComponent::comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged)
{
    if (comboBoxThatHasChanged == &resizenator)
    {
        int selectedId = resizenator.getSelectedId();
        juce::String selectedText = resizenator.getItemText(selectedId - 1);
        juce::Logger::outputDebugString("Selected: " + selectedText);
        
    }
}
