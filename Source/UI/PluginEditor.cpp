/*
  ==============================================================================

    PluginEditor.cpp
    GUNDAM_Motion - Mecha Movement Sound Generator

    This file contains the implementation of the plugin editor class.
    It handles the creation, layout, and interaction of GUI components.

  ==============================================================================
*/

#include "../Source/PluginProcessor.h"
#include "../UI/PluginEditor.h"
#include "../Source/UI/Components/UIGraph.h"

//==============================================================================
MechaSoundGeneratorAudioProcessorEditor::MechaSoundGeneratorAudioProcessorEditor(MechaSoundGeneratorAudioProcessor& p)
    : AudioProcessorEditor(&p), processorRef(p)
{
    // --- Hiss Section ---
    // Hiss Level Slider & Label
    addAndMakeVisible(hissLevelSlider);
    addAndMakeVisible(hissLevelLabel);
    hissLevelSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    hissLevelSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    hissLevelLabel.setText("Hiss Level", juce::dontSendNotification);
    hissLevelLabel.attachToComponent(&hissLevelSlider, false);
    hissLevelLabel.setJustificationType(juce::Justification::centredTop);
    hissLevelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.apvts, ParameterIDs::hissLevel, hissLevelSlider);

    // Hiss Cutoff Slider & Label
    addAndMakeVisible(hissCutoffSlider);
    addAndMakeVisible(hissCutoffLabel);
    hissCutoffSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    hissCutoffSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    hissCutoffSlider.setSkewFactorFromMidPoint(1000.0); // Logarithmic scale for frequency
    hissCutoffLabel.setText("Hiss Cutoff", juce::dontSendNotification);
    hissCutoffLabel.attachToComponent(&hissCutoffSlider, false);
    hissCutoffLabel.setJustificationType(juce::Justification::centredTop);
    hissCutoffAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.apvts, ParameterIDs::hissCutoff, hissCutoffSlider);

    // Hiss Resonance Slider & Label
    addAndMakeVisible(hissResonanceSlider);
    addAndMakeVisible(hissResonanceLabel);
    hissResonanceSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    hissResonanceSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    hissResonanceLabel.setText("Hiss Reso", juce::dontSendNotification);
    hissResonanceLabel.attachToComponent(&hissResonanceSlider, false);
    hissResonanceLabel.setJustificationType(juce::Justification::centredTop);
    hissResonanceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.apvts, ParameterIDs::hissResonance, hissResonanceSlider);

    // --- Servo Section ---
    // Servo Level Slider & Label
    addAndMakeVisible(servoLevelSlider);
    addAndMakeVisible(servoLevelLabel);
    servoLevelSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    servoLevelSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    servoLevelLabel.setText("Servo Level", juce::dontSendNotification);
    servoLevelLabel.attachToComponent(&servoLevelSlider, false);
    servoLevelLabel.setJustificationType(juce::Justification::centredTop);
    servoLevelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.apvts, ParameterIDs::servoLevel, servoLevelSlider);

    // Servo Pitch Slider & Label
    addAndMakeVisible(servoPitchSlider);
    addAndMakeVisible(servoPitchLabel);
    servoPitchSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    servoPitchSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    servoPitchSlider.setSkewFactorFromMidPoint(500.0); // Logarithmic scale for frequency
    servoPitchLabel.setText("Servo Pitch", juce::dontSendNotification);
    servoPitchLabel.attachToComponent(&servoPitchSlider, false);
    servoPitchLabel.setJustificationType(juce::Justification::centredTop);
    servoPitchAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.apvts, ParameterIDs::servoPitch, servoPitchSlider);

    // Servo Mod Depth Slider & Label
    addAndMakeVisible(servoModDepthSlider);
    addAndMakeVisible(servoModDepthLabel);
    servoModDepthSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    servoModDepthSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    servoModDepthLabel.setText("Servo Mod Depth", juce::dontSendNotification);
    servoModDepthLabel.attachToComponent(&servoModDepthSlider, false);
    servoModDepthLabel.setJustificationType(juce::Justification::centredTop);
    servoModDepthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.apvts, ParameterIDs::servoModDepth, servoModDepthSlider);

    // Servo Mod Rate Slider & Label
    addAndMakeVisible(servoModRateSlider);
    addAndMakeVisible(servoModRateLabel);
    servoModRateSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    servoModRateSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    servoModRateSlider.setSkewFactorFromMidPoint(3.0); // Slight skew for rate
    servoModRateLabel.setText("Servo Mod Rate", juce::dontSendNotification);
    servoModRateLabel.attachToComponent(&servoModRateSlider, false);
    servoModRateLabel.setJustificationType(juce::Justification::centredTop);
    servoModRateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.apvts, ParameterIDs::servoModRate, servoModRateSlider);

    // --- PowerCore Section --- [cite: 35]
