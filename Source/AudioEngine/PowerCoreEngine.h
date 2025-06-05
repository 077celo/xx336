#pragma once

#include "../Source/AudioEngine/SoundEngineBase.h"
#include "../Source/Parameters/Parameters.h" // For EngineParameterSet
#include <juce_dsp/juce_dsp.h>
#include <vector> // For std::vector of oscillators

class PowerCoreEngine : public SoundEngineBase
{
public:
    PowerCoreEngine();
    ~PowerCoreEngine() override = default;

    // --- SoundEngineBase overrides ---
    void prepare(const juce::dsp::ProcessSpec& spec) override;
    void reset() override;
    void processAddingTo(juce::dsp::ProcessContextReplacing<float>& context) override;
    void updateParameters(const EngineParameterSet& allParams) override;
    void setEnabled(bool enabled) override;
    bool getEnabled() const override;
    double getCPUUsage() const override; // Placeholder
    size_t getMemoryUsage() const override; // Placeholder

private:
    // DSP Components for PowerCoreEngine [cite: 10]
    std::vector<juce::dsp::Oscillator<float>> harmonicOscillators;
    juce::dsp::Oscillator<float> fundamentalOsc;
    juce::dsp::Oscillator<float> pulsationLFO; // For amplitude modulation [cite: 10]
    juce::dsp::StateVariableTPTFilter<float> toneFilter; // For tonal shaping and filter sweeps [cite: 10]
    juce::dsp::Gain<float> humGain;

    // Smoothed parameters for envelopes/transitions
    juce::LinearSmoothedValue<float> smoothedHumLevel;
    juce::LinearSmoothedValue<float> smoothedFilterCutoff;

    // Cached parameters from PowerCoreParams
    float currentHumLevel = 0.0f;
    float currentFundamentalPitch = 60.0f;
    float currentHumComplexity = 0.5f; // Controls number/amplitude of harmonics
    float currentPulsationRate = 1.0f;
    float currentPulsationDepth = 0.3f;
    bool currentActivationTrigger = false; // For activation states [cite: 10]
    float currentActivationTime = 2.0f; // For power transition duration [cite: 10]
    float currentEnergyType = 0.5f; // Character (smooth to aggressive), can map to waveform or filter [cite: 10]
    float currentFilterCutoff = 5000.0f; // For filter sweep capabilities
    float currentFilterResonance = 1.0f;


    // Parameters for activation envelope (conceptual)
    bool isActivating = false;
    bool isDeactivating = false;
    float activationEnvelope = 0.0f; // Current state of activation (0 to 1)
    float activationIncrement = 0.0f; // Calculated based on activationTime

    static constexpr int MAX_HARMONICS = 5; // Example: up to 5 harmonics
};