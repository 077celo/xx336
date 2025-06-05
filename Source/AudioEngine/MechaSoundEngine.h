// Source/AudioEngine/MechaSoundEngine.h
#pragma once

#include <juce_dsp/juce_dsp.h>
#include <vector>
#include <memory>
#include "../Source/AudioEngine/NoiseGenerator.h"     // Uses SimpleNoiseGenerator
#include "../Source/AudioEngine/SoundEngineBase.h"    // For SoundEngineBase interface
#include "../Parameters/Parameters.h" // For EngineParameterSet

// Forward declare concrete engines that will be managed
class ServoEngine;
class PowerCoreEngine;


// The MechaSound::ParameterValues struct previously here can be removed
// if EngineParameterSet is used consistently from PluginProcessor.

class MechaSoundEngine
{
public:
    MechaSoundEngine();
    ~MechaSoundEngine();

    void prepare(const juce::dsp::ProcessSpec& spec);
    void reset();

    // Main processing method, now takes EngineParameterSet
    void process(juce::AudioBuffer<float>& buffer, const EngineParameterSet& allParams);

private:
    // Hiss components (kept in MechaSoundEngine for now)
    SimpleNoiseGenerator noiseGen;
    juce::dsp::StateVariableTPTFilter<float> hissFilter; // TPT Filter for hiss [cite: 18]

    // Collection of sound engines
    std::vector<std::unique_ptr<SoundEngineBase>> soundEngines;
};