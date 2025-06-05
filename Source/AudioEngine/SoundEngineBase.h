// Source/AudioEngine/SoundEngineBase.h
#pragma once

#include <juce_dsp/juce_dsp.h>

// Forward declaration for the main parameter structure.
// This will be defined in Parameters/Parameters.h
struct EngineParameterSet;

class SoundEngineBase
{
public:
    virtual ~SoundEngineBase() = default;

    // DSP Lifecycle
    /** @brief Prepares the engine for playback. */
    virtual void prepare(const juce::dsp::ProcessSpec& spec) = 0;

    /** @brief Resets the internal state of the engine. */
    virtual void reset() = 0;

    /** @brief Processes audio and adds its output to the context's audio block.
        It's the responsibility of the concrete engine to correctly mix or add
        its sound to the provided audio block.
    */
    virtual void processAddingTo(juce::dsp::ProcessContextReplacing<float>& context) = 0;

    // Parameter Management
    /** @brief Updates the engine's parameters from the global set.
        Each engine will extract its relevant parameters from 'allParams'.
    */
    virtual void updateParameters(const EngineParameterSet& allParams) = 0;

    /** @brief Enables or disables the engine's processing. */
    virtual void setEnabled(bool enabled) = 0;

    /** @brief Returns true if the engine is currently enabled for processing. */
    virtual bool getEnabled() const = 0;

    // Performance Monitoring (as per Technical Specifications)
    /** @brief Returns the estimated CPU usage of this engine (0.0 to 1.0). */
    virtual double getCPUUsage() const = 0;

    /** @brief Returns the estimated memory usage of this engine in bytes. */
    virtual size_t getMemoryUsage() const = 0;

protected:
    std::atomic<bool> isEnabledFlag{ false }; // Internal flag to store enabled state
    double currentSampleRate = 44100.0;
    int currentBlockSize = 512;
};