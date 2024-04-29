#include "Utils.h"


const juce::String updatePedalState(bool usingState)
{
    juce::String toggleText = usingState ? "Off" : "On";
    juce::Logger::outputDebugString("Pedal state: " + toggleText);

    return toggleText;
}

