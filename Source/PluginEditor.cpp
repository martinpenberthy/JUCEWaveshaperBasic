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
    setSize (400, 250);
    
    addAndMakeVisible(sliderPreGain);
    addAndMakeVisible(labelPreGain);
    
    addAndMakeVisible(sliderPostGain);
    addAndMakeVisible(labelPostGain);
    
    addAndMakeVisible(waveshapeType);

    
    sliderPreGain.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    sliderPreGain.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 76, 38);
    sliderPreGain.setDoubleClickReturnValue(true, 0.0f);
    sliderPreGain.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::black.withAlpha(0.0f));
    sliderPreGain.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colours::green.withAlpha(0.75f));
    sliderPreGain.setColour(juce::Slider::ColourIds::rotarySliderOutlineColourId, juce::Colours::black.withAlpha(0.75f));
    sliderPreGain.setColour(juce::Slider::ColourIds::textBoxTextColourId, juce::Colours::whitesmoke.withAlpha(0.25f));
    sliderPreGain.setColour(juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colours::black.withAlpha(0.0f));
    //labelPreGain.attachToComponent(&sliderPreGain, false);
    labelPreGain.setText("PreGain(dB)", juce::dontSendNotification);
    
   /* sliderPreGain.onValueChange = [this]()
    {
        //audioProcessor.volume.setTargetValue(sliderGain.getValue());
        audioProcessor.preGainVal = sliderPreGain.getValue();
    };*/
    
    sliderPostGain.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    sliderPostGain.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 76, 38);
    sliderPostGain.setDoubleClickReturnValue(true, 0.0f);
    sliderPostGain.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::black.withAlpha(0.0f));
    sliderPostGain.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colours::green.withAlpha(0.75f));
    sliderPostGain.setColour(juce::Slider::ColourIds::rotarySliderOutlineColourId, juce::Colours::black.withAlpha(0.7   5f));
    sliderPostGain.setColour(juce::Slider::ColourIds::textBoxTextColourId, juce::Colours::whitesmoke.withAlpha(0.25f));
    sliderPostGain.setColour(juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colours::black.withAlpha(0.0f));
    //labelPostGain.attachToComponent(&sliderPostGain, false);
    labelPostGain.setText("PostGain(dB)", juce::dontSendNotification);
    
    /*sliderPostGain.onValueChange = [this]()
    {
        //audioProcessor.volume.setTargetValue(sliderGain.getValue());
        audioProcessor.postGainVal = sliderPostGain.getValue();
    };*/
    
    
    labelWaveshapeType.attachToComponent(&waveshapeType, false);
    labelWaveshapeType.setColour(juce::Label::textColourId, juce::Colours::white);
    waveshapeType.addItem("Tanh", 1);
    waveshapeType.addItem("myAmp", 2);
    waveshapeType.addItem("x/abs(x)+1", 3);
    waveshapeType.addItem("Atan", 4);
    waveshapeType.addItem("HalfRect", 5);
    waveshapeType.addItem("Amp1", 6);
    waveshapeType.onChange = [this]{modeMenuChanged();};
    waveshapeType.setSelectedId(1);

    
    
    sliderAttachmentPreGain = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "PREGAIN", sliderPreGain);
    
    sliderAttachmentPostGain = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "POSTGAIN", sliderPostGain);
    
    comboAttachmentWaveshapeType = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "TYPE", waveshapeType);
    
}

WaveshaperBasicAudioProcessorEditor::~WaveshaperBasicAudioProcessorEditor()
{
}

//==============================================================================
void WaveshaperBasicAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll (juce::Colour::fromRGB(15, 16, 59));
    //g.fillAll(juce::Colour::fromRGB(30, 10, 70));

    juce::Rectangle<int> area (0, 0, getWidth(), getHeight());
    juce::ColourGradient bgGradient =juce::ColourGradient(juce::Colours::navy, 0, 0, juce::Colours::whitesmoke, getWidth(), getHeight(), false);
    g.setGradientFill(bgGradient);
    g.fillRect (area);
}

void WaveshaperBasicAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    int knobSize = 150;
    int width = getWidth();
    
    sliderPreGain.setBounds((width / 3) - 115, (getHeight() / 2) - (knobSize / 2) + 15, knobSize, knobSize);
    labelPreGain.setBounds(sliderPreGain.getX() + 30, sliderPreGain.getY() - 10, 100, 25);
    
    sliderPostGain.setBounds(((width / 3) * 2), (getHeight() / 2) - (knobSize / 2) + 15, knobSize, knobSize);
    labelPostGain.setBounds(sliderPostGain.getX() + 30, sliderPostGain.getY() - 10, 100, 25);

    
    waveshapeType.setBounds((getWidth()/2) - 50, 20, 100, 25);
}

void WaveshaperBasicAudioProcessorEditor::modeMenuChanged()
{
    switch (waveshapeType.getSelectedId())
    {
        case 1:
            audioProcessor.waveshapeFunction = "Tanh";
            break;
        case 2:
            audioProcessor.waveshapeFunction = "myAmp";
            break;
        case 3:
            audioProcessor.waveshapeFunction = "x/abs(x)+1";
            break;
        case 4:
            audioProcessor.waveshapeFunction = "Atan";
            break;
        case 5:
            audioProcessor.waveshapeFunction = "HalfRect";
            break;
        case 6:
            audioProcessor.waveshapeFunction = "Amp1";
            break;
        default:
            audioProcessor.waveshapeFunction = "Tanh";
            break;
    }
}
    
