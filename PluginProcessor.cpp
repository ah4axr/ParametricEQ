/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AndrewHp02parametricEqAudioProcessor::AndrewHp02parametricEqAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    //High params
    addParameter(mFilterFcParamHigh = new juce::AudioParameterFloat("Filter Cutoff High (Hz)",
        "FiltFcHigh",
        2000.0f,
        20000.0f,
        10000.0f
    ));
    addParameter(mGainParamHigh = new AudioParameterFloat(
        "gainDBHigh",
        "GainDBHigh",
        -20.0f,
        20.0f,
        0.0f
    ));

    //Mid Params
    addParameter(mFilterFcParamMid = new juce::AudioParameterFloat("Filter Cutoff Mid (Hz)",
        "FiltFcMid",
        200.0f,
        2000.0f,
        1000.0f
    ));
    addParameter(mGainParamMid = new AudioParameterFloat(
        "gainDBMid",
        "GainDBMid",
        -20.0f,
        20.0f,
        0.0f
    ));
    addParameter(mFilterQParam = new juce::AudioParameterFloat("Filter Q",
        "FiltQ",
        1.0f,
        10.0f,
        5.0
    ));

    //Low params
    addParameter(mFilterFcParamLow = new juce::AudioParameterFloat("Filter Cutoff Low (Hz)",
        "FiltFcLow",
        20.0f,
        200.0f,
        100.0f
    ));
    addParameter(mGainParamLow = new AudioParameterFloat(
        "gainDBLow",
        "GainDBLow",
        -20.0f,
        20.0f,
        0.0f
    ));
}

AndrewHp02parametricEqAudioProcessor::~AndrewHp02parametricEqAudioProcessor()
{
}

//==============================================================================
const juce::String AndrewHp02parametricEqAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AndrewHp02parametricEqAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AndrewHp02parametricEqAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AndrewHp02parametricEqAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AndrewHp02parametricEqAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AndrewHp02parametricEqAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AndrewHp02parametricEqAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AndrewHp02parametricEqAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String AndrewHp02parametricEqAudioProcessor::getProgramName (int index)
{
    return {};
}

void AndrewHp02parametricEqAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void AndrewHp02parametricEqAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    mFs = sampleRate;
}

void AndrewHp02parametricEqAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AndrewHp02parametricEqAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void AndrewHp02parametricEqAudioProcessor::calcAlgorithmParams()
{
    //get user params
    float fcHigh = mFilterFcParamHigh->get();
    float gHigh = mGainParamHigh->get();

    //calc filter coefficients array of floats
    float coeffs[5];
    Mu45FilterCalc::calcCoeffsHighShelf(coeffs, fcHigh, gHigh, mFs);
    //set coefficients for each filter
    mFilterLHigh.setCoefficients(coeffs[0], coeffs[1], coeffs[2], coeffs[3], coeffs[4]);
    mFilterRHigh.setCoefficients(coeffs[0], coeffs[1], coeffs[2], coeffs[3], coeffs[4]);

    float fcMid = mFilterFcParamMid->get();
    float gMid = mGainParamMid->get();
    float Q = mFilterQParam->get();

    Mu45FilterCalc::calcCoeffsPeak(coeffs, fcMid, gMid, Q, mFs);
    //set coefficients for each filter
    mFilterLMid.setCoefficients(coeffs[0], coeffs[1], coeffs[2], coeffs[3], coeffs[4]);
    mFilterRMid.setCoefficients(coeffs[0], coeffs[1], coeffs[2], coeffs[3], coeffs[4]);

    float fcLow = mFilterFcParamLow->get();
    float gLow = mGainParamLow->get();
    Mu45FilterCalc::calcCoeffsLowShelf(coeffs, fcLow, gLow, mFs);
    //set coefficients for each filter
    mFilterLLow.setCoefficients(coeffs[0], coeffs[1], coeffs[2], coeffs[3], coeffs[4]);
    mFilterRLow.setCoefficients(coeffs[0], coeffs[1], coeffs[2], coeffs[3], coeffs[4]);
}

void AndrewHp02parametricEqAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.

    calcAlgorithmParams();

    auto* channelDataLeft = buffer.getWritePointer(0);
    auto* channelDataRight = buffer.getWritePointer(1);
    for (int samp = 0; samp < buffer.getNumSamples(); samp++)
    {
        channelDataLeft[samp] = mFilterLLow.tick(mFilterLMid.tick(mFilterLHigh.tick(channelDataLeft[samp])));
        channelDataRight[samp] = mFilterRLow.tick(mFilterRMid.tick(mFilterRHigh.tick(channelDataRight[samp])));
    }
}

//==============================================================================
bool AndrewHp02parametricEqAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AndrewHp02parametricEqAudioProcessor::createEditor()
{
    return new AndrewHp02parametricEqAudioProcessorEditor (*this);
}

//==============================================================================
void AndrewHp02parametricEqAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void AndrewHp02parametricEqAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AndrewHp02parametricEqAudioProcessor();
}
