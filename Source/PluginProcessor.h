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
    
    juce::ValueTree variableTree;
    
    
    // Amp
    
    
    
    
    
    // Cabinet
    juce::dsp::Convolution speakerModule;
    
    juce::File root, savedFile;
    
    
private:
    //==============================================================================
    ArtisianDSPAudioProcessor* editor;
    
    juce::AudioProcessorValueTreeState::ParameterLayout createParameters();
    
    juce::LinearSmoothedValue<float> rmsLevelLeft, rmsLevelRight;
    
    

    
    
    juce::LinearSmoothedValue<float> rmsOutputLevelLeft, rmsOutputLevelRight;
    
    //==============================================================================
    
    // Noise Gate
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
    
    // Compressor
    juce::dsp::Compressor<float> comPressor;
    bool usingComp = false;
    float compAttack = 50 * 0.001;
    float compRelease = 50 * 0.001;
    int compRatio = 1;
    juce::dsp::Gain<float> compressorLevel;
    
    // Tube Screamer
    juce::dsp::IIR::Filter<float> highPassFilter;
    float tscutoffFrequency;
    bool usingTS = false;
    float tsDrive;
    float tsLevel;
    
    // Amp
    bool usingAmp = false;
    
    juce::dsp::Gain<float> ampInputGain;
    juce::dsp::WaveShaper<float> waveshaper;
    
    juce::dsp::IIR::Filter<float> tight;
    float tightEnabled;
    
    
    juce::dsp::LadderFilter<float> resonanceFilter;
    
    float presenceEQ;
    juce::dsp::IIR::Filter<float> presenceFilter;
    
    
    
    float ampOD;
    float ampGain;
    float masterVol = 1;
    
    
    
    float treble = 1;
    float mids = 1;
    float bass = 1;
    

    
    juce::dsp::IIR::Filter<float> lowPeak;
    juce::dsp::IIR::Filter<float> midPeak;
    juce::dsp::IIR::Filter<float> highPeak;
    
    
    juce::dsp::Gain<float> ampOutputGain;
    
    
    
    // Reverb
   juce::dsp::Reverb verbL, verbR;
   juce::dsp::Reverb::Parameters verbParams;
   
   bool usingVerb;
   float roomsize;
   float damping;
   float width;
    
    
    // Cabinet
    bool usingIR = false;
    juce::dsp::Gain<float> speakerCompensate;
    
    
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ArtisianDSPAudioProcessor)
};


