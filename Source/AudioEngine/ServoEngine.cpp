// Source/AudioEngine/ServoEngine.cpp
#include "../Source/AudioEngine/ServoEngine.h"

ServoEngine::ServoEngine()
{
    // Constructor: isEnabledFlag is initialized in SoundEngineBase
    isEnabledFlag = true; // Default to enabled, or set via updateParameters/setEnabled
}

void ServoEngine::prepare(const juce::dsp::ProcessSpec& spec)
{
    currentSampleRate = spec.sampleRate;
    currentBlockSize = static_cast<int>(spec.maximumBlockSize);

    servoOsc.prepare(spec);
    servoLFO.prepare(spec);
    reset(); // Ensure a clean state
}

void ServoEngine::reset()
{
    servoOsc.reset();
    servoLFO.reset();
    // Reset phase or any other state if necessary
}

void ServoEngine::updateParameters(const EngineParameterSet& allParams)
{
    // Extract servo-specific parameters
    currentLevel = allParams.servo.level;
    currentPitch = allParams.servo.pitch;
    currentModDepth = allParams.servo.modDepth;
    currentModRate = allParams.servo.modRate;

    // Potentially update isEnabledFlag based on a parameter if needed:
    // isEnabledFlag = allParams.servo.enabled; (if ServoParams had an 'enabled' field)
}

void ServoEngine::processAddingTo(juce::dsp::ProcessContextReplacing<float>& context)
{
    if (!isEnabledFlag || currentLevel < 0.001f) // Check enabled flag and negligible level
        return;

    auto& outputBlock = context.getOutputBlock();
    auto numSamples = outputBlock.getNumSamples();
    auto numChannels = outputBlock.getNumChannels();

    servoLFO.setFrequency(currentModRate);

    for (size_t channel = 0; channel < numChannels; ++channel)
    {
        auto* channelData = outputBlock.getChannelPointer(channel);

        for (size_t sample = 0; sample < numSamples; ++sample)
        {
            float lfoSample = servoLFO.processSample(0.0f);
            float pitchModAmount = currentPitch * lfoSample * currentModDepth;
            float modulatedPitch = currentPitch + pitchModAmount;
            modulatedPitch = std::max(20.0f, std::min(modulatedPitch, 20000.0f));

            servoOsc.setFrequency(modulatedPitch, true); // true for immediate update
            float servoSample = servoOsc.processSample(0.0f);

            // Add to existing channel data
            channelData[sample] += servoSample * currentLevel;
        }
    }
}

void ServoEngine::setEnabled(bool enabled)
{
    isEnabledFlag = enabled;
}

bool ServoEngine::getEnabled() const
{
    return isEnabledFlag;
}

double ServoEngine::getCPUUsage() const
{
    // Placeholder - implement proper measurement if needed
    return 0.0;
}

size_t ServoEngine::getMemoryUsage() const
{
    // Placeholder - calculate actual memory if needed
    // sizeof(*this) + sizeof(servoOsc) + sizeof(servoLFO) approx.
    return sizeof(*this) + 2 * sizeof(juce::dsp::Oscillator<float>);
}