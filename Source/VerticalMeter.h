#pragma once

#include <JuceHeader.h>

namespace gui
{
    class VerticalMeter : public juce::Component
    {
    public:
        void paint(juce::Graphics& g) override
        {
            auto bounds = getLocalBounds().toFloat();
            
            g.setColour(juce::Colours::white.withBrightness(0.4f));
            g.fillRoundedRectangle(bounds, 5.f);
            
            g.setColour(juce::Colours::white);
            // map level from -60.f / +6.f to 0 / height
            const auto scaledY = juce::jmap(level, -60.f, 6.f, 0.f, static_cast<float>(getHeight()));
            g.fillRoundedRectangle(bounds.removeFromBottom(scaledY), 5.f);
        }
        
        void setLevel(const float value) { level = value; }
    private:
        float level = -60.f;
    };
}

