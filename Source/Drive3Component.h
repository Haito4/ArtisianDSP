#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class ArtisianDSPAudioProcessor;

class Drive3Component : public juce::Component
{
public:
    Drive3Component(ArtisianDSPAudioProcessor& processor);
    ~Drive3Component();
    
    void resized() override;
private:
    ArtisianDSPAudioProcessor& audioProcessor;
    
    juce::Label driveLabel;
    
    std::unique_ptr<juce::Slider> driveKnob;
    std::unique_ptr<juce::Slider> toneKnob;
    std::unique_ptr<juce::Slider> volumeKnob;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> driveAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> toneAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> volumeAttachment;
};
