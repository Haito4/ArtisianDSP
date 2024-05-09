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
        
    }
//    ~Impulse6Component();
    
    virtual void resized() override
    {
        impulseLabel.setBounds(290, 350, 140, 50);
        
        irToggleImage.setBounds(335, 450, 50, 50);
    }
    
    void buttonClicked(juce::Button* button) override
    {
    
    }
private:
    ArtisianDSPAudioProcessor& audioProcessor;
    
    
    juce::ImageButton irToggleImage;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> irToggleAttachment;
    
    juce::Label impulseLabel;
};
