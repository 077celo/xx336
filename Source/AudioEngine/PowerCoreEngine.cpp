#include "PowerCoreEngine.h"
#include <cmath> // For std::sin, std::tanh

PowerCoreEngine::PowerCoreEngine()
{
    // Initialize oscillators with a sine wave function
    fundamentalOsc.initialise([](float x) { return std::sin(x); });
    pulsationLFO.initialise([](float x) { return std::sin(x); }); // Simple sine LFO for pulsation

    // Initialize harmonic oscillators
    for (int i = 0; i < MAX_HARMONICS; ++i)
    {
        harmonicOscillators.emplace_back();
        harmonicOscillators.back().initialise([](float x) { return std::sin(x); });
    }

    isEnabledFlag = true;
}

void PowerCoreEngine::prepare(const juce::dsp::ProcessSpec& spec)
{
    currentSampleRate = spec.sampleRate;
    currentBlockSize = static_cast<int>(spec.maximumBlockSize);

    fundamentalOsc.prepare(spec);
    pulsationLFO.prepare(spec);
    for (auto& osc : harmonicOscillators)
    {
        osc.prepare(spec);
    }

    toneFilter.prepare(spec);
    humGain.prepare(spec);
    humGain.setGainLinear(0.0f);

    // Ensure activationTime is positive before using it for smoothing reset
    float initialActivationTime = (currentActivationTime > 0.001f) ? currentActivationTime : 2.0f; // Default if invalid
    smoothedHumLevel.reset(spec.sampleRate, initialActivationTime);
    smoothedFilterCutoff.reset(spec.sampleRate, 0.05);

    reset();
}

void PowerCoreEngine::reset()
{
    fundamentalOsc.reset();
    pulsationLFO.reset();
    for (auto& osc : harmonicOscillators)
    {
        osc.reset();
    }
    toneFilter.reset();
    humGain.reset();

    smoothedHumLevel.setCurrentAndTargetValue(0.0f);
    // Reset filter smoothed value too
    smoothedFilterCutoff.setCurrentAndTargetValue(currentFilterCutoff); // Assuming currentFilterCutoff has a sane default

    activationEnvelope = 0.0f;
    isActivating = false;
    isDeactivating = false;
}

void PowerCoreEngine::updateParameters(const EngineParameterSet& allParams)
{
    const auto& params = allParams.powerCore; // params is now a reference to allParams.powerCore
    currentHumLevel = params.humLevel;
    currentFundamentalPitch = params.fundamentalPitch;
    currentHumComplexity = params.humComplexity;
    currentPulsationRate = params.pulsationRate;
    currentPulsationDepth = params.pulsationDepth;

    float newActivationTime = (params.activationTime > 0.001f) ? params.activationTime : 0.001f; // Ensure positive

    if (params.activationTrigger && !currentActivationTrigger) {
        isActivating = true;
        isDeactivating = false;
        // Calculate increment based on sample rate and new activation time
        activationIncrement = (currentSampleRate > 0) ? (1.0f / (newActivationTime * static_cast<float>(currentSampleRate))) : 1.0f;
    }
    else if (!params.activationTrigger && currentActivationTrigger) {
        isDeactivating = true;
        isActivating = false;
        activationIncrement = (currentSampleRate > 0) ? (1.0f / (newActivationTime * static_cast<float>(currentSampleRate))) : 1.0f;
    }
    currentActivationTrigger = params.activationTrigger;
    currentActivationTime = newActivationTime;

    currentEnergyType = params.energyType;
    // These lines should now be correct because filterCutoff and filterResonance are members of PowerCoreParams
    currentFilterCutoff = params.filterCutoff;
    currentFilterResonance = params.filterResonance;

    smoothedHumLevel.setTargetValue(currentHumLevel);
    // Check if currentActivationTime has changed and smoothing duration needs update
    if (currentActivationTime > 0.001f) {
        // Always reset with new activation time - SmoothedValue will handle smoothing appropriately
        smoothedHumLevel.reset(currentSampleRate, currentActivationTime);
    }


    smoothedFilterCutoff.setTargetValue(currentFilterCutoff);

    pulsationLFO.setFrequency(currentPulsationRate);

    int numActiveHarmonics = static_cast<int>(currentHumComplexity * MAX_HARMONICS);

    fundamentalOsc.setFrequency(currentFundamentalPitch, true);
    for (int i = 0; i < MAX_HARMONICS; ++i)
    {
        if (i < numActiveHarmonics) {
            harmonicOscillators[i].setFrequency(currentFundamentalPitch * (2.0f * (i + 1) + 1.0f), true);
        }
    }

    toneFilter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
    toneFilter.setResonance(currentFilterResonance);
}

