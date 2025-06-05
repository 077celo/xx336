// Source/AudioEngine/MechaSoundEngine.cpp
#include "../Source/AudioEngine/MechaSoundEngine.h"
#include "../Source/AudioEngine/ServoEngine.h" // Include concrete engine implementations
#include "../Source/AudioEngine/PowerCoreEngine.h"
#include <juce_core/juce_core.h> // For juce::jmap

MechaSoundEngine::MechaSoundEngine()
{
    // Create and add sound engines
    // In Batch 1, only ServoEngine is added. Future engines will be added here.
    soundEngines.push_back(std::make_unique<ServoEngine>());
    soundEngines.push_back(std::make_unique<PowerCoreEngine>());
    // For example, later:
    // soundEngines.push_back(std::make_unique<PowerCoreEngine>());
    // soundEngines.push_back(std::make_unique<ThrusterEngine>());
}

MechaSoundEngine::~MechaSoundEngine()
{
    // unique_ptr will handle cleanup
}

void MechaSoundEngine::prepare(const juce::dsp::ProcessSpec& spec)
{
    // Prepare Hiss components
    noiseGen.prepare(spec);
    hissFilter.prepare(spec);
    hissFilter.setType(juce::dsp::StateVariableTPTFilterType::lowpass); // [cite: 18]

    // Prepare all managed sound engines
    for (auto& engine : soundEngines)
    {
        engine->prepare(spec);
    }
    reset();
}

void MechaSoundEngine::reset()
{
    // Reset Hiss components
    noiseGen.reset();
    hissFilter.reset();

    // Reset all managed sound engines
    for (auto& engine : soundEngines)
    {
        engine->reset();
    }
}

void MechaSoundEngine::process(juce::AudioBuffer<float>& buffer, const EngineParameterSet& allParams)
{
    // 1. Update parameters for all engines
    for (auto& engine : soundEngines)
    {
        engine->updateParameters(allParams);
        // Optionally, you could setEnabled here based on global or specific APVTS params
        // For example, if you add a global enable for each engine type in APVTS:
        // if (engine_is_servo) engine->setEnabled(apvts_servo_global_enable_param_value);
    }

    // 2. Process Hiss (directly in MechaSoundEngine for now)
    // Buffer should be cleared by PluginProcessor before this call if this is the first processing stage.
    // Or, MechaSoundEngine ensures it's cleared before hiss processing.
    // Assuming PluginProcessor clears it.

    juce::dsp::AudioBlock<float> tempProcessingBlock(buffer); // Create a block for processing
    juce::dsp::ProcessContextReplacing<float> hissContext(tempProcessingBlock);

    if (allParams.hiss.level > 0.001f)
    {
        hissFilter.setCutoffFrequency(allParams.hiss.cutoff);
        // Map resonance: Q range 0.1-18.0 to filter resonance 0.1-0.95 [cite: 1] (MechaSoundEngine.cpp)
        hissFilter.setResonance(juce::jmap(allParams.hiss.resonanceQ, 0.1f, 18.0f, 0.1f, 0.95f));

        noiseGen.process(hissContext); // tempProcessingBlock (buffer) now contains noise
        hissFilter.process(hissContext); // tempProcessingBlock (buffer) now contains filtered noise
        buffer.applyGain(allParams.hiss.level); // Apply hiss level to the main buffer
    }
    else
    {
        // If hiss is not active, ensure the buffer portion intended for hiss is silent.
        // If PluginProcessor cleared the buffer, this is fine.
        // If Hiss was supposed to write to a *part* of the buffer and other engines to others,
        // more complex buffer management is needed. Current assumption: Hiss is the base layer.
        // If hiss processed first and then others add, and hiss is 0, the buffer remains clear for others.
        // If hiss processing modifies the buffer that others will add to,
        // and hiss level is 0, it should effectively result in a clear buffer (or an untouched buffer)
        // before other engines process.
        // The applyGain(0) would effectively clear it IF it was full of hiss.
        // If noiseGen/hissFilter were skipped, buffer remains as it was (hopefully cleared by Processor).
        // Let's ensure buffer is what other engines expect if hiss is off:
        // If hiss was the *only* thing and is now off, buffer should be clear.
        // If hiss is one of many, and it's off, its contribution is zero.
        // The original code cleared the buffer (if hiss was off) *after* the hiss block.
        // This seems fine: if hiss is off, buffer is clear after this stage from PluginProcessor's clear.
        // If hiss is on but level is 0, applyGain(0) makes its part 0.
    }


    // 3. Process all managed sound engines
    // Each engine's processAddingTo will add its sound to the buffer.
    // The buffer currently contains the (potentially silent) hiss sound.
    for (auto& engine : soundEngines)
    {
        if (engine->getEnabled())
        {
            // Create a new context for each engine to ensure they operate on the current state of the buffer
            juce::dsp::AudioBlock<float> engineBlock(buffer);
            juce::dsp::ProcessContextReplacing<float> engineContext(engineBlock);
            engine->processAddingTo(engineContext);
        }
    }
}