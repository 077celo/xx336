#pragma once

#include "../PluginProcessor.h"

// Forward declaration
class UIGraph;

class MechaSoundGeneratorAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    MechaSoundGeneratorAudioProcessorEditor(MechaSoundGeneratorAudioProcessor&);
    ~MechaSoundGeneratorAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    MechaSoundGeneratorAudioProcessor& processorRef;

    const juce::Colour bgColour = juce::Colour(0xff262833);
    const juce::Colour sectionBgColour = juce::Colour(0xff343746);
    const juce::Colour accentColour1 = juce::Colour(0xff78C0A8);
    const juce::Colour textColour = juce::Colours::lightgrey;
    const juce::Colour outlineColour = juce::Colours::black.withAlpha(0.5f);

    std::unique_ptr<UIGraph> uiGraphArea;

    // UI Components
    juce::Label titleLabel;

    juce::Slider hissLevelSlider;
    juce::Label hissLevelLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> hissLevelAttachment;

    juce::Slider hissCutoffSlider;
    juce::Label hissCutoffLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> hissCutoffAttachment;

    juce::Slider hissResonanceSlider;
    juce::Label hissResonanceLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> hissResonanceAttachment;

    juce::Slider servoLevelSlider;
    juce::Label servoLevelLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> servoLevelAttachment;

    juce::Slider servoPitchSlider;
    juce::Label servoPitchLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> servoPitchAttachment;

    juce::Slider servoModDepthSlider;
    juce::Label servoModDepthLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> servoModDepthAttachment;

    juce::Slider servoModRateSlider;
    juce::Label servoModRateLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> servoModRateAttachment;

    // --- PowerCore UI Components ---
    juce::Slider powerCoreHumLevelSlider;
    juce::Label powerCoreHumLevelLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> powerCoreHumLevelAttachment;

    juce::Slider powerCorePitchSlider;
    juce::Label powerCorePitchLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> powerCorePitchAttachment;

    juce::Slider powerCoreComplexitySlider;
    juce::Label powerCoreComplexityLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> powerCoreComplexityAttachment;

    juce::Slider powerCorePulsationRateSlider;
    juce::Label powerCorePulsationRateLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> powerCorePulsationRateAttachment;

    juce::Slider powerCorePulsationDepthSlider;
    juce::Label powerCorePulsationDepthLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> powerCorePulsationDepthAttachment;

    juce::ToggleButton powerCoreActivationButton; // For the boolean trigger
    juce::Label powerCoreActivationLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> powerCoreActivationAttachment;

    juce::Slider powerCoreActivationTimeSlider;
    juce::Label powerCoreActivationTimeLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> powerCoreActivationTimeAttachment;

    juce::Slider powerCoreEnergyTypeSlider;
    juce::Label powerCoreEnergyTypeLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> powerCoreEnergyTypeAttachment;

    juce::Slider powerCoreFilterCutoffSlider;
    juce::Label powerCoreFilterCutoffLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> powerCoreFilterCutoffAttachment;

    juce::Slider powerCoreFilterResonanceSlider;
    juce::Label powerCoreFilterResonanceLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> powerCoreFilterResonanceAttachment;


    juce::Slider masterGainSlider;
    juce::Label masterGainLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> masterGainAttachment;

    void initializeUI();
    void setupSlidersWithReducedSize();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MechaSoundGeneratorAudioProcessorEditor)
};
