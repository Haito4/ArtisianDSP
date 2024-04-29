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
                       .withInput  ("Input",  juce::AudioChannelSet::mono(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this, nullptr, "Parameters", createParameters())
#endif
{
    apvts.state.addListener(this);
    
    isOpen = true;
    
    double sampleRate = getSampleRate();
    int bufferSize = static_cast<int>(0.015 * sampleRate);
    averagingBuffer.resize(bufferSize, 0.0f);

}

juce::AudioProcessorValueTreeState::ParameterLayout ArtisianDSPAudioProcessor::createParameters()
{
    juce::AudioProcessorValueTreeState::ParameterLayout params;
    
    // Input & Output Gain
    params.add(std::make_unique<juce::AudioParameterFloat>("INPUT_GAIN", "Input Gain", -15.f, 15.f, 0.f));
    params.add(std::make_unique<juce::AudioParameterFloat>("OUTPUT_GAIN", "Output Gain", -15.f, 15.f, 0.f));
    
    // Noise Gate
    params.add(std::make_unique<juce::AudioParameterBool>("USING_GATE", "Using Gate", false));
    params.add(std::make_unique<juce::AudioParameterFloat>("THRESHOLD", "Threshold", -100.0f, 6.0f, -20.0f));
    params.add(std::make_unique<juce::AudioParameterFloat>("RATIO", "Ratio", 1.0f, 10.0f, 1.0f));
    params.add(std::make_unique<juce::AudioParameterFloat>("ATTACK", "Attack", 1.0f, 100.0f, 50.0f));
    params.add(std::make_unique<juce::AudioParameterFloat>("RELEASE", "Release", 1.0f, 100.0f, 50.0f));
    
    // Compressor
    params.add(std::make_unique<juce::AudioParameterBool>("USING_COMP", "Using Compressor", false));
    params.add(std::make_unique<juce::AudioParameterFloat>("COMP_THRES", "Compressor Threshold", -100.0f, 6.0f, -20.0f));
    params.add(std::make_unique<juce::AudioParameterFloat>("COMP_ATTACK", "Compressor Attack", 1.0f, 100.0f, 50.0f));
    params.add(std::make_unique<juce::AudioParameterFloat>("COMP_RELEASE", "Compressor Release", 1.0f, 100.0f, 50.0f));
    params.add(std::make_unique<juce::AudioParameterFloat>("COMP_RATIO", "Compressor Ratio", 1.0f, 8.0f, 1.0f));
    
    // Tube Screamer
    params.add(std::make_unique<juce::AudioParameterBool>("USING_TS", "Using Tube Screamer", false));
    params.add(std::make_unique<juce::AudioParameterFloat>("TS_DRIVE", "Tube Screamer Drive", 1.f, 500.f, 1.f));
    params.add(std::make_unique<juce::AudioParameterFloat>("TS_TONE", "Tube Screamer Tone", 20.f, 700.f, 20.f));
    params.add(std::make_unique<juce::AudioParameterFloat>("TS_LEVEL", "Tube Screamer Level", 0.f, 1.f, 0.5f));
    
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
    // Noise Gate
    int bufferSize = static_cast<int>(0.015 * sampleRate);
    averagingBuffer.resize(bufferSize, 0.0f);
    attackRate = 1 / (sampleRate * attackTime);
    releaseRate = 1 / (sampleRate * releaseTime);


    // RMS
    rmsLevelLeft.reset(sampleRate, 0.5);
    rmsLevelRight.reset(sampleRate, 0.5);
    rmsLevelLeft.setCurrentAndTargetValue(-100.f);
    rmsLevelRight.setCurrentAndTargetValue(-100.f);
    
    
    // DSP Initialisation
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumInputChannels();
        // Tube Screamer
    highPassFilter.reset();
    highPassFilter.prepare(spec);
        // Compressor
    comPressor.setThreshold(-20.f);
    comPressor.setAttack(4.f);
    comPressor.setRelease(10.f);
    comPressor.setRatio(100.f);
    comPressor.prepare(spec);
}

void ArtisianDSPAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ArtisianDSPAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    if (layouts.getMainInputChannelSet() != juce::AudioChannelSet::mono())
        return false;

    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    return true;
}
#endif


void ArtisianDSPAudioProcessor::valueTreePropertyChanged(juce::ValueTree &treeWhosePropertyChanged, const juce::Identifier &property)
{
    // Input & Output Gain
    if (property == juce::Identifier("INPUT_GAIN"))
    {
        if (*apvts.getRawParameterValue("INPUT_GAIN") == -15.0f)
        {
            inputGainFloat = -1000.0f; // Mute at minimum value
        }
        else
        {
            inputGainFloat = *apvts.getRawParameterValue("INPUT_GAIN");
        }
    }
    else if (property == juce::Identifier("OUTPUT_GAIN"))
    {
        outputGainFloat = *apvts.getRawParameterValue("OUTPUT_GAIN");
    }
    
    else if (property == juce::Identifier("USING_GATE"))
    {
        juce::Logger::outputDebugString("usingGate: " + juce::String(usingGate ? "true" : "false"));
    }
    
   
    
    
    
    shouldUpdate = true;
}

void ArtisianDSPAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    if (shouldUpdate)
    {
        // Noise Gate
        usingGate = static_cast<bool>(*apvts.getRawParameterValue("USING_GATE"));
        thresholdValue = juce::Decibels::decibelsToGain(static_cast<float>(*apvts.getRawParameterValue("THRESHOLD")));
        attackTime = static_cast<float>(*apvts.getRawParameterValue("ATTACK")) * 0.001; // Convert value to seconds
        releaseTime = static_cast<float>(*apvts.getRawParameterValue("RELEASE")) * 0.001;
        attackRate = 1 / (getSampleRate() * attackTime);
        releaseRate = 1 / (getSampleRate() * releaseTime);
        
        // Compressor
        usingComp = static_cast<bool>(*apvts.getRawParameterValue("USING_COMP"));
        comPressor.setThreshold(static_cast<float>(*apvts.getRawParameterValue("COMP_THRES")));
        comPressor.setAttack(static_cast<float>(*apvts.getRawParameterValue("COMP_ATTACK")));
        comPressor.setRelease(static_cast<float>(*apvts.getRawParameterValue("COMP_RELEASE")));
        comPressor.setRatio(static_cast<float>(*apvts.getRawParameterValue("COMP_RATIO")));
        
        // Overdrive
        usingTS = static_cast<bool>(*apvts.getRawParameterValue("USING_TS"));
        tscutoffFrequency = static_cast<float>(*apvts.getRawParameterValue("TS_TONE"));
        highPassFilter.coefficients = juce::dsp::IIR::Coefficients<float>::makeHighPass(getSampleRate(), tscutoffFrequency);
        
        tsDrive = static_cast<float>(*apvts.getRawParameterValue("TS_DRIVE"));
        tsLevel = static_cast<float>(*apvts.getRawParameterValue("TS_LEVEL"));
        
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
    auto* channelData = buffer.getWritePointer(0);
    for (int sample = 0; sample < buffer.getNumSamples(); sample++)
    {
        // Input Gain
        channelData[sample] = channelData[sample] * juce::Decibels::decibelsToGain(inputGainFloat);

        
        // Noise Gate
        if (usingGate)
        {
            auto current = channelData[sample];
            
            // Adds the squared value of the current sample to the averaging buffer at the current index
            averagingBuffer[currentBufferIndex] = current * current;
            // increment current index while staying within buffer size
            currentBufferIndex = (currentBufferIndex + 1) % averagingBuffer.size();
            
            // Average of the squared values from the averaging buffer for use when comparing to threshold
            float sumSquaredValues = std::accumulate(averagingBuffer.begin(), averagingBuffer.end(), 0.0f);
            float averagedValue = sumSquaredValues / static_cast<float>(averagingBuffer.size());
            
            if (isOpen == true) // If Gate Open
            {
                // Increment by reciprocal of sample rate to track time open
                openTime += 1 / getSampleRate();
                gateMultiplier += attackRate;
                gateMultiplier = juce::jlimit(0.0, 1.0, gateMultiplier); // clamp value within range of 0 and 1
                if ((averagedValue < thresholdValue)) // && (openTime > holdTime)
                {
                    isOpen = false; // Close the Gate
                    openTime = 0.0;
                }
            }
            else // If Gate Closed
            {
                openTime += 1/ getSampleRate();
                gateMultiplier -= releaseRate;
                gateMultiplier = juce::jlimit(0.0, 1.0, gateMultiplier);
                if ((averagedValue > thresholdValue))
                {
                    isOpen = true; // Open the Gate
                    openTime = 0.0;
                }
            }
            
            // Actually apply the changes
            channelData[sample] *= gateMultiplier;
        }
        
        
        // Compressor
        if (usingComp)
        {
            channelData[sample] = comPressor.processSample(0, channelData[sample]);
        }
        
        
        // Overdrive
        if (usingTS)
        {
            // HPF
            channelData[sample] = highPassFilter.processSample(channelData[sample]);
            
            // Distortion
            if (tsDrive > 1)
            {
                float distortedValue = (2.0f / juce::MathConstants<float>::pi) * atan(channelData[sample] * tsDrive);
                const float minOutput = 0.0f;
                const float maxOutput = 0.2f;
                channelData[sample] = juce::jlimit(minOutput, maxOutput, distortedValue);
            }
            
            // Level
            channelData[sample] *= tsLevel;
        }
        
        
        // Amplifier
        
        
        // Reverb
        
        
        
        // Impulse Response
        
        
        // Output Gain
        channelData[sample] = channelData[sample] * juce::Decibels::decibelsToGain(outputGainFloat);
        
        
        
        auto* rightChannelData = buffer.getWritePointer(1);
        rightChannelData[sample] = channelData[sample]; // copy audio data to right side channel
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
