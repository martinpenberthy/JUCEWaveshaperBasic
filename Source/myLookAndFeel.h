/*
  ==============================================================================

    myLookAndFeel.h
    Created: 21 Aug 2023 4:55:33pm
    Author:  Martin Penberthy

  ==============================================================================
*/

#pragma once
class myLookAndFeel : public juce::LookAndFeel_V4
{
public:
    myLookAndFeel()
    {
        setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::black.withAlpha(1.0f));
        setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colours::white.withAlpha(1.0f));
        setColour(juce::Slider::ColourIds::rotarySliderOutlineColourId, juce::Colours::black.withAlpha(0.75f));
        setColour(juce::Slider::ColourIds::textBoxTextColourId, juce::Colours::black.withAlpha(0.75f));
        setColour(juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colours::black.withAlpha(0.75f));
        
        setColour(juce::PopupMenu::ColourIds::backgroundColourId, juce::Colours::black.withAlpha(0.25f));
        setColour(juce::ComboBox::ColourIds::backgroundColourId, juce::Colours::black.withAlpha(0.25f));
    }
    
    juce::Font getComboBoxFont (juce::ComboBox& /*box*/) override
    {
        return getCommonMenuFont();
    }
    
    juce::Font getPopupMenuFont() override
    {
        return getCommonMenuFont();
    }
    
    juce::Font getLabelFont(juce::Label &) override
    {
        return myFont;
    }
    
    
private:
    juce::Font getCommonMenuFont()
    {
       // return Font ("Wingdings", "Regular", 20.f);
        return myFont;
    }
    
    juce::Font myFont = juce::Font("Helvetica", "Regular", 12.f);
    
};