void PowerCoreEngine::processAddingTo(juce::dsp::ProcessContextReplacing<float>& context)
{
    if (!isEnabledFlag) return;

    auto& outputBlock = context.getOutputBlock();
    auto numSamples = outputBlock.getNumSamples();
    auto numChannels = outputBlock.getNumChannels();

    for (size_t sample = 0; sample < numSamples; ++sample)
    {
        if (isActivating) {
            activationEnvelope += activationIncrement;
            if (activationEnvelope >= 1.0f) {
                activationEnvelope = 1.0f;
                isActivating = false;
            }
        }
        else if (isDeactivating) {
            activationEnvelope -= activationIncrement;
            if (activationEnvelope <= 0.0f) {
                activationEnvelope = 0.0f;
                isDeactivating = false;
            }
        }

        float overallLevel = smoothedHumLevel.getNextValue() * activationEnvelope;
        if (overallLevel < 0.0001f) { // Reduced threshold slightly
            for (size_t channel = 0; channel < numChannels; ++channel) {
                outputBlock.getChannelPointer(channel)[sample] += 0.0f;
            }
            // Ensure LFO and oscillators are processed to keep their phase correct even if output is zero.
            pulsationLFO.processSample(0.0f);
            fundamentalOsc.processSample(0.0f);
            int numActiveHarmonics = static_cast<int>(currentHumComplexity * MAX_HARMONICS);
            for (int i = 0; i < numActiveHarmonics; ++i) {
                harmonicOscillators[i].processSample(0.0f);
            }
            smoothedFilterCutoff.getNextValue(); // Keep smoothed values ticking over

            continue;
        }

        float coreSound = fundamentalOsc.processSample(0.0f);

        int numActiveHarmonics = static_cast<int>(currentHumComplexity * MAX_HARMONICS);
        float harmonicContent = 0.0f;
        for (int i = 0; i < numActiveHarmonics; ++i)
        {
            harmonicContent += harmonicOscillators[i].processSample(0.0f) / (static_cast<float>(i) + 2.0f);
        }
        coreSound += harmonicContent * currentHumComplexity;
        // Apply energyType effect - e.g. tanh for more aggressive, or smoother clipping
        if (currentEnergyType > 0.5f) {
            coreSound = std::tanh(coreSound * (1.0f + currentEnergyType)); // More aggressive with higher energy type
        }
        else {
            coreSound = coreSound * (0.5f + currentEnergyType); // Softer for lower energy type
        }


        float lfoSample = (1.0f - currentPulsationDepth) + currentPulsationDepth * ((pulsationLFO.processSample(0.0f) * 0.5f) + 0.5f);
        coreSound *= lfoSample;

        toneFilter.setCutoffFrequency(smoothedFilterCutoff.getNextValue());

        // Process filter per channel if it's stateful and meant to be stereo
        // For now, assume mono processing for the filter before splitting to channels
        float filteredSound = coreSound; // If mono processing
        // If stereo filter processing desired, it would be:
        // float leftSample = toneFilter.processSample(0, coreSound);
        // float rightSample = toneFilter.processSample(1, coreSound); // Requires filter to be stereo aware or two mono filters

        for (size_t channel = 0; channel < numChannels; ++channel)
        {
            // If filter is processed mono, apply same filtered sound. 
            // If stereo, apply respective channel's sample.
            outputBlock.getChannelPointer(channel)[sample] += toneFilter.processSample(static_cast<int>(channel), coreSound); // processSample now takes channel index
        }
    }
}

void PowerCoreEngine::setEnabled(bool enabled)
{
    isEnabledFlag = enabled;
    if (!enabled) {
        // When disabling, ensure a smooth fade or quick ramp down
        // Trigger deactivation sequence if not already deactivating
        if (activationEnvelope > 0.0f && !isDeactivating) {
            isDeactivating = true;
            isActivating = false; // Ensure it's not trying to activate
            // Use a faster deactivation time if desired when explicitly disabled
            float disableTime = 0.1f; // e.g., 100ms to fade out
            if (currentSampleRate > 0 && disableTime > 0.001f) {
                activationIncrement = (1.0f / (disableTime * static_cast<float>(currentSampleRate)));
            }
            else {
                activationIncrement = 1.0f; // Fallback to immediate if sample rate is zero
            }
        }
        // It might be better to just let the current activation/deactivation play out
        // or force activationEnvelope to 0 after a short ramp.
        // Forcing target for smoothedHumLevel to 0.
        smoothedHumLevel.setTargetValue(0.0f);

    }
    else {
        // If enabling and it was fully off, maybe trigger activation if trigger is set
        // This depends on desired logic. For now, enabling just allows processing.
        // The activationTrigger parameter should handle starting the sound.
    }
}

bool PowerCoreEngine::getEnabled() const
{
    return isEnabledFlag;
}

double PowerCoreEngine::getCPUUsage() const
{
    return 0.0;
}

size_t PowerCoreEngine::getMemoryUsage() const
{
    return sizeof(*this) +
        sizeof(juce::dsp::Oscillator<float>) * (1 + 1 + MAX_HARMONICS) +
        sizeof(juce::dsp::StateVariableTPTFilter<float>) +
        sizeof(juce::dsp::Gain<float>);
}

