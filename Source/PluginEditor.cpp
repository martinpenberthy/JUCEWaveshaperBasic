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
    sliderPreGain.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 25);
    sliderPreGain.setDoubleClickReturnValue(true, 0.0f);
    sliderPreGain.setSkewFactor(5.0f);
    
    labelPreGain.setText("Pre (dB)", juce::dontSendNotification);
    sliderPreGain.setLookAndFeel(&lookAndFeel);
    labelPreGain.setLookAndFeel(&lookAndFeel);
    

    sliderPostGain.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    sliderPostGain.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 25);
    sliderPostGain.setDoubleClickReturnValue(true, 0.0f);
    sliderPostGain.setSkewFactor(0.5f);
    
    
    
    labelPostGain.setText("Post (dB)", juce::dontSendNotification);
    sliderPostGain.setLookAndFeel(&lookAndFeel);
    labelPostGain.setLookAndFeel(&lookAndFeel);
    
    labelWaveshapeType.attachToComponent(&waveshapeType, false);
    labelWaveshapeType.setColour(juce::Label::textColourId, juce::Colours::white);
    waveshapeType.addItem("Fat", 1);
    waveshapeType.addItem("Tight", 2);
    waveshapeType.addItem("Classic", 3);
    waveshapeType.addItem("Clear", 4);
    waveshapeType.addItem("HardClip", 5);
    waveshapeType.addItem("Sauce", 6);
    waveshapeType.onChange = [this]{modeMenuChanged();};
    waveshapeType.setJustificationType(juce::Justification::centred);
    //waveshapeType.setSelectedId(1);
    //waveshapeType.setColour(juce::ComboBox::ColourIds::backgroundColourId, juce::Colours::black.withAlpha(0.5f));
    //waveshapeType.setColour(juce::PopupMenu::backgroundColourId, juce::Colours::black.withAlpha(0.5f));
    waveshapeType.setLookAndFeel(&lookAndFeel);
    
    
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
    juce::ColourGradient bgGradient = juce::ColourGradient(juce::Colours::navy, 0, 0, juce::Colours::whitesmoke, getWidth(), getHeight(), false);
    //bgGradient.multiplyOpacity(0.8);
    g.setGradientFill(bgGradient);
    g.fillRect (area);
    
    backgroundImage = juce::ImageCache::getFromMemory(BinaryData::_Logos_Final2_alpha_png, BinaryData::_Logos_Final2_alpha_pngSize);
    g.drawImageWithin(backgroundImage, 0, -100, getWidth(), getHeight(), juce::RectanglePlacement::stretchToFit);
}

void WaveshaperBasicAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    int knobSize = 150;
    int width = getWidth();
    
    sliderPreGain.setBounds((width / 3) - 115, (getHeight() / 2) - (knobSize / 2) + 40, knobSize, knobSize);
    labelPreGain.setBounds(sliderPreGain.getX() + 50, sliderPreGain.getY() - 10, 100, 25);
    
    sliderPostGain.setBounds(((width / 3) * 2) - 29, (getHeight() / 2) - (knobSize / 2) + 40, knobSize, knobSize);
    labelPostGain.setBounds(sliderPostGain.getX() + 50, sliderPostGain.getY() - 10, 100, 25);

    
    waveshapeType.setBounds((getWidth()/2) - 40, 60, 100, 25);
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
    
