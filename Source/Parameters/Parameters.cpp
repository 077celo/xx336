#include "../Source/Parameters/Parameters.h"

juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    // --- Hydraulic Hiss Parameters ---
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        ParameterIDs::hissLevel, "Hiss Level", ParameterRanges::gainRange(), 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        ParameterIDs::hissCutoff, "Hiss Cutoff", ParameterRanges::frequencyRange(100.0f, 18000.0f), 5000.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        ParameterIDs::hissResonance, "Hiss Resonance", ParameterRanges::qRange(), 1.0f));

    // --- Servo Whine Parameters ---
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        ParameterIDs::servoLevel, "Servo Level", ParameterRanges::gainRange(), 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        ParameterIDs::servoPitch, "Servo Pitch", ParameterRanges::frequencyRange(50.0f, 5000.0f), 440.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        ParameterIDs::servoModDepth, "Servo Mod Depth", ParameterRanges::percentRange(), 0.1f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        ParameterIDs::servoModRate, "Servo Mod Rate", ParameterRanges::rateRange(0.1f, 30.0f), 1.0f));

    // --- PowerCore Engine Parameters ---
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        ParameterIDs::powerCoreHumLevel, "Power Core Hum Level", ParameterRanges::gainRange(), 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        ParameterIDs::powerCoreFundamentalPitch, "Power Core Pitch", ParameterRanges::frequencyRange(20.0f, 200.0f), 60.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        ParameterIDs::powerCoreHumComplexity, "Power Core Complexity", ParameterRanges::percentRange(), 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        ParameterIDs::powerCorePulsationRate, "Power Core Pulsation Rate", ParameterRanges::rateRange(0.1f, 5.0f), 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        ParameterIDs::powerCorePulsationDepth, "Power Core Pulsation Depth", ParameterRanges::percentRange(), 0.3f));
    params.push_back(std::make_unique<juce::AudioParameterBool>(
        ParameterIDs::powerCoreActivationTrigger, "Power Core Activation", false));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        ParameterIDs::powerCoreActivationTime, "Power Core Activation Time", ParameterRanges::timeRange(0.5f, 10.0f), 2.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        ParameterIDs::powerCoreEnergyType, "Power Core Energy Type", ParameterRanges::percentRange(), 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        ParameterIDs::powerCoreFilterCutoff, "Power Core Filter Cutoff", ParameterRanges::frequencyRange(100.0f, 10000.0f), 5000.0f));
    // Corrected call to qRange for PowerCoreFilterResonance - using specific range for this parameter
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        ParameterIDs::powerCoreFilterResonance, "Power Core Filter Reso", ParameterRanges::qRange(0.1f, 10.0f, 0.01f, 0.7f), 1.0f));


    // --- Master Parameters ---
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        ParameterIDs::masterGain, "Master Gain", ParameterRanges::gainRange(), 0.707f));

    return { params.begin(), params.end() };
}
