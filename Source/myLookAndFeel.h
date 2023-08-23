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
