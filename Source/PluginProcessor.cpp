/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ArtisianDSPAudioProcessor::ArtisianDSPAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this, nullptr, "Parameters", createParameters())
#endif
{
    apvts.state.addListener(this);
}

juce::AudioProcessorValueTreeState::ParameterLayout ArtisianDSPAudioProcessor::createParameters()
{
    juce::AudioProcessorValueTreeState::ParameterLayout params;
    
    params.add(std::make_unique<juce::AudioParameterFloat>("THRESHOLD", "Threshold", -96.0f, 6.0f, 1.0f));
    params.add(std::make_unique<juce::AudioParameterFloat>("RATIO", "Ratio", 1.0f, 10.0f, 1.0f));
    params.add(std::make_unique<juce::AudioParameterFloat>("ATTACK", "Attack", 1.0f, 300.0f, 20.0f));
    params.add(std::make_unique<juce::AudioParameterFloat>("RELEASE", "Release", 1.0f, 700.0f, 20.0f));
    
    return params;
}

ArtisianDSPAudioProcessor::~ArtisianDSPAudioProcessor()
{
}

//==============================================================================
const juce::String ArtisianDSPAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ArtisianDSPAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ArtisianDSPAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ArtisianDSPAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ArtisianDSPAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ArtisianDSPAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ArtisianDSPAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ArtisianDSPAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ArtisianDSPAudioProcessor::getProgramName (int index)
{
    return {};
}

void ArtisianDSPAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void ArtisianDSPAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    rmsLevelLeft.reset(sampleRate, 0.5);
    rmsLevelRight.reset(sampleRate, 0.5);
    rmsLevelLeft.setCurrentAndTargetValue(-100.f);
    rmsLevelRight.setCurrentAndTargetValue(-100.f);
    
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();
    
}

void ArtisianDSPAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ArtisianDSPAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif


void ArtisianDSPAudioProcessor::valueTreePropertyChanged(juce::ValueTree &treeWhosePropertyChanged, const juce::Identifier &property)
{
    shouldUpdate = true;
    

    auto thresholdValue = apvts.getRawParameterValue("THRESHOLD")->load();
    juce::Logger::outputDebugString("Threshold value: " + juce::String(thresholdValue));
    
    
}

void ArtisianDSPAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    if (shouldUpdate)
    {
        thresholdValue = static_cast<float>(*apvts.getRawParameterValue("THRESHOLD"));
        juce::Logger::outputDebugString("Threshold variable: " + juce::String(thresholdValue));
        
        shouldUpdate = false;
    }
    
    
    
    //==============================================================================
    // RMS Level for input meter
    rmsLevelLeft.skip(buffer.getNumSamples());
    rmsLevelRight.skip(buffer.getNumSamples());
    // RMS Left Channel
    {
        const auto value = juce::Decibels::gainToDecibels(buffer.getRMSLevel(0, 0, buffer.getNumSamples()));
        if (value < rmsLevelLeft.getCurrentValue()) // if value less than current RMS value
            rmsLevelLeft.setTargetValue(value); // then go toward value steadily
        else
            rmsLevelLeft.setCurrentAndTargetValue(value); // otherwise immediately jump to higher value
        
    }
    // RMS Right Channel
    {
        const auto value = juce::Decibels::gainToDecibels(buffer.getRMSLevel(1, 0, buffer.getNumSamples()));
        if (value < rmsLevelRight.getCurrentValue())
            rmsLevelRight.setTargetValue(value);
        else
            rmsLevelRight.setCurrentAndTargetValue(value);
    }
    //==============================================================================
    

    // Clears any output channels that didn't contain input data
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    // Main Processing
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        // ..do something to the data...
        auto* channelData = buffer.getWritePointer (channel);
            
        for (int sample = 0; sample < buffer.getNumSamples(); sample++)
        {
            // Input Gain
            channelData[sample] = channelData[sample] * juce::Decibels::decibelsToGain(inputGainFloat);

            
            // Noise Gate
            if (usingGate == true)
            {
                // This works currently, but audio gets crackly when it gets close to but not to the threshold
                // Could solve with hysteris
                float sampleDb = juce::Decibels::gainToDecibels(std::abs(channelData[sample]));
                if (sampleDb < thresholdValue)
                    {
                        // Calculate the reduction in gain based on how far below the threshold the sample is
                        float reductionDb = thresholdValue - sampleDb;
                        
                        // If the reduction in gain exceeds a certain threshold, mute the sample
                        if (reductionDb > -12.0f)
                        {
                            channelData[sample] = 0.0f;
                        }
                    }
            }
            
            
            // Output Gain
            channelData[sample] = channelData[sample] * juce::Decibels::decibelsToGain(outputGainFloat);
            
        }
    }
}





//==============================================================================
bool ArtisianDSPAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ArtisianDSPAudioProcessor::createEditor()
{
    // Standard Editor
    return new WrappedRasterAudioProcessorEditor (*this);
    
    //Parameter Editor
//    return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void ArtisianDSPAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    
    
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    
}

void ArtisianDSPAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    
}

float ArtisianDSPAudioProcessor::getRmsValue(const int channel) const
{
    jassert(channel == 0 || channel == 1);
    if (channel == 0)
        return rmsLevelLeft.getCurrentValue();
    if (channel == 1)
        return rmsLevelRight.getCurrentValue();
    return 0.f;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ArtisianDSPAudioProcessor();
}
