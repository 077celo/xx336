// Source/PluginProcessor.cpp
#include "../Source/PluginProcessor.h"
#include "../Source/UI/PluginEditor.h" // Make sure this path is correct
// Parameters.h is included via PluginProcessor.h
// MechaSoundEngine.h is included via PluginProcessor.h

//==============================================================================
MechaSoundGeneratorAudioProcessor::MechaSoundGeneratorAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        // For a synth/generator, we typically don't declare inputs
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    ),
#else
    :
#endif
apvts(*this, nullptr, "MechaSoundParams", createParameterLayout())
{
    // Cache the raw parameter pointers from APVTS
    hissLevelParam = apvts.getRawParameterValue(ParameterIDs::hissLevel);
    hissCutoffParam = apvts.getRawParameterValue(ParameterIDs::hissCutoff);
    hissResonanceParam = apvts.getRawParameterValue(ParameterIDs::hissResonance);

    servoLevelParam = apvts.getRawParameterValue(ParameterIDs::servoLevel);
    servoPitchParam = apvts.getRawParameterValue(ParameterIDs::servoPitch);
    servoModDepthParam = apvts.getRawParameterValue(ParameterIDs::servoModDepth);
    servoModRateParam = apvts.getRawParameterValue(ParameterIDs::servoModRate);

    // Initialize PowerCore parameter pointers
    powerCoreHumLevelParam = apvts.getRawParameterValue(ParameterIDs::powerCoreHumLevel);
    powerCoreFundamentalPitchParam = apvts.getRawParameterValue(ParameterIDs::powerCoreFundamentalPitch);
    powerCoreHumComplexityParam = apvts.getRawParameterValue(ParameterIDs::powerCoreHumComplexity);
    powerCorePulsationRateParam = apvts.getRawParameterValue(ParameterIDs::powerCorePulsationRate);
    powerCorePulsationDepthParam = apvts.getRawParameterValue(ParameterIDs::powerCorePulsationDepth);
    // Corrected: AudioParameterBool uses a float internally (0.0 or 1.0). getRawParameterValue returns std::atomic<float>*.
    // No cast to std::atomic<bool>* is needed or correct here.
    powerCoreActivationTriggerParam = apvts.getRawParameterValue(ParameterIDs::powerCoreActivationTrigger);
    powerCoreActivationTimeParam = apvts.getRawParameterValue(ParameterIDs::powerCoreActivationTime);
    powerCoreEnergyTypeParam = apvts.getRawParameterValue(ParameterIDs::powerCoreEnergyType);
    powerCoreFilterCutoffParam = apvts.getRawParameterValue(ParameterIDs::powerCoreFilterCutoff);
    powerCoreFilterResonanceParam = apvts.getRawParameterValue(ParameterIDs::powerCoreFilterResonance);

    masterGainParam = apvts.getRawParameterValue(ParameterIDs::masterGain);

    jassert(hissLevelParam != nullptr && hissCutoffParam != nullptr && hissResonanceParam != nullptr);
    jassert(servoLevelParam != nullptr && servoPitchParam != nullptr && servoModDepthParam != nullptr && servoModRateParam != nullptr);
    jassert(powerCoreHumLevelParam != nullptr && powerCoreFundamentalPitchParam != nullptr && powerCoreHumComplexityParam != nullptr &&
        powerCorePulsationRateParam != nullptr && powerCorePulsationDepthParam != nullptr && powerCoreActivationTriggerParam != nullptr &&
        powerCoreActivationTimeParam != nullptr && powerCoreEnergyTypeParam != nullptr && powerCoreFilterCutoffParam != nullptr && powerCoreFilterResonanceParam != nullptr);
    jassert(masterGainParam != nullptr);
}

MechaSoundGeneratorAudioProcessor::~MechaSoundGeneratorAudioProcessor() noexcept
{
}

//==============================================================================
const juce::String MechaSoundGeneratorAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MechaSoundGeneratorAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool MechaSoundGeneratorAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool MechaSoundGeneratorAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double MechaSoundGeneratorAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MechaSoundGeneratorAudioProcessor::getNumPrograms()
{
    return 1;
}

int MechaSoundGeneratorAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MechaSoundGeneratorAudioProcessor::setCurrentProgram(int index)
{
    juce::ignoreUnused(index);
}

const juce::String MechaSoundGeneratorAudioProcessor::getProgramName(int index)
{
    juce::ignoreUnused(index);
    return {};
}

void MechaSoundGeneratorAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
    juce::ignoreUnused(index, newName);
}

//==============================================================================
void MechaSoundGeneratorAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = (juce::uint32)samplesPerBlock;
    spec.numChannels = (juce::uint32)getTotalNumOutputChannels();

    mechaSoundEngine.prepare(spec);
    releaseResources();
}

void MechaSoundGeneratorAudioProcessor::releaseResources()
{
    mechaSoundEngine.reset();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MechaSoundGeneratorAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    if (layouts.getMainOutputChannelSet() == juce::AudioChannelSet::mono()
        || layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo())
    {
#if ! JucePlugin_IsSynth
        if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
            return false;
#endif
        return true;
    }
    return false;
#endif
}
#endif

void MechaSoundGeneratorAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    for (int channel = 0; channel < totalNumOutputChannels; ++channel)
        buffer.clear(channel, 0, buffer.getNumSamples());


    EngineParameterSet currentParams;

    currentParams.hiss.level = hissLevelParam->load();
    currentParams.hiss.cutoff = hissCutoffParam->load();
    currentParams.hiss.resonanceQ = hissResonanceParam->load();

    currentParams.servo.level = servoLevelParam->load();
    currentParams.servo.pitch = servoPitchParam->load();
    currentParams.servo.modDepth = servoModDepthParam->load();
    currentParams.servo.modRate = servoModRateParam->load();

    currentParams.powerCore.humLevel = powerCoreHumLevelParam->load();
    currentParams.powerCore.fundamentalPitch = powerCoreFundamentalPitchParam->load();
    currentParams.powerCore.humComplexity = powerCoreHumComplexityParam->load();
    currentParams.powerCore.pulsationRate = powerCorePulsationRateParam->load();
    currentParams.powerCore.pulsationDepth = powerCorePulsationDepthParam->load();
    // For AudioParameterBool, load() returns float (0.0f or 1.0f). Convert to bool.
    currentParams.powerCore.activationTrigger = powerCoreActivationTriggerParam->load() > 0.5f;
    currentParams.powerCore.activationTime = powerCoreActivationTimeParam->load();
    currentParams.powerCore.energyType = powerCoreEnergyTypeParam->load();
    currentParams.powerCore.filterCutoff = powerCoreFilterCutoffParam->load();       // This line should now be fine
    currentParams.powerCore.filterResonance = powerCoreFilterResonanceParam->load(); // This line should now be fine

    mechaSoundEngine.process(buffer, currentParams);

    const float currentMasterGain = masterGainParam->load();
    buffer.applyGain(currentMasterGain);

    juce::ignoreUnused(midiMessages);
}

//==============================================================================
bool MechaSoundGeneratorAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* MechaSoundGeneratorAudioProcessor::createEditor()
{
    return new MechaSoundGeneratorAudioProcessorEditor(*this);
}

//==============================================================================
void MechaSoundGeneratorAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    if (xml.get() != nullptr)
        copyXmlToBinary(*xml, destData);
}

void MechaSoundGeneratorAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(apvts.state.getType()))
            apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
}

//==============================================================================
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MechaSoundGeneratorAudioProcessor();
}
