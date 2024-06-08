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
    
    variableTree = {
        
        "Variables", {},
        {
            { "Group", {{ "name", "IR Vars" }},
                {
                    { "Parameter", {{ "id", "file1" }, { "value", "/" }}},
                    { "Parameter", {{ "id", "root" }, { "value", "/" }}},
                }
            }
        }
    };
    
    
    
    apvts.state.setProperty(Service::PresetManager::presetNameProperty, "", nullptr);
    apvts.state.setProperty("version", ProjectInfo::versionString, nullptr);
    presetManager = std::make_unique<Service::PresetManager>(apvts, *this);
    
//    apvts.state.setProperty("IRPath", "", nullptr);
//    apvts.state.setProperty("IRName", "", nullptr);
    
    
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
    params.add(std::make_unique<juce::AudioParameterFloat>("COMP_LEVEL", "Compressor Level", -20.f, 10.f, 0.f));
    
    // Tube Screamer
    params.add(std::make_unique<juce::AudioParameterBool>("USING_TS", "Using Tube Screamer", false));
    params.add(std::make_unique<juce::AudioParameterFloat>("TS_DRIVE", "Tube Screamer Drive", 1.f, 500.f, 1.f));
    params.add(std::make_unique<juce::AudioParameterFloat>("TS_TONE", "Tube Screamer Tone", 20.f, 700.f, 20.f));
    params.add(std::make_unique<juce::AudioParameterFloat>("TS_LEVEL", "Tube Screamer Level", 0.f, 1.f, 0.5f));
    
    // Amplifier
    params.add(std::make_unique<juce::AudioParameterBool>("USING_AMP", "Using Amplifier", false));
    params.add(std::make_unique<juce::AudioParameterFloat>("AMP_INPUTGAIN", "Amplifier Input Gain", 0.f, 40.f, 0.f));
    params.add(std::make_unique<juce::AudioParameterBool>("AMP_TIGHT", "Amp Tight", false));
    
    
    params.add(std::make_unique<juce::AudioParameterFloat>("AMP_PRESENCE", "Amplifier Presence", 0.5f, 1.5f, 1.0f));
    params.add(std::make_unique<juce::AudioParameterFloat>("AMP_RESONANCE", "Amplifier Resonance", 1.f, 10.f, 5.f));
    
    
    params.add(std::make_unique<juce::AudioParameterFloat>("AMP_DRIVE", "Amplifier Drive", 0.f, 1.f, 0.5f));
    
    params.add(std::make_unique<juce::AudioParameterFloat>("AMP_BASS", "Amp Lows", 0.f, 2.f, 1.f));
    params.add(std::make_unique<juce::AudioParameterFloat>("AMP_MIDS", "Amp Middle", 0.f, 2.f, 1.f));
    params.add(std::make_unique<juce::AudioParameterFloat>("AMP_HI", "Amp Highs", 0.f, 2.f, 1.f));
    
    params.add(std::make_unique<juce::AudioParameterFloat>("AMP_MASTER", "Amp Master Volume", 0.f, 2.f, 1.f));
    params.add(std::make_unique<juce::AudioParameterFloat>("AMP_OUTPUTGAIN", "Amplifier Output Gain", -10.f, 10.f, 0.f));
    
    // Reverb
    params.add(std::make_unique<juce::AudioParameterBool>("USING_VERB", "Using Reverb", false));
    params.add(std::make_unique<juce::AudioParameterFloat>("VERB_SIZE", "Reverb Size", juce::NormalisableRange<float>(0.0f, 1.0f, 0.001f, 1.0f), 0.5f));
    params.add(std::make_unique<juce::AudioParameterFloat>("VERB_DAMPING", "Reverb Damping", juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f, 1.0f), 0.5f));
    params.add(std::make_unique<juce::AudioParameterFloat>("VERB_WIDTH", "Reverb Width", juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f, 1.0f), 0.5f));
    params.add(std::make_unique<juce::AudioParameterFloat>("VERB_DRYWET", "Reverb Dry/Wet", juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f, 1.0f), 0.5f));

    
    // Impulse Response
    params.add(std::make_unique<juce::AudioParameterBool>("USING_IR", "Using Impulse Response", false));
    params.add(std::make_unique<juce::AudioParameterFloat>("IR_VOLUME", "IR Volume", 0.f, 2.f, 1.f));
    
    
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
    
    
    // Spec Initialisation
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = 2;
    
    //-----------------------------------------------------------------
    
    // Tube Screamer
    highPassFilter.reset();
    highPassFilter.prepare(spec);
    
    //-----------------------------------------------------------------
    
    // Compressor
    comPressor.setThreshold(-20.f);
    comPressor.setAttack(4.f);
    comPressor.setRelease(10.f);
    comPressor.setRatio(100.f);
    comPressor.prepare(spec);
    
    compressorLevel.prepare(spec);
    speakerCompensate.setGainDecibels(0.0);
    
    //-----------------------------------------------------------------
    
    // Amplifier
    ampInputGain.prepare(spec);
    
    waveshaper.functionToUse = [] (float x) {
        float a, x2, y;
        x = x * 0.25f;
        a = std::abs (x);
        x2 = x * x;
        y = 1 - 1 / (1 + a + x2 + 0.66422417311781f * x2 * a + 0.36483285408241f * x2 * x2);
        return (x >= 0) ? y : -y;
    };
    
    
    tight.prepare(spec);
    
    resonanceFilter.prepare(spec);