// Hum Level
    addAndMakeVisible(powerCoreHumLevelSlider);
    addAndMakeVisible(powerCoreHumLevelLabel);
    powerCoreHumLevelSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    powerCoreHumLevelSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    powerCoreHumLevelLabel.setText("PC Hum Level", juce::dontSendNotification);
    powerCoreHumLevelLabel.attachToComponent(&powerCoreHumLevelSlider, false);
    powerCoreHumLevelLabel.setJustificationType(juce::Justification::centredTop);
    powerCoreHumLevelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.apvts, ParameterIDs::powerCoreHumLevel, powerCoreHumLevelSlider);

    // Fundamental Pitch
    addAndMakeVisible(powerCorePitchSlider);
    addAndMakeVisible(powerCorePitchLabel);
    powerCorePitchSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    powerCorePitchSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    powerCorePitchSlider.setSkewFactorFromMidPoint(100.0); // Log for pitch [cite: 10]
    powerCorePitchLabel.setText("PC Pitch", juce::dontSendNotification);
    powerCorePitchLabel.attachToComponent(&powerCorePitchSlider, false);
    powerCorePitchLabel.setJustificationType(juce::Justification::centredTop);
    powerCorePitchAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.apvts, ParameterIDs::powerCoreFundamentalPitch, powerCorePitchSlider);

    // Hum Complexity
    addAndMakeVisible(powerCoreComplexitySlider);
    addAndMakeVisible(powerCoreComplexityLabel);
    powerCoreComplexitySlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    powerCoreComplexitySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    powerCoreComplexityLabel.setText("PC Complexity", juce::dontSendNotification);
    powerCoreComplexityLabel.attachToComponent(&powerCoreComplexitySlider, false);
    powerCoreComplexityLabel.setJustificationType(juce::Justification::centredTop);
    powerCoreComplexityAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.apvts, ParameterIDs::powerCoreHumComplexity, powerCoreComplexitySlider);

    // Pulsation Rate
    addAndMakeVisible(powerCorePulsationRateSlider);
    addAndMakeVisible(powerCorePulsationRateLabel);
    powerCorePulsationRateSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    powerCorePulsationRateSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    powerCorePulsationRateLabel.setText("PC Pulse Rate", juce::dontSendNotification);
    powerCorePulsationRateLabel.attachToComponent(&powerCorePulsationRateSlider, false);
    powerCorePulsationRateLabel.setJustificationType(juce::Justification::centredTop);
    powerCorePulsationRateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.apvts, ParameterIDs::powerCorePulsationRate, powerCorePulsationRateSlider);

    // Pulsation Depth
    addAndMakeVisible(powerCorePulsationDepthSlider);
    addAndMakeVisible(powerCorePulsationDepthLabel);
    powerCorePulsationDepthSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    powerCorePulsationDepthSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    powerCorePulsationDepthLabel.setText("PC Pulse Depth", juce::dontSendNotification);
    powerCorePulsationDepthLabel.attachToComponent(&powerCorePulsationDepthSlider, false);
    powerCorePulsationDepthLabel.setJustificationType(juce::Justification::centredTop);
    powerCorePulsationDepthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.apvts, ParameterIDs::powerCorePulsationDepth, powerCorePulsationDepthSlider);

    // Activation Trigger Button [cite: 10]
    addAndMakeVisible(powerCoreActivationButton);
    addAndMakeVisible(powerCoreActivationLabel);
    powerCoreActivationButton.setButtonText("Activate");
    powerCoreActivationLabel.setText("PC Activation", juce::dontSendNotification);
    powerCoreActivationLabel.attachToComponent(&powerCoreActivationButton, false); // Adjust attachment if needed
    powerCoreActivationLabel.setJustificationType(juce::Justification::centredTop); // Or leftOfComponent
    powerCoreActivationAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        processorRef.apvts, ParameterIDs::powerCoreActivationTrigger, powerCoreActivationButton);

    // Activation Time
    addAndMakeVisible(powerCoreActivationTimeSlider);
    addAndMakeVisible(powerCoreActivationTimeLabel);
    powerCoreActivationTimeSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    powerCoreActivationTimeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    powerCoreActivationTimeLabel.setText("PC Act Time", juce::dontSendNotification);
    powerCoreActivationTimeLabel.attachToComponent(&powerCoreActivationTimeSlider, false);
    powerCoreActivationTimeLabel.setJustificationType(juce::Justification::centredTop);
    powerCoreActivationTimeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.apvts, ParameterIDs::powerCoreActivationTime, powerCoreActivationTimeSlider);

    // Energy Type
    addAndMakeVisible(powerCoreEnergyTypeSlider);
    addAndMakeVisible(powerCoreEnergyTypeLabel);
    powerCoreEnergyTypeSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    powerCoreEnergyTypeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    powerCoreEnergyTypeLabel.setText("PC Energy Type", juce::dontSendNotification);
    powerCoreEnergyTypeLabel.attachToComponent(&powerCoreEnergyTypeSlider, false);
    powerCoreEnergyTypeLabel.setJustificationType(juce::Justification::centredTop);
    powerCoreEnergyTypeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.apvts, ParameterIDs::powerCoreEnergyType, powerCoreEnergyTypeSlider);

    // Filter Cutoff
    addAndMakeVisible(powerCoreFilterCutoffSlider);
    addAndMakeVisible(powerCoreFilterCutoffLabel);
    powerCoreFilterCutoffSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    powerCoreFilterCutoffSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    powerCoreFilterCutoffSlider.setSkewFactorFromMidPoint(1000.0); // Log for frequency
    powerCoreFilterCutoffLabel.setText("PC Filter Cutoff", juce::dontSendNotification);
    powerCoreFilterCutoffLabel.attachToComponent(&powerCoreFilterCutoffSlider, false);
    powerCoreFilterCutoffLabel.setJustificationType(juce::Justification::centredTop);
    powerCoreFilterCutoffAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.apvts, ParameterIDs::powerCoreFilterCutoff, powerCoreFilterCutoffSlider);

    // Filter Resonance
    addAndMakeVisible(powerCoreFilterResonanceSlider);
    addAndMakeVisible(powerCoreFilterResonanceLabel);
    powerCoreFilterResonanceSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    powerCoreFilterResonanceSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    powerCoreFilterResonanceLabel.setText("PC Filter Reso", juce::dontSendNotification);
    powerCoreFilterResonanceLabel.attachToComponent(&powerCoreFilterResonanceSlider, false);
    powerCoreFilterResonanceLabel.setJustificationType(juce::Justification::centredTop);
    powerCoreFilterResonanceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.apvts, ParameterIDs::powerCoreFilterResonance, powerCoreFilterResonanceSlider);


    // --- Master Section ---
    // Master Gain Slider & Label
    addAndMakeVisible(masterGainSlider);
    addAndMakeVisible(masterGainLabel);
    masterGainSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    masterGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    masterGainLabel.setText("Master Gain", juce::dontSendNotification);
    masterGainLabel.attachToComponent(&masterGainSlider, false);
    masterGainLabel.setJustificationType(juce::Justification::centredTop);
    masterGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.apvts, ParameterIDs::masterGain, masterGainSlider);

    // Set the size of the editor window.
    // This size can be adjusted based on the number of controls and desired layout.
    setSize(800, 500); // Increased height for more sliders
}

