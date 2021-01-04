/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AndrewHp02parametricEqAudioProcessorEditor::AndrewHp02parametricEqAudioProcessorEditor (AndrewHp02parametricEqAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (475, 500);

    auto& params = processor.getParameters();

    //Highs
    //----------------------------------------------------------
    AudioParameterFloat* audioParam = (AudioParameterFloat*)params.getUnchecked(0);
    mFilterFcHighSlider.setBounds(300, 440, 125, 50);
    mFilterFcHighSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    mFilterFcHighSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 70, 20);
    mFilterFcHighSlider.setRange(audioParam->range.start, audioParam->range.end);
    mFilterFcHighSlider.setValue(*audioParam);
    mFilterFcHighSlider.setTextValueSuffix(" Hz");
    mFilterFcHighSlider.addListener(this);
    addAndMakeVisible(mFilterFcHighSlider);
    addAndMakeVisible(highFcLabel);
    highFcLabel.setText("High Frequency", juce::dontSendNotification);
    highFcLabel.setJustificationType(juce::Justification::centred);
    highFcLabel.attachToComponent(&mFilterFcHighSlider, false);

    audioParam = (AudioParameterFloat*)params.getUnchecked(1);
    mGainHighSlider.setBounds(325, 50, 75, 250);
    mGainHighSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
    mGainHighSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 80, 30);
    mGainHighSlider.setRange(audioParam->range.start, audioParam->range.end);
    mGainHighSlider.setValue(*audioParam);
    mGainHighSlider.setTextValueSuffix(" dB");
    mGainHighSlider.addListener(this);
    addAndMakeVisible(mGainHighSlider);
    addAndMakeVisible(highGLabel);
    highGLabel.setText("High Shelf", juce::dontSendNotification);
    highGLabel.setJustificationType(juce::Justification::centred);
    highGLabel.attachToComponent(&mGainHighSlider, false);
    //----------------------------------------------------------

    //Mids
    //----------------------------------------------------------
    audioParam = (AudioParameterFloat*)params.getUnchecked(2);
    mFilterFcMidSlider.setBounds(175, 440, 125, 50);
    mFilterFcMidSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    mFilterFcMidSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 70, 20);
    mFilterFcMidSlider.setRange(audioParam->range.start, audioParam->range.end);
    mFilterFcMidSlider.setValue(*audioParam);
    mFilterFcMidSlider.setTextValueSuffix(" Hz");
    mFilterFcMidSlider.addListener(this);
    addAndMakeVisible(mFilterFcMidSlider);
    addAndMakeVisible(midFcLabel);
    midFcLabel.setText("Mid Frequency", juce::dontSendNotification);
    midFcLabel.setJustificationType(juce::Justification::centred);
    midFcLabel.attachToComponent(&mFilterFcMidSlider, false);

    audioParam = (AudioParameterFloat*)params.getUnchecked(3);
    mGainMidSlider.setBounds(200, 50, 75, 250);
    mGainMidSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
    mGainMidSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 80, 30);
    mGainMidSlider.setRange(audioParam->range.start, audioParam->range.end);
    mGainMidSlider.setValue(*audioParam);
    mGainMidSlider.setTextValueSuffix(" dB");
    mGainMidSlider.addListener(this);
    addAndMakeVisible(mGainMidSlider);
    addAndMakeVisible(midGLabel);
    midGLabel.setText("Peak/Notch", juce::dontSendNotification);
    midGLabel.setJustificationType(juce::Justification::centred);
    midGLabel.attachToComponent(&mGainMidSlider, false);

    audioParam = (AudioParameterFloat*)params.getUnchecked(4);
    mFilterQSlider.setBounds(190, 330, 100, 80);
    mFilterQSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    //mFilterQSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    mFilterQSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 70, 20);
    mFilterQSlider.setRange(audioParam->range.start, audioParam->range.end);
    mFilterQSlider.setValue(*audioParam);
    mFilterQSlider.addListener(this);
    addAndMakeVisible(mFilterQSlider);
    addAndMakeVisible(resonance);
    resonance.setText("Resonance", juce::dontSendNotification);
    resonance.setJustificationType(juce::Justification::centred);
    resonance.attachToComponent(&mFilterQSlider, false);

    //Lows
    //----------------------------------------------------------
    audioParam = (AudioParameterFloat*)params.getUnchecked(5);
    mFilterFcLowSlider.setBounds(50, 440, 125, 50);
    mFilterFcLowSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    mFilterFcLowSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 70, 20);
    mFilterFcLowSlider.setRange(audioParam->range.start, audioParam->range.end);
    mFilterFcLowSlider.setValue(*audioParam);
    mFilterFcLowSlider.setTextValueSuffix(" Hz");
    mFilterFcLowSlider.addListener(this);
    addAndMakeVisible(mFilterFcLowSlider);
    addAndMakeVisible(lowFcLabel);
    lowFcLabel.setText("Low Frequency", juce::dontSendNotification);
    lowFcLabel.setJustificationType(juce::Justification::centred);
    lowFcLabel.attachToComponent(&mFilterFcLowSlider, false);

    audioParam = (AudioParameterFloat*)params.getUnchecked(6);
    mGainLowSlider.setBounds(75, 50, 75, 250);
    mGainLowSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
    mGainLowSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 80, 30);
    mGainLowSlider.setRange(audioParam->range.start, audioParam->range.end);
    mGainLowSlider.setValue(*audioParam);
    mGainLowSlider.setTextValueSuffix(" dB");
    mGainLowSlider.addListener(this);
    addAndMakeVisible(mGainLowSlider);
    addAndMakeVisible(lowGLabel);
    lowGLabel.setText("Low Shelf", juce::dontSendNotification);
    lowGLabel.setJustificationType(juce::Justification::centred);
    lowGLabel.attachToComponent(&mGainLowSlider, false);
}