//    resonanceFilter.setMode(juce::dsp::LadderFilterMode::LPF12);
//    resonanceFilter.setResonance(0.2f);
//    resonanceFilter.setDrive(1.0f);
//
    presenceFilter.prepare(spec);
    
    bass = juce::jlimit(0.01f, 2.0f, bass);
    mids = juce::jlimit(0.01f, 2.0f, mids);
    treble = juce::jlimit(0.01f, 2.0f, treble);
    
    lowPeak.coefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(getSampleRate(), 100.f, 0.6f, bass);
    midPeak.coefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(getSampleRate(), 500.0f, 0.9f, mids);
    highPeak.coefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(getSampleRate(), 5000.0f, 0.6f, treble);
    
    lowPeak.reset();
    midPeak.reset();
    highPeak.reset();
    lowPeak.prepare(spec);
    midPeak.prepare(spec);
    highPeak.prepare(spec);
    
    ampOutputGain.prepare(spec);
    
    //-----------------------------------------------------------------
    
    // Impulse Response
    speakerModule.prepare(spec);
    
//    speakerModule.loadImpulseResponse(BinaryData::ML_Sound_Labs_BEST_IR_IN_THE_WORLD_wav,
//                                      BinaryData::ML_Sound_Labs_BEST_IR_IN_THE_WORLD_wavSize,
//                                      juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::yes, 0);
//    bestIrInTheWorld.append(BinaryData::ML_Sound_Labs_BEST_IR_IN_THE_WORLD_wav,
//                            BinaryData::ML_Sound_Labs_BEST_IR_IN_THE_WORLD_wavSize);
//    catch33;
//    chaosphere;
//    cCollapse;
//    destroyEraseImprove;
//    eyeIr;
//    immutable;
//    koloss;
//    nothing;
//    nothingRR;
//    obZen;
//    pitchBlack;
//    tVSoR;
    
    
    
    speakerCompensate.prepare(spec);
    speakerCompensate.setRampDurationSeconds(0.02);
    speakerCompensate.setGainDecibels(6.0);
    
    //-----------------------------------------------------------------
    
    // Reverb
    juce::dsp::ProcessSpec spec2;
    spec2.sampleRate = sampleRate;
    spec2.maximumBlockSize = samplesPerBlock;
    spec2.numChannels = 1;

    verbL.prepare(spec2);
    verbR.prepare(spec2);
    
    verbParams.roomSize = roomsize;
    verbParams.damping = damping;
    verbParams.width = width;
    
    //-----------------------------------------------------------------
    

    shouldUpdate = true;
}

void ArtisianDSPAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ArtisianDSPAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
//    if (layouts.getMainInputChannelSet() != juce::AudioChannelSet::mono())
//        return false;

    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
