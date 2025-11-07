#pragma once

#include <JuceHeader.h>
#include "PlayerAudio.h" 

class WaveformDisplay : public juce::Component,
    public juce::Timer
{
public:

    WaveformDisplay(PlayerAudio& audioPlayer);
    ~WaveformDisplay() override;

    void paint(juce::Graphics& g) override;
    void resized() override;


    void timerCallback() override;


    void loadURL(const juce::File& file) { playerAudio.getThumbnail().setSource(new juce::FileInputSource(file)); }


private:
    PlayerAudio& playerAudio;


    void drawPlaybackPosition(juce::Graphics& g);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveformDisplay)
};
