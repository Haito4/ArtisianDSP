#pragma once

#include <JuceHeader.h>

class HelpComponent : public juce::Component
{
public:
    HelpComponent()
    {
        addAndMakeVisible(helpMenuHeading);
        helpMenuHeading.setText("Help Menu", juce::dontSendNotification);
        helpMenuHeading.setJustificationType(juce::Justification::centred);
        helpMenuHeading.setFont(juce::Font(20.0f, juce::Font::bold));
        
        
        addAndMakeVisible(howToUseHeading);
        howToUseHeading.setText("How to use this plugin", juce::dontSendNotification);
        howToUseHeading.setJustificationType(juce::Justification::left);
        howToUseHeading.setFont(juce::Font(15.0f, juce::Font::bold));
        
        
        addAndMakeVisible(howToUseBody);
        howToUseBody.setText(
            std::string("Input/Output Sliders: \n") +
            "You can alter the input and output sliders to change the loudness of the signal entering and leaving the plugin. \n\n"
                             
            "Noise Gate:\n"
            "The noise gate is used to eliminate unwanted noise and hum. "
            "It helps maintain a tight sound by cutting off the signal below the value specified by the threshold knob. The attack and release knobs alter the speed at which the gate opens and closes. \n"
            "Using the gate is beneficial for playing syncopated phrases that also need to utilise distortions. \n\n"

            "Compressor:\n"
            "The compressor helps to even out the dynamics of your guitar by reducing the volume of loud sounds and increasing the volume of softer sounds. "
            "This results in a more consistent tone. It's particularly useful for tapping, ensuring that every note is clearly balance levelwise.\n\n"
            
            "Overdrive:\n"
            "The overdrive pedal serves multiple purposes. "
            "It can be used to tighten up high-gain tones by cutting out the low frequencies with the tone knob, or used as the main source of distortion with drive knob. "
            "The latter case serves as an excellent way to add clarity modern metal tones. \n\n"
            
            "Amplifier:\n"
            "The amp sim provides the saturated distortion needed for metal genres, whilst also being able to be used lightly. "
            "Its various knobs allow for a wide range of tone-shaping options. \n"
            "Use in conjunction with a low-drive, high-tone overdrive setting for a distorted, but clean tone. \n\n"
            
            "Reverb:\n"
            "Reverb adds a sense of space and depth to the sound by simulating the reflections of sound within a room, its size adjustable with the size knob. "
            "When used properly, it creates a more natural sound, making the tone less dry. Reverb is versatile, spacing out clean tones, adding depth to leads, and thickening rhythms. \n\n"
                             
            "Impulse Response Loader:\n"
            "The impulse response (IR) loader is used to simulate the response of different speaker cabinets and recording environments. "
            "The most drastic transformation to tone is achieved by loading different IRs, which can emulate anything from a vintage 1x12\" cab to a modern 4x12\" stack, as well as different mic placements and recording rooms. "
            "This plugin contains 13 IRs that you can select with the dropdown box. If you have your own IRs in the form of .wav files, you can load them by clicking the \"Load IR\" button and selecting them.  \n\n"
            
            "Preset System:\n"
            "This plugin contains a preset system that allows you to save the state of all of your adjusted parameters and store them within a unique file. When clicking the save button, a file prompt will open. Choose a name for the preset and click save. It will now be available to load via the dropdown menu. \n"
            "Presets are also navigatable via the arrow buttons adjacent to the dropdown box. Should you no longer wish to have a certain preset, you can delete it with the delete button. \n"
            "If a preset contains a path to an IR file that doesn't exist, a \"fix\" prompt will appear, allowing you to select the path of the file. The new path will be added to file automatically, and loaded. \n"
            "This plugin comes with some example presets that you can find prefixed with \"EX\". These allow you to jump straight into a fully-designed preset and get straight to the point.",
            juce::dontSendNotification);
        howToUseBody.setJustificationType(juce::Justification::topLeft);
        howToUseBody.setColour(juce::Label::textColourId, juce::Colours::white);
        howToUseBody.setColour(juce::Label::backgroundColourId, juce::Colour(0x2c3027));
        

        addAndMakeVisible(howToUseBodyViewport);
        howToUseBodyViewport.setViewedComponent(&howToUseBody, false);
        howToUseBodyViewport.setScrollBarsShown(true, false, true, false);
        
        
        addAndMakeVisible(privacyHeading);
        privacyHeading.setText("Privacy Disclosure", juce::dontSendNotification);
        privacyHeading.setFont(juce::Font(20.0f));
        privacyHeading.setJustificationType(juce::Justification::left);
        
        
        addAndMakeVisible(privacyBody);
        privacyBody.setText("This program doesn't take your personal data.", juce::dontSendNotification);
        privacyBody.setColour(juce::TextEditor::backgroundColourId, juce::Colours::transparentBlack);
        privacyBody.setColour(juce::TextEditor::outlineColourId, juce::Colours::transparentBlack);
        
        
        addAndMakeVisible(linksLabel);
        linksLabel.setText("External Links", juce::dontSendNotification);
        linksLabel.setJustificationType(juce::Justification::right);
        linksLabel.setFont(juce::Font(20.0f));
        
        
        addAndMakeVisible(onlineHelpButton);
        onlineHelpButton.setButtonText("Go to Online Help");
        onlineHelpButton.onClick = [this](){
            juce::URL url("https://haito4.github.io");
            url.launchInDefaultBrowser();
        };
        
        
        addAndMakeVisible(viewCodeButton);
        viewCodeButton.setButtonText("View Source Code");
        viewCodeButton.onClick = [this](){
            juce::URL url("https://github.com/Haito4/ArtisianDSP/tree/6d8ce1b1b9851edd188b81b02a9eb14add4b9f76/Source");
            url.launchInDefaultBrowser();
        };
        
        howToUseBodyViewport.toFront(false);
        
    }

    void paint(juce::Graphics& g) override
    {
        g.setColour(juce::Colours::black.withAlpha(0.6f));
        g.fillRect(0, 0, 720, 510);
        
        g.setColour(juce::Colours::white);
        g.drawRect(helpOverlayBounds, 2);
    }
    
    void resized() override
    {
        if (isVisible())
        {
            auto overlayWidth = getWidth() / 1.1;
            auto overlayHeight = getHeight() / 1.1;
            helpOverlayBounds = juce::Rectangle<int>((getWidth() - overlayWidth) / 2, (getHeight() - overlayHeight) / 2, overlayWidth, overlayHeight);
        }
        
        
        helpMenuHeading.setBounds(260, 27, 200, 50);
        
        howToUseHeading.setBounds(100, 50, 200, 50);
        howToUseBody.setBounds(60, 90, 595, 800); // Slightly narrower than the viewport because of scrollbar
        howToUseBodyViewport.setBounds(60, 90, 600, 300);
        
        privacyHeading.setBounds(36, 408, 230, 25);
        privacyBody.setBounds(37, 430, 320, 50);
        
        linksLabel.setBounds(437, 408, 247, 25);
        viewCodeButton.setBounds(440, 433, 120, 50);
        onlineHelpButton.setBounds(563, 433, 120, 50);
    }
    
private:
    juce::Rectangle<int> helpOverlayBounds;
    
    juce::Label helpMenuHeading;
    juce::Label privacyHeading;
    juce::Label privacyBody;
    
    
    juce::Label howToUseHeading;
    juce::Label howToUseBody;
    juce::Viewport howToUseBodyViewport;
    
    
    juce::Label linksLabel;
    juce::TextButton onlineHelpButton;
    juce::TextButton viewCodeButton;
    
    
    
};
