/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
WaveshaperBasicAudioProcessor::WaveshaperBasicAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this, nullptr, juce::Identifier("Parameters"), createParameters() )//Constructor for apvts
#endif
{
    apvts.state.addListener(this);
}

WaveshaperBasicAudioProcessor::~WaveshaperBasicAudioProcessor()
{
}

//==============================================================================
const juce::String WaveshaperBasicAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool WaveshaperBasicAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool WaveshaperBasicAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool WaveshaperBasicAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double WaveshaperBasicAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int WaveshaperBasicAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int WaveshaperBasicAudioProcessor::getCurrentProgram()
{
    return 0;
}

void WaveshaperBasicAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String WaveshaperBasicAudioProcessor::getProgramName (int index)
{
    return {};
}

void WaveshaperBasicAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void WaveshaperBasicAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    auto &waveshaper = processorChain.get<waveshaperIndex>();
    
    waveshaper.functionToUse = [](float x)
    {
        //return juce::jlimit(float (-0.1), float (0.1), x);
        //return std::tanh (x);
        return x / (std::abs(x) + 1);
        //return std::pow(x, 3.0f);
    };
    waveshapeFunctionCurrent = "x/abs(x)+1";
    waveshapeFunction = "x/abs(x)+1";
    
    auto &preGain = processorChain.get<preGainIndex>();
    preGain.setGainDecibels(0.0f);
    
    auto &postGain = processorChain.get<postGainIndex>();
    postGain.setGainDecibels(0.0f);
    
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();
    
    processorChain.prepare(spec);
    
    /*juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;    
    spec.numChannels = getTotalNumOutputChannels();
    
    waveShaper.prepare(spec);*/
}

void WaveshaperBasicAudioProcessor::reset()
{
    processorChain.reset();
}

void WaveshaperBasicAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool WaveshaperBasicAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
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

void WaveshaperBasicAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    if(waveshapeFunction != waveshapeFunctionCurrent)
        setFunctionToUse(waveshapeFunction);
    
    auto& preGain = processorChain.get<preGainIndex>();
    preGain.setGainDecibels(preGainVal);
    
    auto& postGain = processorChain.get<postGainIndex>();
    postGain.setGainDecibels(postGainVal);
    
    juce::dsp::AudioBlock<float> block (buffer);
    processorChain.process(juce::dsp::ProcessContextReplacing<float>(block));
}

//==============================================================================
bool WaveshaperBasicAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* WaveshaperBasicAudioProcessor::createEditor()
{
    return new WaveshaperBasicAudioProcessorEditor (*this);
}

//==============================================================================
void WaveshaperBasicAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void WaveshaperBasicAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

juce::AudioProcessorValueTreeState::ParameterLayout WaveshaperBasicAudioProcessor::createParameters()
{
    juce::AudioProcessorValueTreeState::ParameterLayout params;
    
    params.add(std::make_unique<juce::AudioParameterFloat>("PREGAIN", "PreGain", -96.0f, 48.0f, 0.0f));
    params.add(std::make_unique<juce::AudioParameterFloat>("POSTGAIN", "PostGain", -96.0f, 48.0f, 0.0f));

    params.add(std::make_unique<juce::AudioParameterChoice>("TYPE", "Type",
                                                            juce::StringArray {"Tanh", "Hardclip", "x/abs(x)+1", "Atan", "HalfRect",
                                                                                "Amp1"},
                                                            1));
    return params;
}

void WaveshaperBasicAudioProcessor::setFunctionToUse(std::string func)
{
    auto &waveshaper = processorChain.get<waveshaperIndex>();
    
    if(func == "Tanh")
    {
        waveshaper.functionToUse = [](float x)
        {
            return std::tanh (x);
        };
        waveshapeFunctionCurrent = "Tanh";
    }
    else if(func == "x/abs(x)+1")
    {
        waveshaper.functionToUse = [](float x)
        {
            return x / (std::abs(x) + 1);
        };
        waveshapeFunctionCurrent = "x/abs(x)+1";
    }
    else if(func == "Hardclip")
    {
        waveshaper.functionToUse = [](float x)
        {
            return juce::jlimit(float (-0.1), float (0.1), x);
        };
        waveshapeFunctionCurrent = "Hardclip";
    }
    else if(func == "Atan")
    {
        waveshaper.functionToUse = [](float x)
        {
            return std::atan(x);
        };
        waveshapeFunctionCurrent = "Atan";
    }
    else if(func == "HalfRect")
    {
        waveshaper.functionToUse = [](float x)
        {
            if(x < 0.0f)
                return 0.0f;
            else
                return x;
        };
        waveshapeFunctionCurrent = "HalfRect";
    }
    else if(func == "Amp1")
    {
        waveshaper.functionToUse = [](float x)
        {
            float param = 0.9f;
            return (x * (std::abs(x) + param) / (x * x + (param - 1.0f) * std::abs(x) + 1.0f)) * 0.7f;
        };
        waveshapeFunctionCurrent = "Amp1";
    }
    return;
}


//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new WaveshaperBasicAudioProcessor();
}
