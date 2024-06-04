#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Jb_knobs.h"

class Impulse6Component : public juce::Component,
                          public juce::Button::Listener,
                          public juce::Slider::Listener,
                          public juce::ValueTree::Listener,
                          public juce::Timer
{
public:
    Impulse6Component(ArtisianDSPAudioProcessor& processor) : audioProcessor(processor)
    {
        audioProcessor.apvts.state.addListener(this);
        
        audioProcessor.variableTree2.addListener(this);
        
        // Label
        impulseLabel.setFont(20.f);
        impulseLabel.setJustificationType(juce::Justification::centred);
        impulseLabel.setText("Impulse Response", juce::dontSendNotification);
        addAndMakeVisible(impulseLabel);
        
        
        // Bypass Switch
        addAndMakeVisible(irToggleImage);
        irToggleImage.addListener(this);
        irToggleImage.setImages(false, true, true, juce::ImageCache::getFromMemory(BinaryData::dogreen_png, BinaryData::dogreen_pngSize), 0.5f, juce::Colours::green,
                                                      juce::ImageCache::getFromMemory(BinaryData::dohover_png, BinaryData::dohover_pngSize), 0.5f, juce::Colours::blue,
                                                      juce::ImageCache::getFromMemory(BinaryData::dored_png, BinaryData::dored_pngSize), 0.5f, juce::Colours::red);
        irToggleImage.setClickingTogglesState(true);
        irToggleAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "USING_IR", irToggleImage);
        
        
        // Ir Selector
        addAndMakeVisible(loadBtn);
        loadBtn.setButtonText("Load IR");
        loadBtn.onClick = [this]()
        {
            fileChooser = std::make_unique<juce::FileChooser>("Choose IR File",
                                                              audioProcessor.root,
                                                              "*");
            
            const auto fileChooserFlags = juce::FileBrowserComponent::openMode |
                                          juce::FileBrowserComponent::canSelectFiles |
                                          juce::FileBrowserComponent::canSelectDirectories;
            fileChooser->launchAsync(fileChooserFlags, [this](const juce::FileChooser& chooser)
            {
                juce::File result (chooser.getResult());
                
                if (result.getFileExtension() == ".wav")
                {
                    audioProcessor.savedFile = result;
                    audioProcessor.variableTree.setProperty("file1",
                                                            result.getFullPathName(),
                                                            nullptr);
                    audioProcessor.variableTree.setProperty("root",
                                                            result.getParentDirectory().getFullPathName(),
                                                            nullptr);
                    audioProcessor.root = result.getParentDirectory().getFullPathName();
                    
                    
                    audioProcessor.speakerModule.loadImpulseResponse(result, // apply to ir loader
                                                                     juce::dsp::Convolution::Stereo::yes,
                                                                     juce::dsp::Convolution::Trim::yes, 0);
                    irName.setText(result.getFileName(), juce::dontSendNotification);
                    
                    
                    audioProcessor.lastIrPath = result.getFullPathName();
                    audioProcessor.lastIrName = result.getFileName();
                    
                    
                    DBG("Current IR: " + audioProcessor.lastIrName);
                    DBG("Current IR Path: " + audioProcessor.lastIrPath);
                    
                    
//                    audioProcessor.apvts.state.setProperty("IRPath", result.getFullPathName(), nullptr);
//                    audioProcessor.apvts.state.setProperty("IRName", result.getFileName(), nullptr);
                }
            });
        };
        
        addAndMakeVisible(irName);
        irName.setJustificationType(juce::Justification::horizontallyCentred);
        
        
        
        addAndMakeVisible(jbSlider);
//        jbSlider.setText("VOLUME", juce::dontSendNotification);
//        volLabel.setJustificationType(juce::Justification::centred);
        jbSlider.setLookAndFeel(&afxLookAndFeel);
        jbSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        jbSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, jbSlider.getTextBoxHeight());
        volAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "IR_VOLUME", jbSlider);
        
        
        
        // Button to Fix IR locations from broken presets
        addAndMakeVisible(fixButton);
        fixButton.setButtonText("Fix");
        if (audioProcessor.validIrLoaded == false) // If the initally loaded Ir is invalid
        {
            fixButton.setVisible(true);
            irName.setText("Invalid IR Loaded!", juce::dontSendNotification);
        }
        else
        {
            fixButton.setVisible(false);
        }
        
        
        fixButton.onClick = [this]()
        {
            altFileChooser = std::make_unique<juce::FileChooser>("Locate Missing IR File",
                                                              audioProcessor.root,
                                                              "*");
            
            const auto fileChooserFlags = juce::FileBrowserComponent::openMode |
                                          juce::FileBrowserComponent::canSelectFiles |
                                          juce::FileBrowserComponent::canSelectDirectories;
            altFileChooser->launchAsync(fileChooserFlags, [this](const juce::FileChooser& chooser)
            {
                juce::File result (chooser.getResult());
                
                if (result.getFileExtension() == ".wav")
                {
                    audioProcessor.savedFile = result;
                    audioProcessor.variableTree.setProperty("file1",
                                                            result.getFullPathName(),
                                                            nullptr);
                    audioProcessor.variableTree.setProperty("root",
                                                            result.getParentDirectory().getFullPathName(),
                                                            nullptr);
                    audioProcessor.root = result.getParentDirectory().getFullPathName();
                    
                    
                    audioProcessor.speakerModule.loadImpulseResponse(result, // apply to ir loader
                                                                     juce::dsp::Convolution::Stereo::yes,
                                                                     juce::dsp::Convolution::Trim::yes, 0);
                    irName.setText(result.getFileName(), juce::dontSendNotification);
                    
                    
                    audioProcessor.lastIrPath = result.getFullPathName();
                    audioProcessor.lastIrName = result.getFileName();
                    
                    
                    DBG("Current IR (fixed): " + audioProcessor.lastIrName);
                    DBG("Current IR Path (fixed): " + audioProcessor.lastIrPath);
                    
                    
                    
                    // Update Current XML to new IR values
                    const auto presetFile = juce::File(audioProcessor.presetName);
                    
                    DBG("Updating preset file: " + presetFile.getFullPathName());
                    if (presetFile.existsAsFile())
                    {
                        std::unique_ptr<juce::XmlElement> xml = juce::XmlDocument::parse(presetFile);
                        
                        if (xml != nullptr)
                        {
                            DBG("opened xml");
                            xml->setAttribute("IRPath", audioProcessor.lastIrPath);
                            xml->setAttribute("IRName", audioProcessor.lastIrName);
                            
                            DBG("new attributes set!");
                            if (!xml->writeTo(presetFile))
                            {
                                DBG("Could not update preset file: " + presetFile.getFullPathName());
                            }
                        }
                        else
                        {
                            DBG("failed to open xml");
                        }
                    }
                    
                    fixButton.setVisible(false); // hide button after successfully chosen file
                    audioProcessor.validIrLoaded = true;
                    DBG("ir fix finished!!!!!");
                }
            });
        };
        
    }
    
    ~Impulse6Component()
    {
        audioProcessor.apvts.state.removeListener(this);
        
        audioProcessor.variableTree2.removeListener(this);
    }
    
    virtual void resized() override
    {
        impulseLabel.setBounds(290, 150, 140, 50);
        
        loadBtn.setBounds(310, 270, 100, 50);
        irName.setBounds(260, 340, 200, 50);
        
        if (audioProcessor.validIrLoaded == false) // If the initally loaded Ir is invalid
        {
            irName.setText("Invalid IR Loaded!", juce::dontSendNotification);
        }
        else
        {
            irName.setText(audioProcessor.lastIrName, juce::dontSendNotification);
        }
        
        irToggleImage.setBounds(335, 405, 50, 50);
        
        fixButton.setBounds(310, 470, 100, 50);
        
//        jbSlider.setBounds(405, 405, 100, 100);
    }
    
    void buttonClicked(juce::Button* button) override
    {
        
    }
    
    virtual void sliderValueChanged (juce::Slider* slider) override
    {
        
    }
    
    void valueTreeRedirected(juce::ValueTree& treeWhichHasBeenChanged) override
    {
//        DBG("Setting Impulse Preset Values..");
        
        audioProcessor.shouldUpdate = true;
    }
    
    void timerCallback() override
    {
        
    }
    
    
    void valueTreePropertyChanged(juce::ValueTree& tree, const juce::Identifier& property) override
    {
        
        if (property == juce::Identifier("NEW_IRNAME"))
        {
            DBG("passed identifier");
            
            if (audioProcessor.validIrLoaded == false)
            {
                fixButton.setVisible(true); // show button to locate missing ir
                irName.setText("Invalid IR Loaded!", juce::dontSendNotification);
            }
            else
            {
                auto newIRName = tree.getProperty(property).toString();
                irName.setText(newIRName, juce::dontSendNotification);
                fixButton.setVisible(false);
            }
        }
    }
    
    
    
    
private:
    ArtisianDSPAudioProcessor& audioProcessor;
    
    AfxLookAndFeel afxLookAndFeel;
    
    
    juce::ImageButton irToggleImage;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> irToggleAttachment;
    
    
    
    juce::TextButton loadBtn;
    std::unique_ptr<juce::FileChooser> fileChooser;
    std::unique_ptr<juce::FileChooser> altFileChooser;
    juce::Label irName;
    
    
    juce::TextButton fixButton;
    
    juce::Label impulseLabel;
    
    
    
    juce::Slider jbSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> volAttachment;
};