//        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
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
        compressorLevel.setGainDecibels(*apvts.getRawParameterValue("COMP_LEVEL"));
        
        // Overdrive
        usingTS = static_cast<bool>(*apvts.getRawParameterValue("USING_TS"));
        tscutoffFrequency = static_cast<float>(*apvts.getRawParameterValue("TS_TONE"));
        highPassFilter.coefficients = juce::dsp::IIR::Coefficients<float>::makeHighPass(getSampleRate(), tscutoffFrequency);
        tsDrive = static_cast<float>(*apvts.getRawParameterValue("TS_DRIVE"));
        tsLevel = static_cast<float>(*apvts.getRawParameterValue("TS_LEVEL"));
        
        // Amp
        usingAmp = static_cast<bool>(*apvts.getRawParameterValue("USING_AMP"));
        
        ampInputGain.setGainDecibels(*apvts.getRawParameterValue("AMP_INPUTGAIN"));
        ampOutputGain.setGainDecibels(*apvts.getRawParameterValue("AMP_OUTPUTGAIN"));
        
        ampOD = static_cast<float>(*apvts.getRawParameterValue("AMP_DRIVE"));
//        ampOD = juce::jlimit(0.001f, 1.0f, ampOD);
        
        
        tight = juce::dsp::IIR::Coefficients<float>::makeLowShelf(getSampleRate(), 400.0f, 0.4f, 0.4f);
        tightEnabled = static_cast<bool>(*apvts.getRawParameterValue("AMP_TIGHT"));
        
        
        resonanceFilter.setCutoffFrequencyHz(*apvts.getRawParameterValue("AMP_RESONANCE") * 1000.0f);
        
        
        presenceEQ = *apvts.getRawParameterValue("AMP_PRESENCE");
        float centerFrequency = 3000.0f + presenceEQ * 500.0f;
        float qFactor = 0.6f + presenceEQ * 0.05f;
        centerFrequency = juce::jlimit (1000.0f, 6000.0f, centerFrequency);
        qFactor = juce::jlimit (0.1f, 1.0f, qFactor);
        presenceFilter = juce::dsp::IIR::Coefficients<float>::makePeakFilter(getSampleRate(), centerFrequency, qFactor, presenceEQ);
        

        bass = juce::jlimit(0.01f, 2.0f, apvts.getRawParameterValue("AMP_BASS")->load());
        mids = juce::jlimit(0.01f, 2.0f, apvts.getRawParameterValue("AMP_MIDS")->load());
        treble = juce::jlimit(0.01f, 2.0f,apvts.getRawParameterValue("AMP_HI")->load());
        
        lowPeak.coefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(getSampleRate(), 100.f, 0.6f, bass);
        midPeak.coefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(getSampleRate(), 500.0f, 0.9f, mids);
        highPeak.coefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(getSampleRate(), 5000.0f, 0.6f, treble);
        
        masterVol = static_cast<float>(*apvts.getRawParameterValue("AMP_MASTER"));
        
        // IR
        usingIR = static_cast<bool>(*apvts.getRawParameterValue("USING_IR"));
        

        // Reverb
        usingVerb = static_cast<bool>(*apvts.getRawParameterValue("USING_VERB"));
        
        
        verbParams.roomSize = *apvts.getRawParameterValue ("VERB_SIZE");
        verbParams.damping = *apvts.getRawParameterValue ("VERB_DAMPING");
        verbParams.width = *apvts.getRawParameterValue ("VERB_WIDTH");
        verbParams.wetLevel = *apvts.getRawParameterValue ("VERB_DRYWET");
        verbParams.dryLevel = 1.0f - *apvts.getRawParameterValue ("VERB_DRYWET");


        verbL.setParameters(verbParams);
        verbR.setParameters(verbParams);

        
        shouldUpdate = false;
    }
    
    
    if (shouldLoadIr)
    {
        if (isIrBinary)
        {
            loadBinaryIr(currentBinaryIrId);
            DBG("ir should be loaded (binary)");
            
            isIrBinary = false;
            
        }
        else
        {
            speakerModule.loadImpulseResponse(juce::File(lastIrPath.toStdString()),
                                              juce::dsp::Convolution::Stereo::yes,
                                              juce::dsp::Convolution::Trim::yes, 0);
            DBG("ir should be loaded (file)");
        }
        
        shouldLoadIr = false;
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
            channelData[sample] *= gateMultiplier; // apply the changes
        }
        
        
        // Compressor
        if (usingComp)
        {
            channelData[sample] = comPressor.processSample(0, channelData[sample]);
            channelData[sample] = compressorLevel.processSample(channelData[sample]);
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
        if (usingAmp)
        {
            // Resonance
//            channelData[sample] = resonanceFilter.processSample(channelData[sample], 0);
            
            channelData[sample] = tight.processSample(channelData[sample]);
            
            
            // Preamp
            channelData[sample] = ampInputGain.processSample(channelData[sample]);
            channelData[sample] = waveshaper.processSample(channelData[sample]);
//            float output = (3 + (ampOD * 250)) * channelData[sample] / (std::abs(channelData[sample] * (ampOD * 250)) + 1); // soft clipping
//            channelData[sample] = juce::jlimit(-1.0f, 1.0f, output);
            

            if (tightEnabled)
            {
            }
            
            // Prescence
            channelData[sample] = presenceFilter.processSample(channelData[sample]);
            
            
            // Equaliser
            channelData[sample] = lowPeak.processSample(channelData[sample]);
            channelData[sample] = midPeak.processSample(channelData[sample]);
            channelData[sample] = highPeak.processSample(channelData[sample]);
            
            
            channelData[sample] = ampOutputGain.processSample(channelData[sample]);
            
            channelData[sample] = channelData[sample] * masterVol;
            
            channelData[sample] = juce::jlimit(0.0f, 0.35f * masterVol, channelData[sample]);
        }
        
        
        
       
        
        
        // Output Gain
        channelData[sample] = channelData[sample] * juce::Decibels::decibelsToGain(outputGainFloat);
        
        
        auto* rightChannelData = buffer.getWritePointer(1);
        rightChannelData[sample] = channelData[sample]; // copy audio data to right side channel
    }
    
    
    // Reverb
    if (usingVerb)
    {
        juce::dsp::AudioBlock<float> block2 (buffer);
        auto leftBlock = block2.getSingleChannelBlock (0);
        auto rightBlock = block2.getSingleChannelBlock (1);
        
        juce::dsp::ProcessContextReplacing<float> leftContext (leftBlock);
        juce::dsp::ProcessContextReplacing<float> rightContext (rightBlock);
        
        verbL.process(leftContext);
        verbR.process(rightContext);
    }
    
    
    if (usingIR) {
        juce::dsp::AudioBlock<float> block {buffer};
        speakerModule.process(juce::dsp::ProcessContextReplacing<float>(block));
        speakerCompensate.process(juce::dsp::ProcessContextReplacing<float>(block)); // boost by 6dB
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
    
    const auto state = apvts.copyState();
    const auto xml(state.createXml());

    DBG("Getting state information...");
    
    
    copyXmlToBinary(*xml, destData);
    
}

void ArtisianDSPAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    DBG("Setting State information...");
    
    
    
    const auto xmlState = getXmlFromBinary(data, sizeInBytes);
    if (xmlState == nullptr)
        return;
    
    
    DBG("passed xmlstate nullptr");
    const auto newTree = juce::ValueTree::fromXml(*xmlState);
    apvts.replaceState(newTree);
    
    DBG("breakpoint 2");
    
    shouldUpdate = true;
    
    if (!juce::File(presetManager->getCurrentPresetPath()).exists())
        return;
    
    
    // Ir loading from preset
    presetManager->getCurrentPresetIrType();
    
    DBG("ir path type gotten");
    
    
    if (isIrBinary)
    {
        presetManager->getCurrentPresetIrId();
        shouldLoadIr = true;
        validIrLoaded = true;
        DBG("Loading initial IR from Binary Data");
    }
    else
    {
        presetManager->getCurrentPresetIr();
        
        if (juce::File(lastIrPath).existsAsFile())
        {
            validIrLoaded = true;
            shouldLoadIr = true;
            DBG("Loading initial ir from preset");

        }
        else
        {
            validIrLoaded = false;
            shouldLoadIr = false;
            DBG("Invalid initial IR");
            
            
        }
    }
    
    
}




void loadBinaryIr(int impulseId)
{
    
}