AndrewHp02parametricEqAudioProcessorEditor::~AndrewHp02parametricEqAudioProcessorEditor()
{
}

//==============================================================================
void AndrewHp02parametricEqAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText("Parametric EQ", getLocalBounds(), juce::Justification::centredTop, 1);
}

void AndrewHp02parametricEqAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void AndrewHp02parametricEqAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
    auto& params = processor.getParameters();
    if (slider == &mFilterFcHighSlider)
    {
        AudioParameterFloat* audioParam = (AudioParameterFloat*)params.getUnchecked(0);
        *audioParam = mFilterFcHighSlider.getValue();
    }
    else if (slider == &mGainHighSlider)
    {
        AudioParameterFloat* audioParam = (AudioParameterFloat*)params.getUnchecked(1);
        *audioParam = mGainHighSlider.getValue();
    }
    else if (slider == &mFilterFcMidSlider)
    {
        AudioParameterFloat* audioParam = (AudioParameterFloat*)params.getUnchecked(2);
        *audioParam = mFilterFcMidSlider.getValue();
    }
    else if (slider == &mGainMidSlider)
    {
        AudioParameterFloat* audioParam = (AudioParameterFloat*)params.getUnchecked(3);
        *audioParam = mGainMidSlider.getValue();
    }
    else if (slider == &mFilterQSlider)
    {
        AudioParameterFloat* audioParam = (AudioParameterFloat*)params.getUnchecked(4);
        *audioParam = mFilterQSlider.getValue();
    }
    else if (slider == &mFilterFcLowSlider)
    {
        AudioParameterFloat* audioParam = (AudioParameterFloat*)params.getUnchecked(5);
        *audioParam = mFilterFcLowSlider.getValue();
    }
    else if (slider == &mGainLowSlider)
    {
        AudioParameterFloat* audioParam = (AudioParameterFloat*)params.getUnchecked(6);
        *audioParam = mGainLowSlider.getValue();
    }
}