/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
WaveshaperBasicAudioProcessorEditor::WaveshaperBasicAudioProcessorEditor (WaveshaperBasicAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    
    addAndMakeVisible(sliderPreGain);
    addAndMakeVisible(sliderPostGain);

    
    sliderPreGain.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    sliderPreGain.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 76, 38);
    sliderPreGain.setDoubleClickReturnValue(true, 0.0f);
    sliderPreGain.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::black.withAlpha(0.0f));
    sliderPreGain.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colours::whitesmoke.withAlpha(0.25f));
    sliderPreGain.setColour(juce::Slider::ColourIds::rotarySliderOutlineColourId, juce::Colours::black.withAlpha(0.25f));
    sliderPreGain.setColour(juce::Slider::ColourIds::textBoxTextColourId, juce::Colours::whitesmoke.withAlpha(0.25f));
    sliderPreGain.setColour(juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colours::black.withAlpha(0.0f));
    labelPreGain.attachToComponent(&sliderPreGain, false);
    labelPreGain.setText("PreGain(dB)", juce::dontSendNotification);
    
    sliderPreGain.onValueChange = [this]()
    {
        //audioProcessor.volume.setTargetValue(sliderGain.getValue());
        audioProcessor.preGainVal = sliderPreGain.getValue();
    };
    
    sliderPostGain.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    sliderPostGain.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 76, 38);
    sliderPostGain.setDoubleClickReturnValue(true, 0.0f);
    sliderPostGain.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::black.withAlpha(0.0f));
    sliderPostGain.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colours::whitesmoke.withAlpha(0.25f));
    sliderPostGain.setColour(juce::Slider::ColourIds::rotarySliderOutlineColourId, juce::Colours::black.withAlpha(0.25f));
    sliderPostGain.setColour(juce::Slider::ColourIds::textBoxTextColourId, juce::Colours::whitesmoke.withAlpha(0.25f));
    sliderPostGain.setColour(juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colours::black.withAlpha(0.0f));
    labelPostGain.attachToComponent(&sliderPostGain, false);
    labelPostGain.setText("PostGain(dB)", juce::dontSendNotification);
    
    sliderPostGain.onValueChange = [this]()
    {
        //audioProcessor.volume.setTargetValue(sliderGain.getValue());
        audioProcessor.postGainVal = sliderPostGain.getValue();
    };
    
    
    
    
   sliderAttachmentPreGain = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "PREGAIN", sliderPreGain);
    
    sliderAttachmentPostGain = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "POSTGAIN", sliderPostGain);
    
}

WaveshaperBasicAudioProcessorEditor::~WaveshaperBasicAudioProcessorEditor()
{
}

//==============================================================================
void WaveshaperBasicAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    
    sliderPreGain.setBounds(getWidth()/2 - 100, getHeight()/2 - 100, 150, 150);
    sliderPostGain.setBounds(getWidth()/2 + 50, getHeight()/2 - 100, 150, 150);
}

void WaveshaperBasicAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
