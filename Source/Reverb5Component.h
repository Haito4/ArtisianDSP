#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class Reverb5Component : public juce::Component,
                         public juce::Button::Listener,
                         public juce::Slider::Listener
{
public:
    Reverb5Component(ArtisianDSPAudioProcessor& processor) : audioProcessor(processor), tooltipWindow(this, 900)
    {
        reverbLabel.setFont(20.f);
        reverbLabel.setJustificationType(juce::Justification::centred);
        reverbLabel.setText("Reverb", juce::dontSendNotification);
        addAndMakeVisible(reverbLabel);
        
        
        
        // Bypass
        addAndMakeVisible(reverbToggleImage);
        reverbToggleImage.addListener(this);
        reverbToggleImage.setImages(false, true, true, juce::ImageCache::getFromMemory(BinaryData::dogreen_png, BinaryData::dogreen_pngSize), 0.5f, juce::Colours::green,
                                                      juce::ImageCache::getFromMemory(BinaryData::dohover_png, BinaryData::dohover_pngSize), 0.5f, juce::Colours::blue,
                                                      juce::ImageCache::getFromMemory(BinaryData::dored_png, BinaryData::dored_pngSize), 0.5f, juce::Colours::red);
        reverbToggleImage.setClickingTogglesState(true);
        reverbToggleAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "USING_VERB", reverbToggleImage);
        reverbToggleImage.setTooltip("Turns the reverb On/Off.");
        
        
        // Size
        addAndMakeVisible (sizeKnob);
        sizeKnob.setTooltip("Adjusts the size of the room in which the reverb occurs. Increase for longer decay times, decrease for shorter ones.");
        sizeKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        
        sizeKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 15);
        sizeLabel.setText ("Size", juce::NotificationType::dontSendNotification);
        sizeLabel.attachToComponent (&sizeKnob, false);
        sizeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "VERB_SIZE", sizeKnob);
        
        
        // Damping
        addAndMakeVisible (dampKnob);
        dampKnob.setTooltip("Controls the rate at which high frequencies decay. Lower values result in a brighter sound.");
        dampKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        
        dampKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 15);
        dampLabel.setText ("Damp", juce::NotificationType::dontSendNotification);
        dampLabel.attachToComponent (&dampKnob, false);
        dampAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "VERB_DAMPING", dampKnob);
        
        
        // Width
        addAndMakeVisible (widthKnob);
        widthKnob.setTooltip("Controls the stereo width of the reverb. Increase for more ambiance.");
        widthKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        
        widthKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 15);
        widthLabel.setText ("Width", juce::NotificationType::dontSendNotification);
        widthLabel.attachToComponent (&widthKnob, false);
        widthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "VERB_WIDTH", widthKnob);
        
        // Blend
        addAndMakeVisible (dwKnob);
        dwKnob.setTooltip("Adjusts the balance between the dry (original) and wet (reverberated) signals. Increase to emphasize the reverb effect, decrease to retain more of the original signal.");
        dwKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        
        dwKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 15);
        dwLabel.setText ("Dry/Wet", juce::NotificationType::dontSendNotification);
        dwLabel.attachToComponent (&dwKnob, false);
        dwAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "VERB_DRYWET", dwKnob);
        
        
        
    }
//    ~Reverb5Component();
    
    virtual void resized() override
    {
//        reverbLabel.setBounds(getLocalBounds());
        
        
        
        reverbToggleImage.setBounds(640, 305, 50, 50);
        
        sizeKnob.setBounds (102, 245, 100, 100);
        dampKnob.setBounds (206, 245, 100, 100);
        widthKnob.setBounds (310, 245, 100, 100);
        dwKnob.setBounds (414, 245, 100, 100);
        
        
    }
    
    void buttonClicked(juce::Button* button) override
    {
    }
    
    void sliderValueChanged(juce::Slider* slider) override
    {
    }
private:
    ArtisianDSPAudioProcessor& audioProcessor;
    
    juce::TooltipWindow tooltipWindow;
    
    juce::ImageButton reverbToggleImage;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> reverbToggleAttachment;
    
    
    juce::Label sizeLabel,
                dampLabel,
                widthLabel,
                dwLabel;
    juce::Slider sizeKnob,
                 dampKnob,
                 widthKnob,
                 dwKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>  sizeAttachment,
                                                                           dampAttachment,
                                                                           widthAttachment,
                                                                           dwAttachment;
    
    juce::Label reverbLabel;
};
