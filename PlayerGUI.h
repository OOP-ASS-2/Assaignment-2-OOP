#pragma once
#include <JuceHeader.h>
#include "PlayerAudio.h"
#include "WaveformDisplay.h"
#include "Playlist.h" 
#include <cmath> 

class PlayerGUI : public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener,
    public juce::Timer
{
public:
    PlayerGUI();
    ~PlayerGUI() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
    void releaseResources();

    juce::AudioSource* getOutputAudioSource();
    juce::AudioTransportSource* getTransportSource() { return playerAudio.getTransportSource(); }

    void setPlayerGain(float newGain) { playerAudio.setGain(newGain); }

    void setPlaylist(Playlist* playlistPtr);

private:
    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;
    void timerCallback() override;

    PlayerAudio playerAudio;
    WaveformDisplay waveformDisplay{ playerAudio };

    Playlist* playlist = nullptr;

    juce::TextButton loadButton{ "Load" };
    juce::TextButton playButton{ "Play" };
    juce::TextButton pauseButton{ "Pause" };
    juce::TextButton stopButton{ "Stop" };
    juce::TextButton restartButton{ "Restart" };
    juce::TextButton goToEndButton{ "End" };
    juce::ToggleButton loopButton{ "Loop" };
    juce::TextButton muteButton{ "Mute" };
    juce::TextButton seekBackwardButton{ "<< 10s" };
    juce::TextButton seekForwardButton{ ">> 10s" };

    juce::TextButton abLoopStartButton{ "A" };
    juce::TextButton abLoopEndButton{ "B" };
    juce::ToggleButton abLoopToggleButton{ "A-B Loop" };


    double loopStartPoint = -1.0;
    double loopEndPoint = -1.0;


    juce::String formatTime(double seconds)
    {
        int totalSeconds = (int)std::round(seconds);
        int minutes = totalSeconds / 60;
        int remainingSeconds = totalSeconds % 60;
        return juce::String::formatted("%02d:%02d", minutes, remainingSeconds);
    }

    juce::Slider volumeSlider;
    juce::Slider speedSlider;
    juce::Slider positionSlider;

    juce::Label speedLabel{ "Speed: ", "Speed: " };
    juce::Label positionLabel;
    juce::Label metadataLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)
};
