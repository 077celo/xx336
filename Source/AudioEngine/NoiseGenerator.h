#pragma once

#include <juce_dsp/juce_dsp.h>
#include <random> // For std::mt19937, std::uniform_real_distribution, std::random_device

// --- Custom Noise Generator ---
class SimpleNoiseGenerator
{
public:
    SimpleNoiseGenerator() : distribution(-1.0f, 1.0f) {}

    void prepare(const juce::dsp::ProcessSpec& spec)
    {
        sampleRate = spec.sampleRate;
        // Seed the generator. Using random_device for a non-deterministic seed.
        // Per your technical documentation: "Cryptographically secure seeding" [cite: 18]
        // std::random_device is suitable for this.
        generator.seed(std::random_device{}());
    }

    void reset()
    {
        // Optionally re-seed or reset any state if needed.
        // For mt19937, re-seeding is often done.
        generator.seed(std::random_device{}());
    }

    template<typename ProcessContext>
    void process(const ProcessContext& context)
    {
        auto&& outBlock = context.getOutputBlock();

        for (size_t channel = 0; channel < outBlock.getNumChannels(); ++channel)
        {
            auto* channelData = outBlock.getChannelPointer(channel);
            for (size_t sample = 0; sample < outBlock.getNumSamples(); ++sample)
            {
                channelData[sample] = distribution(generator);
            }
        }
    }

    // Process a single sample (if needed, though block processing is typical for DSP modules)
    float processSample()
    {
        return distribution(generator);
    }

private:
    std::mt19937 generator; // Mersenne Twister [cite: 18]
    std::uniform_real_distribution<float> distribution; // Uniform real distribution [-1.0, 1.0] [cite: 18]
    double sampleRate = 44100.0; // Initialized, then set in prepare
};