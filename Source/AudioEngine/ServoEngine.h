// Source/AudioEngine/ServoEngine.h
#pragma once

#include "../Source/AudioEngine/SoundEngineBase.h"    // Include the new base class
#include "../Parameters/Parameters.h" // For EngineParameterSet (needed by updateParameters)
#include <juce_dsp/juce_dsp.h>
#include <cmath>                // For std::sin, std::max, std::min

class ServoEngine : public SoundEngineBase // Inherit from SoundEngineBase
{
public:
    ServoEngine();
    ~ServoEngine() override = default;

    // --- SoundEngineBase overrides ---
    void prepare(const juce::dsp::ProcessSpec& spec) override;
    void reset() override;
    void processAddingTo(juce::dsp::ProcessContextReplacing<float>& context) override;
    void updateParameters(const EngineParameterSet& allParams) override;
    void setEnabled(bool enabled) override;
    bool getEnabled() const override;
    double getCPUUsage() const override;
    size_t getMemoryUsage() const override;

private:
    juce::dsp::Oscillator<float> servoOsc{ [](float x) { return std::sin(x); } }; // [cite: 18]
    juce::dsp::Oscillator<float> servoLFO{ [](float x) { return std::sin(x); } }; // [cite: 18]

    // Cached parameters
    float currentLevel = 0.0f;
    float currentPitch = 440.0f;
    float currentModDepth = 0.1f;
    float currentModRate = 1.0f;
};