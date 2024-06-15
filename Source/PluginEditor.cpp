/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
RasterComponent::RasterComponent(ArtisianDSPAudioProcessor& p) : audioProcessor(p),
                                                                 multiSceneComponent(p), presetPanel(audioProcessor.getPresetManager()),
                                                                 helpComponent()
{
    
    // Vertical Input & Output Meters
    addAndMakeVisible(verticalInputMeterL);
//    addAndMakeVisible(verticalInputMeterR);
//    addAndMakeVisible(verticalOutputMeterL);
    addAndMakeVisible(verticalOutputMeterR);
    // Refresh Rate for Meter
    startTimerHz(60);
    
    // Help Menu
    addAndMakeVisible(helpButton);
    helpButton.setButtonText("?");
    helpButton.addListener(this);
    
    
    
    // MultiScene Elements
    addAndMakeVisible(multiSceneComponent);
    
    
    // Input Gain Knob
    addAndMakeVisible(inputGainSlider);
    inputGainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    inputGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 15);
    inputGainSlider.setRange(-15.0f, 15.0f);
    inputGainSlider.setValue(0.0f);
    inputGainSlider.addListener (this);
    inputGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "INPUT_GAIN", inputGainSlider);
    
    // Input Gain Label
    addAndMakeVisible (inputGainLabel);
    inputGainLabel.setText ("Input", juce::dontSendNotification);
    inputGainLabel.setColour (juce::Label::textColourId, juce::Colours::ghostwhite);
    inputGainLabel.setJustificationType (juce::Justification::centredBottom);
    
    

    // Output Gain Knob
    addAndMakeVisible(outputGainSlider);
    outputGainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    outputGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 15);
    outputGainSlider.setRange(-15.0f, 15.0f);
    outputGainSlider.setValue(0.0f);
    outputGainSlider.addListener (this);
    outputGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "OUTPUT_GAIN", outputGainSlider);
        
    
    // Output Gain Label
    addAndMakeVisible (outputGainLabel);
    outputGainLabel.setText ("Output", juce::dontSendNotification);
    outputGainLabel.setColour (juce::Label::textColourId, juce::Colours::ghostwhite);
    outputGainLabel.setJustificationType (juce::Justification::centredBottom);
    
    // Resize Combobox
    addAndMakeVisible(presetSelector);
    presetSelector.addListener(this);
    
    
    
    
    // Preset Management
    addAndMakeVisible(presetPanel);
    presetPanel.toFront(true);
}

RasterComponent::~RasterComponent()
{
}

void RasterComponent::timerCallback()
{
    //Input
    verticalInputMeterL.setLevel(audioProcessor.getRmsValue(0));
//    verticalInputMeterR.setLevel(audioProcessor.getRmsValue(1));
    verticalInputMeterL.repaint();
//    verticalInputMeterR.repaint();
    
    // Output
//    verticalOutputMeterL.setLevel(audioProcessor.getRmsValue(0));
    verticalOutputMeterR.setLevel(audioProcessor.getRmsValue(1));
//    verticalOutputMeterL.repaint();
    verticalOutputMeterR.repaint();

}

//==============================================================================
void RasterComponent::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
    
    g.setColour(juce::Colour (26, 26, 26));
    g.fillRect(getBounds().removeFromBottom(30));
    
    
}

void RasterComponent::resized()
{
    
    sliderThreshold.setBounds(100, 100, 20, 20);
    

    
    multiSceneComponent.setBounds(0, 0, 720, 506);
    
    verticalInputMeterL.setBounds(15, 15, 20, 205);
//    verticalInputMeterR.setBounds(25, 15, 15, 200);
//    verticalOutputMeterL.setBounds(676, 15, 15, 200);
    verticalOutputMeterR.setBounds(685, 15, 20, 205);
    
    inputGainSlider.setBounds(40, 55, 100, 100);
    outputGainSlider.setBounds(575, 55, 100, 100);
    inputGainLabel.setBounds(40, 159, 100, 25);
    outputGainLabel.setBounds(575, 159, 100, 25);
    
    presetPanel.setBounds(156, 72, 415, 41);
    
    helpButton.setBounds(0, 510, 30, 30);
    
}


//======================================================================
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
//===============================================================


// Value Changes
void RasterComponent::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &inputGainSlider)
    {
        // effectively mute at minimum value
        if (inputGainSlider.getValue() == -15.0f)
        {
            audioProcessor.inputGainFloat = (float) -100.0f;
            
        }
        else
        {
            audioProcessor.inputGainFloat = (float) inputGainSlider.getValue();
        }
    }
    
    if (slider == &outputGainSlider)
    {
        if (inputGainSlider.getValue() == -15.0f)
        {
            audioProcessor.outputGainFloat = (float) -100.0f;
            
        }
        else
        {
            audioProcessor.outputGainFloat = (float) outputGainSlider.getValue();
        }
    }
    
    if (slider == &sliderThreshold)
    {
//        juce::Logger::outputDebugString("Threshold: " + juce::String(apvts.getRawParameterValue("THRESHOLD")));
    }
}

void RasterComponent::comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged)
{
    if (comboBoxThatHasChanged == &presetSelector)
    {
    }
}

void RasterComponent::buttonClicked (juce::Button* button)
{
    if (button == &helpButton)
    {
        if (!helpComponent)
        {
            helpComponent = std::make_unique<HelpComponent>();
            addAndMakeVisible(helpComponent.get());
            helpComponent->setBounds(0, 0, 720, 510);
        }
        else
        {
            helpComponent->setVisible(!helpComponent->isVisible());
            if (helpComponent->isVisible())
            {
                helpComponent->setBounds(0, 0, 720, 510);
            }
            helpComponent->toFront(true);
        }
    }
}

//======================================================================
