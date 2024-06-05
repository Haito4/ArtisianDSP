#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Jb_knobs.h"

class Impulse6Component : public juce::Component,
                          public juce::Button::Listener,
                          public juce::Slider::Listener,
                          public juce::ComboBox::Listener,
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
        
        // Ir Selector (from binary data)
        addAndMakeVisible(binaryIrChooser);
        binaryIrChooser.addListener(this);
        
        // Available Binary IRs
        binaryIrChooser.addItem("ML Sound Lab - Best IR in The World", 1);
        binaryIrChooser.addItem("Contradictions Collapse", 2);
        binaryIrChooser.addItem("Destroy Erase Improve", 3);
        binaryIrChooser.addItem("Chaosphere", 4);
        binaryIrChooser.addItem("Nothing", 5);
        binaryIrChooser.addItem("Nothing (Rerelease)", 6);
        binaryIrChooser.addItem("I", 7);
        binaryIrChooser.addItem("Catch 33", 8);
        binaryIrChooser.addItem("obZen", 9);
        binaryIrChooser.addItem("Koloss", 10);
        binaryIrChooser.addItem("Pitch Black", 11);
        binaryIrChooser.addItem("The Violent Sleep of Reason", 12);
        binaryIrChooser.addItem("Immutable", 13);
        
//        binaryIrChooser.setSelectedId(1); // Default value
        
        
        // Ir Selector (from file)
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
                    
                    
                    
                    // Update Current XML to new IR setting
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
        DBG("RESIZED IS RUN JSUT NOW");
        
        impulseLabel.setBounds(290, 150, 140, 50);
        
        loadBtn.setBounds(488, 244, 130, 45);
        irName.setBounds(260, 383, 200, 50);
        
        
        if (audioProcessor.isIrBinary)
        {
            irName.setText("", juce::dontSendNotification);
            binaryIrChooser.setSelectedId(audioProcessor.currentBinaryIrId);
        }
        else
        {
            if (audioProcessor.validIrLoaded == false) // If the initally loaded Ir is invalid
            {
                irName.setText("Invalid IR Loaded!", juce::dontSendNotification);
            }
            else
            {
                irName.setText(audioProcessor.lastIrName, juce::dontSendNotification);
            }
        }
        
        irToggleImage.setBounds(335, 435, 50, 50);
        
        fixButton.setBounds(310, 470, 100, 50);
        
        binaryIrChooser.setBounds(100, 230, 200, 25);
        
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
            
            
            if (audioProcessor.isIrBinary)
            {
                DBG("gui got to binary");
                
                irName.setText("", juce::dontSendNotification);
                binaryIrChooser.setSelectedId(audioProcessor.currentBinaryIrId);
                
            }
            else
            {
                if (audioProcessor.validIrLoaded == false)
                {
                    fixButton.setVisible(true); // show button to locate missing ir
                    irName.setText("Invalid IR Loaded!", juce::dontSendNotification);
                }
                else
                {
                    DBG("setting ir name based on file" + audioProcessor.lastIrName);
                    audioProcessor.isIrBinary = false;
                    
                    auto newIRName = audioProcessor.lastIrName;
                    irName.setText(newIRName.toStdString(), juce::dontSendNotification);
                    DBG("Newly set text:::: " + irName.getText());
                    fixButton.setVisible(false);
                    binaryIrChooser.setSelectedId(0);
                }
            }
        }
    }
    
    
    
    void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override
    {
        if (comboBoxThatHasChanged == &binaryIrChooser)
        {
            int selectedId = binaryIrChooser.getSelectedId();
            audioProcessor.currentBinaryIrId = selectedId;
            
            if (selectedId != 0)
            {
                audioProcessor.loadBinaryIr(selectedId);
                audioProcessor.currentBinaryIrName = audioProcessor.getBinaryIrName(selectedId);
                audioProcessor.isIrBinary = true;
                irName.setText("", juce::dontSendNotification);
            }
            else
            {
                audioProcessor.isIrBinary = false;
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
    
    
    juce::ComboBox binaryIrChooser;
    
};
