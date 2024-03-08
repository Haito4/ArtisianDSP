//#pragma once
//
//
//
//class TutorialProcessor : public juce::AudioProcessor
//{
//public:
//    using AudioGraphIOProcessor = juce::AudioProcessorGraph::AudioGraphIOProcessor;
//    using Node = juce::AudioProcessorGraph::Node;
//
//
//    TutorialProcessor()
//        : AudioProcessor (BusesProperties().withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
//                                           .withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
//          mainProcessor  (new juce::AudioProcessorGraph())
//
//
//
//
//
//    bool isBusesLayoutSupported (const BusesLayout& layouts) const override
//    {
//        if (layouts.getMainInputChannelSet()  == juce::AudioChannelSet::disabled()
//         || layouts.getMainOutputChannelSet() == juce::AudioChannelSet::disabled())
//            return false;
//
//        if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
//         && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
//            return false;
//
//        return layouts.getMainInputChannelSet() == layouts.getMainOutputChannelSet();
//    }
//
//
//    void prepareToPlay (double sampleRate, int samplesPerBlock) override
//    {
//        mainProcessor->setPlayConfigDetails (getMainBusNumInputChannels(),
//                                             getMainBusNumOutputChannels(),
//                                             sampleRate, samplesPerBlock);
//
//        mainProcessor->prepareToPlay (sampleRate, samplesPerBlock);
//
//        initialiseGraph();
//    }
//
//
//
//
//
//private:
//    Node::Ptr audioInputNode;
//    Node::Ptr audioOutputNode;
//    Node::Ptr midiInputNode;
//    Node::Ptr midiOutputNode;
//
//
//};
