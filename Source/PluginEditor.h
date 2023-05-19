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
class WaveshaperBasicAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    WaveshaperBasicAudioProcessorEditor (WaveshaperBasicAudioProcessor&);
    ~WaveshaperBasicAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void modeMenuChanged();


private:
    juce::Slider sliderPreGain;//Pregain slider
    juce::Label labelPreGain; //Pregain label
    
    juce::Slider sliderPostGain;//Pregain slider
    juce::Label labelPostGain; //Pregain label
    
    juce::Label labelWaveshapeType;
    juce::ComboBox waveshapeType;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderAttachmentPreGain;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderAttachmentPostGain;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> comboAttachmentWaveshapeType;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    WaveshaperBasicAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveshaperBasicAudioProcessorEditor)
};