MechaSoundGeneratorAudioProcessorEditor::~MechaSoundGeneratorAudioProcessorEditor()
{
    // Attachments are std::unique_ptr, so they will be automatically cleaned up.
}

//==============================================================================
void MechaSoundGeneratorAudioProcessorEditor::paint(juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    // You can add drawing code here to customize the look of your plugin editor.
    // For example, drawing section titles or borders.
    // g.setColour(juce::Colours::white);
    // g.setFont(15.0f);
    // g.drawFittedText("Mecha Sound Generator", getLocalBounds(), juce::Justification::centredTop, 1);
}

void MechaSoundGeneratorAudioProcessorEditor::resized()
{
    // This creates a grid layout with 8 knobs per row
    // and reduces knob size by 20%

    auto bounds = getLocalBounds();
    bounds.reduce(20, 20); // Add some padding around the edges

    // Reduced knob size by 20% (from 90 to 72)
    int sliderWidth = 72;
    int sliderHeight = 72;
    int buttonHeight = 24; // For toggle button (also reduced by 20%)
    int verticalSpacing = 8; // Space between rows
    int horizontalSpacing = 8; // Space between columns
    int sectionSpacing = 20; // Space between sections

    // Calculate how many knobs can fit per row (max 8)
    int knobsPerRow = 8;
    int totalRowWidth = (knobsPerRow * sliderWidth) + ((knobsPerRow - 1) * horizontalSpacing);

    // Center the grid if the window is wider than needed
    int gridStartX = bounds.getX() + (bounds.getWidth() - totalRowWidth) / 2;
    if (gridStartX < bounds.getX()) gridStartX = bounds.getX(); // Don't go outside bounds

    int currentX = gridStartX;
    int currentY = bounds.getY();
    int currentKnobInRow = 0;

    // Helper lambda to place a knob and advance position
    auto placeKnob = [&](juce::Component& knob) {
        knob.setBounds(currentX, currentY, sliderWidth, sliderHeight);
        currentX += sliderWidth + horizontalSpacing;
        currentKnobInRow++;

        // Check if we need to move to next row
        if (currentKnobInRow >= knobsPerRow) {
            currentX = gridStartX;
            currentY += sliderHeight + verticalSpacing;
            currentKnobInRow = 0;
        }
        };

    // Helper lambda to place a button (different height)
    auto placeButton = [&](juce::Component& button) {
        // Center the button vertically within the knob height space
        int buttonY = currentY + (sliderHeight - buttonHeight) / 2;
        button.setBounds(currentX, buttonY, sliderWidth, buttonHeight);
        currentX += sliderWidth + horizontalSpacing;
        currentKnobInRow++;

        // Check if we need to move to next row
        if (currentKnobInRow >= knobsPerRow) {
            currentX = gridStartX;
            currentY += sliderHeight + verticalSpacing;
            currentKnobInRow = 0;
        }
        };

    // Helper lambda to start a new section
    auto startNewSection = [&]() {
        if (currentKnobInRow > 0) {
            // Move to next row if current row is not empty
            currentX = gridStartX;
            currentY += sliderHeight + verticalSpacing;
            currentKnobInRow = 0;
        }
        // Add section spacing
        currentY += sectionSpacing;
        };

    // --- Hiss Section ---
    placeKnob(hissLevelSlider);
    placeKnob(hissCutoffSlider);
    placeKnob(hissResonanceSlider);

    startNewSection();

    // --- Servo Section ---
    placeKnob(servoLevelSlider);
    placeKnob(servoPitchSlider);
    placeKnob(servoModDepthSlider);
    placeKnob(servoModRateSlider);

    startNewSection();

    // --- PowerCore Section ---
    placeKnob(powerCoreHumLevelSlider);
    placeKnob(powerCorePitchSlider);
    placeKnob(powerCoreComplexitySlider);
    placeKnob(powerCorePulsationRateSlider);
    placeKnob(powerCorePulsationDepthSlider);

    // Special handling for the activation button
    placeButton(powerCoreActivationButton);

    placeKnob(powerCoreActivationTimeSlider);
    placeKnob(powerCoreEnergyTypeSlider);
    placeKnob(powerCoreFilterCutoffSlider);
    placeKnob(powerCoreFilterResonanceSlider);

    startNewSection();

    // --- Master Section ---
    placeKnob(masterGainSlider);

    // Calculate the total height needed and update window size if necessary
    int totalHeight = currentY + sliderHeight + 40; // Add bottom padding
    if (currentKnobInRow > 0) {
        // Account for the last row
        totalHeight = currentY + sliderHeight + 40;
    }

    // Update the plugin window size if needed
    // Note: You might want to call this in the constructor instead
    // setSize(totalRowWidth + 40, totalHeight);
}

