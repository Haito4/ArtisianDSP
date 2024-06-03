#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class ArtisianDSPAudioProcessor;

namespace Service
{
    class PresetManager : juce::ValueTree::Listener
    {
    public:
        static const juce::File defaultDirectory;
        static const juce::String extension;
        static const juce::String presetNameProperty;
        
        PresetManager(juce::AudioProcessorValueTreeState&, ArtisianDSPAudioProcessor&);
        
        void savePreset(const juce::String& presetName);
        void deletePreset(const juce::String& presetName);
        void loadPreset(const juce::String& presetName);
        int loadNextPreset();
        int loadPreviousPreset();
        juce::StringArray getAllPresets() const;
        juce::String getCurrentPreset() const;
        
        
    private:
        bool aCounter = false;
        
        void valueTreeRedirected(juce::ValueTree& treeWhichHasBeenChanged) override;
        
        juce::AudioProcessorValueTreeState& valueTreeState;
        ArtisianDSPAudioProcessor& audioProcessor;
        
        juce::Value currentPreset;
    };
}
