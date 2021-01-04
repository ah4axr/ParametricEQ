/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
using namespace juce;
class AndrewHp02parametricEqAudioProcessorEditor  : public juce::AudioProcessorEditor,
    public juce::Slider::Listener
{
public:
    AndrewHp02parametricEqAudioProcessorEditor (AndrewHp02parametricEqAudioProcessor&);
    ~AndrewHp02parametricEqAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void sliderValueChanged(Slider* slider) override;
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AndrewHp02parametricEqAudioProcessor& audioProcessor;

    Slider mFilterFcHighSlider;
    Slider mGainHighSlider;

    Slider mFilterFcMidSlider;
    Slider mGainMidSlider;
    Slider mFilterQSlider;

    Slider mFilterFcLowSlider;
    Slider mGainLowSlider;


    juce::Label highFcLabel;
    juce::Label highGLabel;
    juce::Label highGNLabel;
    juce::Label midFcLabel;
    juce::Label midGLabel;
    juce::Label resonance;
    juce::Label lowFcLabel;
    juce::Label lowGLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AndrewHp02parametricEqAudioProcessorEditor)
};
