// Source/Parameters/Parameters.h
#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_core/juce_core.h> // For juce::String
#include <vector>               // For std::vector in createParameterLayout
#include <memory>               // For std::unique_ptr in createParameterLayout

// Parameter IDs (existing)
namespace ParameterIDs
{
    const juce::String hissLevel("hissLevel");
    const juce::String hissCutoff("hissCutoff");
    const juce::String hissResonance("hissResonance");
    const juce::String servoLevel("servoLevel");
    const juce::String servoPitch("servoPitch");
    const juce::String servoModDepth("servoModDepth");
    const juce::String servoModRate("servoModRate");
    const juce::String masterGain("masterGain");

    // PowerCore Parameter IDs
    const juce::String powerCoreHumLevel("powerCoreHumLevel");
    const juce::String powerCoreFundamentalPitch("powerCoreFundamentalPitch");
    const juce::String powerCoreHumComplexity("powerCoreHumComplexity");
    const juce::String powerCorePulsationRate("powerCorePulsationRate");
    const juce::String powerCorePulsationDepth("powerCorePulsationDepth");
    const juce::String powerCoreActivationTrigger("powerCoreActivationTrigger");
    const juce::String powerCoreActivationTime("powerCoreActivationTime");
    const juce::String powerCoreEnergyType("powerCoreEnergyType");
    const juce::String powerCoreFilterCutoff("powerCoreFilterCutoff");
    const juce::String powerCoreFilterResonance("powerCoreFilterResonance");
    // Add new ParameterIDs here for future engines if they are controlled by APVTS
}

// Parameter Ranges
namespace ParameterRanges
{
    inline juce::NormalisableRange<float> gainRange() { return { 0.0f, 1.0f, 0.001f, 0.3f }; }
    inline juce::NormalisableRange<float> percentRange() { return { 0.0f, 1.0f, 0.01f }; }
    inline juce::NormalisableRange<float> frequencyRange(float min = 20.0f, float max = 20000.0f, float interval = 1.0f, float skew = 0.25f) { return { min, max, interval, skew }; }
    // Modified qRange to accept min, max, interval, and skew, with defaults matching original.
    inline juce::NormalisableRange<float> qRange(float min = 0.1f, float max = 18.0f, float interval = 0.01f, float skew = 0.25f) { return { min, max, interval, skew }; }
    inline juce::NormalisableRange<float> rateRange(float min = 0.01f, float max = 20.0f, float interval = 0.01f, float skew = 0.3f) { return { min, max, interval, skew }; }
    inline juce::NormalisableRange<float> timeRange(float min = 0.001f, float max = 10.0f, float interval = 0.001f, float skew = 0.4f) { return { min, max, interval, skew }; }
}

// Parameter structs for individual sound sources/engines
struct HissParams
{
    float level = 0.0f;
    float cutoff = 5000.0f;
    float resonanceQ = 1.0f;
};

struct ServoParams
{
    float level = 0.0f;
    float pitch = 440.0f;
    float modDepth = 0.1f;
    float modRate = 1.0f;
};

struct ThrusterParams
{
    float intensity = 0.0f;
    float tone = 0.0f;
    float ignitionTime = 0.5f;
    float rumbleAmount = 0.5f;
    float instability = 0.0f;
    float filterCutoff = 1000.0f;
    float filterResonance = 0.5f;
    float lfoRate = 1.0f;
    float lfoDepth = 0.0f;
};

struct PowerCoreParams
{
    float humLevel = 0.0f;
    float fundamentalPitch = 60.0f;
    float humComplexity = 0.5f;
    float pulsationRate = 1.0f;
    float pulsationDepth = 0.3f;
    bool activationTrigger = false;
    float activationTime = 2.0f;
    float energyType = 0.5f;
    // Added missing filter parameters for PowerCore
    float filterCutoff = 5000.0f;    // Default value, can be adjusted
    float filterResonance = 1.0f;   // Default value, can be adjusted
};

struct MechanicalJointParams
{
    int movementType = 0;
    float impactForce = 0.0f;
    float materialResonance = 850.0f;
    float grindIntensity = 0.0f;
    float movementSpeed = 1.0f;
    float attackTime = 0.01f;
    float decayTime = 0.5f;
    float metalHardness = 0.5f;
    float jointLooseness = 0.2f;
    float stressLevel = 0.0f;
    float surfaceTexture = 0.3f;
};


// Main struct to hold all engine parameters
struct EngineParameterSet
{
    HissParams hiss;
    ServoParams servo;
    ThrusterParams thruster;
    PowerCoreParams powerCore;
    MechanicalJointParams joint;
};


// Declaration for the layout creation function
juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