void ArtisianDSPAudioProcessor::loadBinaryIr(int impulseId)
{
    juce::MemoryBlock irMemoryBlock;
    const char* data = nullptr;
    size_t dataSize = 0;

    switch (impulseId)
    {
        case 1:
            data = BinaryData::ML_Sound_Labs_BEST_IR_IN_THE_WORLD_wav;
            dataSize = BinaryData::ML_Sound_Labs_BEST_IR_IN_THE_WORLD_wavSize;
            break;
        case 2:
            data = BinaryData::Contradictions_Collapse_wav;
            dataSize = BinaryData::Contradictions_Collapse_wavSize;
            break;
        case 3:
            data = BinaryData::Destroy_Erase_Improve_wav;
            dataSize = BinaryData::Destroy_Erase_Improve_wavSize;
            break;
        case 4:
            data = BinaryData::Chaosphere_wav;
            dataSize = BinaryData::Chaosphere_wavSize;
            break;
        case 5:
            data = BinaryData::Nothing_wav;
            dataSize = BinaryData::Nothing_wavSize;
            break;
        case 6:
            data = BinaryData::Nothing_2006_wav;
            dataSize = BinaryData::Nothing_2006_wavSize;
            break;
        case 7:
            data = BinaryData::I_wav;
            dataSize = BinaryData::I_wavSize;
            break;
        case 8:
            data = BinaryData::Catch_33_wav;
            dataSize = BinaryData::Catch_33_wavSize;
            break;
        case 9:
            data = BinaryData::obZen_wav;
            dataSize = BinaryData::obZen_wavSize;
            break;
        case 10:
            data = BinaryData::Koloss_wav;
            dataSize = BinaryData::Koloss_wavSize;
            break;
        case 11:
            data = BinaryData::Pitch_Black_wav;
            dataSize = BinaryData::Pitch_Black_wavSize;
            break;
        case 12:
            data = BinaryData::The_Violent_Sleep_of_Reason_wav;
            dataSize = BinaryData::The_Violent_Sleep_of_Reason_wavSize;
            break;
        case 13:
            data = BinaryData::Immutable_Blend_wav;
            dataSize = BinaryData::Immutable_Blend_wavSize;
            break;
        default:
            DBG("Unknown selection (while loading)");
            return;
    }
    speakerModule.loadImpulseResponse(data,
                                      dataSize,
                                      juce::dsp::Convolution::Stereo::yes,
                                      juce::dsp::Convolution::Trim::yes, 0);

    DBG("Loaded IR: " + juce::String(impulseId));
}


juce::String ArtisianDSPAudioProcessor::getBinaryIrName(int impulseId)
{
    juce::MemoryBlock irMemoryBlock;
    const char* data = nullptr;
    size_t dataSize = 0;

    switch (impulseId)
    {
        case 1:
            data = BinaryData::ML_Sound_Labs_BEST_IR_IN_THE_WORLD_wav;
            dataSize = BinaryData::ML_Sound_Labs_BEST_IR_IN_THE_WORLD_wavSize;
            break;
        case 2:
            data = BinaryData::Contradictions_Collapse_wav;
            dataSize = BinaryData::Contradictions_Collapse_wavSize;
            break;
        case 3:
            data = BinaryData::Destroy_Erase_Improve_wav;
            dataSize = BinaryData::Destroy_Erase_Improve_wavSize;
            break;
        case 4:
            data = BinaryData::Chaosphere_wav;
            dataSize = BinaryData::Chaosphere_wavSize;
            break;
        case 5:
            data = BinaryData::Nothing_wav;
            dataSize = BinaryData::Nothing_wavSize;
            break;
        case 6:
            data = BinaryData::Nothing_2006_wav;
            dataSize = BinaryData::Nothing_2006_wavSize;
            break;
        case 7:
            data = BinaryData::I_wav;
            dataSize = BinaryData::I_wavSize;
            break;
        case 8:
            data = BinaryData::Catch_33_wav;
            dataSize = BinaryData::Catch_33_wavSize;
            break;
        case 9:
            data = BinaryData::obZen_wav;
            dataSize = BinaryData::obZen_wavSize;
            break;
        case 10:
            data = BinaryData::Koloss_wav;
            dataSize = BinaryData::Koloss_wavSize;
            break;
        case 11:
            data = BinaryData::Pitch_Black_wav;
            dataSize = BinaryData::Pitch_Black_wavSize;
            break;
        case 12:
            data = BinaryData::The_Violent_Sleep_of_Reason_wav;
            dataSize = BinaryData::The_Violent_Sleep_of_Reason_wavSize;
            break;
        case 13:
            data = BinaryData::Immutable_Blend_wav;
            dataSize = BinaryData::Immutable_Blend_wavSize;
            break;
        default:
            DBG("Unknown selection 2");
            return "null";
            
            
    }
    return data;
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