// Updated constructor section - Add this to your constructor after creating all sliders:
void MechaSoundGeneratorAudioProcessorEditor::setupSlidersWithReducedSize()
{
    // Set reduced text box size for all sliders (20% smaller)
    int textBoxWidth = 64;  // Reduced from 80
    int textBoxHeight = 16; // Reduced from 20

    // Apply to all sliders
    hissLevelSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, textBoxWidth, textBoxHeight);
    hissCutoffSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, textBoxWidth, textBoxHeight);
    hissResonanceSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, textBoxWidth, textBoxHeight);

    servoLevelSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, textBoxWidth, textBoxHeight);
    servoPitchSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, textBoxWidth, textBoxHeight);
    servoModDepthSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, textBoxWidth, textBoxHeight);
    servoModRateSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, textBoxWidth, textBoxHeight);

    powerCoreHumLevelSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, textBoxWidth, textBoxHeight);
    powerCorePitchSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, textBoxWidth, textBoxHeight);
    powerCoreComplexitySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, textBoxWidth, textBoxHeight);
    powerCorePulsationRateSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, textBoxWidth, textBoxHeight);
    powerCorePulsationDepthSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, textBoxWidth, textBoxHeight);
    powerCoreActivationTimeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, textBoxWidth, textBoxHeight);
    powerCoreEnergyTypeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, textBoxWidth, textBoxHeight);
    powerCoreFilterCutoffSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, textBoxWidth, textBoxHeight);
    powerCoreFilterResonanceSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, textBoxWidth, textBoxHeight);

    masterGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, textBoxWidth, textBoxHeight);
}