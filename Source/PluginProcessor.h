/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>


//==============================================================================
/**
*/
class ArtisianDSPAudioProcessor
: public juce::AudioProcessor, public juce::ValueTree::Listener
    
{
public:
    
    //==============================================================================
    ArtisianDSPAudioProcessor();
    ~ArtisianDSPAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    float getRmsValue(const int channel) const;

    //==============================================================================
    float inputGainFloat{ 0.0f };
    float outputGainFloat{ 0.0f };
    bool usingGate = false;
    
    bool shouldUpdate = true;
    
    juce::AudioProcessorValueTreeState apvts;
    void valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyChanged, const juce::Identifier& property) override;
    
private:
    //==============================================================================
    ArtisianDSPAudioProcessor* editor;
    
    juce::AudioProcessorValueTreeState::ParameterLayout createParameters();
    
    juce::LinearSmoothedValue<float> rmsLevelLeft, rmsLevelRight;
    
    
    juce::dsp::NoiseGate<float> noiseGate;
    
    
    juce::LinearSmoothedValue<float> rmsOutputLevelLeft, rmsOutputLevelRight;
    
    
    //==============================================================================
    
    float thresholdValue = -20;
    
    float attackTime = 50 * 0.001;
    
    float releaseTime = 50 * 0.001;
    
    
    
    double attackRate = 0.0;
    double releaseRate = 0.0;
    double averagingBufferDuration = 0.015;
    double averagedValue;
    
    
    int currentBufferIndex = 0;
    double gateMultiplier = 0;
    double openTime = 0.0;
    bool isOpen = false;
    std::vector<float> averagingBuffer;
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ArtisianDSPAudioProcessor)
};
