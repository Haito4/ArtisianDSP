#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Gate1Component.h"
#include "Comp2Component.h"
#include "Drive3Component.h"
#include "Amp4Component.h"
#include "Reverb5Component.h"
#include "Impulse6Component.h"
#include <memory>

class MultiSceneComponent : public juce::Component
{
public:
    MultiSceneComponent(ArtisianDSPAudioProcessor& processor);
    ~MultiSceneComponent();

    void resized() ;

private:
    ArtisianDSPAudioProcessor& audioProcessor;
    
    std::unique_ptr<Gate1Component> scene1;
    std::unique_ptr<Comp2Component> scene2;
    std::unique_ptr<Drive3Component> scene3;
    std::unique_ptr<Amp4Component> scene4;
    std::unique_ptr<Reverb5Component> scene5;
    std::unique_ptr<Impulse6Component> scene6;
    

    juce::TextButton sceneButton1;
    juce::TextButton sceneButton2;
    juce::TextButton sceneButton3;
    juce::TextButton sceneButton4;
    juce::TextButton sceneButton5;
    juce::TextButton sceneButton6;

    void showScene(int sceneIndex);
    
    
};
