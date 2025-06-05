#pragma once

// --- Core JUCE Module Includes ---
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h> // Still needed for ProcessSpec etc.
#include <juce_gui_basics/juce_gui_basics.h>

// --- Standard Library Includes ---
#include <memory>
#include <vector>
#include <atomic>
#include <cmath>
// <random> is now primarily in NoiseGenerator.h

// --- Project Includes ---
#include "../Source/Parameters/Parameters.h"     // For parameter definitions and layout function
#include "../Source/AudioEngine/MechaSoundEngine.h" // For the main sound engine

// --- Forward Declaration ---
class MechaSoundGeneratorAudioProcessorEditor; // Keep this if your editor is named this

//==============================================================================
class MechaSoundGeneratorAudioProcessor : public juce::AudioProcessor
{
public:
    //==============================================================================
    MechaSoundGeneratorAudioProcessor();
    ~MechaSoundGeneratorAudioProcessor() noexcept override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    // Public member for APVTS, accessible by the editor
    juce::AudioProcessorValueTreeState apvts;

private:
    //==============================================================================
    // createParameterLayout is now a free function declared in Parameters.h

    // Cached atomic parameter pointers for thread-safe access from audio thread
    std::atomic<float>* hissLevelParam = nullptr;
    std::atomic<float>* hissCutoffParam = nullptr;
    std::atomic<float>* hissResonanceParam = nullptr;
    std::atomic<float>* servoLevelParam = nullptr;
    std::atomic<float>* servoPitchParam = nullptr;
    std::atomic<float>* servoModDepthParam = nullptr;
    std::atomic<float>* servoModRateParam = nullptr;
    std::atomic<float>* powerCoreHumLevelParam = nullptr;
    std::atomic<float>* powerCoreFundamentalPitchParam = nullptr;
    std::atomic<float>* powerCoreHumComplexityParam = nullptr;
    std::atomic<float>* powerCorePulsationRateParam = nullptr;
    std::atomic<float>* powerCorePulsationDepthParam = nullptr;
    std::atomic<float>* powerCoreActivationTriggerParam;
    std::atomic<float>* powerCoreActivationTimeParam = nullptr;
    std::atomic<float>* powerCoreEnergyTypeParam = nullptr;
    std::atomic<float>* powerCoreFilterCutoffParam = nullptr;
    std::atomic<float>* powerCoreFilterResonanceParam = nullptr;
    std::atomic<float>* masterGainParam = nullptr;

    // DSP Engine
    MechaSoundEngine mechaSoundEngine;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MechaSoundGeneratorAudioProcessor)
};