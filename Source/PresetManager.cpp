#include "PresetManager.h"
#include "PluginProcessor.h"

namespace Service
{
    const juce::File PresetManager::defaultDirectory{ juce::File::getSpecialLocation(juce::File::SpecialLocationType::commonDocumentsDirectory)
                                                                                     .getChildFile(ProjectInfo::companyName)
                                                                                     .getChildFile(ProjectInfo::projectName)
        
                                                    };
    const juce::String PresetManager::extension{ "preset" };
    const juce::String PresetManager::presetNameProperty{ "presetName" };

    PresetManager::PresetManager(juce::AudioProcessorValueTreeState& apvts, ArtisianDSPAudioProcessor& p) : valueTreeState(apvts),
                                                                                                            audioProcessor(p)
    {
        // Create a default Preset Directory, if it doesn't exist
        if (!defaultDirectory.exists())
        {
            const auto result = defaultDirectory.createDirectory();
            if (result.failed())
            {
                DBG("Could not create preset directory: " + result.getErrorMessage());
                jassertfalse;
            }
        }

        valueTreeState.state.addListener(this);
        currentPreset.referTo(valueTreeState.state.getPropertyAsValue(presetNameProperty, nullptr));
    }

    void PresetManager::savePreset(const juce::String& presetName)
    {
        if (presetName.isEmpty())
            return;

        currentPreset.setValue(presetName);
        const auto xml = valueTreeState.copyState().createXml();
        xml->setAttribute("IRPath", audioProcessor.lastIrPath);
        xml->setAttribute("IRName", audioProcessor.lastIrName);
        const auto presetFile = defaultDirectory.getChildFile(presetName + "." + extension);
        if (!xml->writeTo(presetFile))
        {
            DBG("Could not create preset file: " + presetFile.getFullPathName());
            jassertfalse;
        }
    }

    void PresetManager::deletePreset(const juce::String& presetName)
    {
        if (presetName.isEmpty())
            return;

        const auto presetFile = defaultDirectory.getChildFile(presetName + "." + extension);
        if (!presetFile.existsAsFile())
        {
            DBG("Preset file " + presetFile.getFullPathName() + " does not exist");
            jassertfalse;
            return;
        }
        if (!presetFile.deleteFile())
        {
            DBG("Preset file " + presetFile.getFullPathName() + " could not be deleted");
            jassertfalse;
            return;
        }
        currentPreset.setValue("");
    }

    void PresetManager::loadPreset(const juce::String& presetName)
    {
        if (presetName.isEmpty())
            return;

        const auto presetFile = defaultDirectory.getChildFile(presetName + "." + extension);
        if (!presetFile.existsAsFile())
        {
            DBG("Preset file " + presetFile.getFullPathName() + " does not exist");
            jassertfalse;
            return;
        }
        // presetFile (XML) -> (ValueTree)
        juce::XmlDocument xmlDocument{ presetFile };
        const auto valueTreeToLoad = juce::ValueTree::fromXml(*xmlDocument.getDocumentElement());

        valueTreeState.replaceState(valueTreeToLoad);
        
        std::unique_ptr<juce::XmlElement> rootElement = xmlDocument.getDocumentElement();
        audioProcessor.lastIrPath = rootElement->getStringAttribute("IRPath").toStdString();
        audioProcessor.lastIrName = rootElement->getStringAttribute("IRName").toStdString();
        
        
        
        DBG(audioProcessor.lastIrPath);
        DBG(audioProcessor.lastIrName);
        
        // Check if the new IR file is valid before loading
        if (juce::File(audioProcessor.lastIrPath).existsAsFile())
        {
            audioProcessor.validIrLoaded = true;
            audioProcessor.shouldLoadIr = true;
            DBG("IR to be loaded: " + audioProcessor.lastIrPath);
            audioProcessor.variableTree2.setProperty("NEW_IRNAME", audioProcessor.lastIrName, nullptr); // Update to notify GUI label to change
        }
        else
        {
            audioProcessor.validIrLoaded = false;
            DBG("Invalid IR directory!");
            audioProcessor.shouldLoadIr = false;
            
            if (aCounter == false){
                DBG("false");
                audioProcessor.variableTree2.setProperty("NEW_IRNAME", "null_ir0", nullptr);
                aCounter = true;
            }
            else {
                audioProcessor.variableTree2.setProperty("NEW_IRNAME", "null_ir1", nullptr);
                aCounter = false;
                DBG("true");
            }
        }
        
        
        currentPreset.setValue(presetName);

    }

    int PresetManager::loadNextPreset()
    {
        const auto allPresets = getAllPresets();
        if (allPresets.isEmpty())
            return -1;
        const auto currentIndex = allPresets.indexOf(currentPreset.toString());
        const auto nextIndex = currentIndex + 1 > (allPresets.size() - 1) ? 0 : currentIndex + 1;
        loadPreset(allPresets.getReference(nextIndex));
        return nextIndex;
    }

    int PresetManager::loadPreviousPreset()
    {
        const auto allPresets = getAllPresets();
        if (allPresets.isEmpty())
            return -1;
        const auto currentIndex = allPresets.indexOf(currentPreset.toString());
        const auto previousIndex = currentIndex - 1 < 0 ? allPresets.size() - 1 : currentIndex - 1;
        loadPreset(allPresets.getReference(previousIndex));
        return previousIndex;
    }

    juce::StringArray PresetManager::getAllPresets() const
    {
        juce::StringArray presets;
        const auto fileArray = defaultDirectory.findChildFiles(
                                                               juce::File::TypesOfFileToFind::findFiles, false, "*." + extension);
        for (const auto& file : fileArray)
        {
            presets.add(file.getFileNameWithoutExtension());
        }
        return presets;
    }

    juce::String PresetManager::getCurrentPreset() const
    {
        return currentPreset.toString();
    }

    void PresetManager::valueTreeRedirected(juce::ValueTree& treeWhichHasBeenChanged)
    {
        currentPreset.referTo(treeWhichHasBeenChanged.getPropertyAsValue(presetNameProperty, nullptr));
        
//        DBG("Workey????");
    }
}
