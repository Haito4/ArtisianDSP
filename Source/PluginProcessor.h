/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PresetManager.h"


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
    
    juce::AudioProcessorValueTreeState apvts;
    void valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyChanged, const juce::Identifier& property) override;
    
    juce::ValueTree variableTree;
    
    juce::ValueTree variableTree2{ "irUpdates" };
    
    
    float getRmsValue(const int channel) const;
    
    Service::PresetManager& getPresetManager() { return *presetManager; }

    //==============================================================================
    float inputGainFloat{ 0.0f };
    
    float outputGainFloat{ 0.0f };
    
    
    bool shouldUpdate = true;
    
    bool shouldLoadIr = false;
    
    
    // Using Effects?
    bool usingGate = false;
    bool usingComp = false;
    bool usingTS = false;
    bool usingAmp = false;
    bool usingIR = false;
    bool usingVerb;
    
    // Cabinet
    juce::dsp::Convolution speakerModule;
    
    juce::File root, savedFile;
    
    // Preset Stuff
    juce::String lastIrName = "null";
    juce::String lastIrPath = "null";
    bool irFound;
    bool validIrLoaded = false;
    juce::String presetName;
    
    void loadBinaryIr(int impulseId);
    
    bool isIrBinary = false;
    bool shouldDoBinaryGUI = false;
    int currentBinaryIrId;
    
    
    
private:
    //==============================================================================
    ArtisianDSPAudioProcessor* editor;
    
    juce::AudioProcessorValueTreeState::ParameterLayout createParameters();
    
    juce::LinearSmoothedValue<float> rmsLevelLeft, rmsLevelRight;
    
    juce::LinearSmoothedValue<float> rmsOutputLevelLeft, rmsOutputLevelRight;
    
    
    std::unique_ptr<Service::PresetManager> presetManager;
    
    
    juce::ListenerList<Listener> listeners;
    
    
    //==============================================================================
    
    // Noise Gate
    juce::dsp::NoiseGate<float> noiseGate;
    
    // Compressor
    juce::dsp::Compressor<float> comPressor;
    
    float compAttack = 50 * 0.001;
    float compRelease = 50 * 0.001;
    int compRatio = 1;
    juce::dsp::Gain<float> compressorLevel;
    
    // Tube Screamer
    juce::dsp::IIR::Filter<float> highPassFilter;
    float tscutoffFrequency;
    float tsDrive;
    float tsLevel;
    
    // Amp
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
    
    
    float roomsize;
    float damping;
    float width;
    
    
    // Cabinet
    juce::dsp::Gain<float> speakerCompensate;
    bool switchEnator = false;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ArtisianDSPAudioProcessor)
};


