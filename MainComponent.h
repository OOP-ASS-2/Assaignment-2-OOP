

#pragma once
#include <JuceHeader.h>
#include "PlayerGUI.h"

//==============================================================================
class MainComponent : public juce::AudioAppComponent,
    public juce::Slider::Listener
{
public:
    MainComponent();
    ~MainComponent() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void paint(juce::Graphics& g) override;
    void resized() override;
    void sliderValueChanged(juce::Slider* slider) override;

private:
    PlayerGUI player1;
    PlayerGUI player2;
    juce::MixerAudioSource mixerSource;

    juce::Slider masterVolumeSlider;

    // << إضافة: متغير لتخزين قيمة الصوت >>
    float masterGain = 0.75f; // << 1. أضف هذا السطر

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
