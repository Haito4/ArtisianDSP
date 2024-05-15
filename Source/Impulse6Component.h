#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class Impulse6Component : public juce::Component,
                          public juce::Button::Listener
{
public:
    Impulse6Component(ArtisianDSPAudioProcessor& processor) : audioProcessor(processor)
    {
        impulseLabel.setFont(20.f);
        impulseLabel.setJustificationType(juce::Justification::centred);
        impulseLabel.setText("Impulse Response", juce::dontSendNotification);
        addAndMakeVisible(impulseLabel);
        
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
                    
                    
                    
//                    audioProcessor.speakerModule.reset();
                    audioProcessor.speakerModule.loadImpulseResponse(result, juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::yes, 0);
                    irName.setText(result.getFileName(), juce::dontSendNotification);
                }
            });
        };
        
        addAndMakeVisible(irName);
        
        
        
        
    }
//    ~Impulse6Component();
    
    virtual void resized() override
    {
        impulseLabel.setBounds(290, 150, 140, 50);
        
        
        loadBtn.setBounds(335, 325, 100, 50);
        irName.setBounds(335, 375, 100, 50);
        
        irToggleImage.setBounds(335, 450, 50, 50);
    }
    
    void buttonClicked(juce::Button* button) override
    {
    
    }
private:
    ArtisianDSPAudioProcessor& audioProcessor;
    
    
    juce::ImageButton irToggleImage;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> irToggleAttachment;
    
    
    
    juce::TextButton loadBtn;
    std::unique_ptr<juce::FileChooser> fileChooser;
    juce::Label irName;
    
    
    juce::Label impulseLabel;
};
