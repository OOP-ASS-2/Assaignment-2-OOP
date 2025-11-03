
#pragma once
#include <JuceHeader.h>
#include "PlayerAudio.h"
#include "WaveformDisplay.h"
class PlayerGUI : public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener
{
public:
    PlayerGUI();
    ~PlayerGUI() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
    void releaseResources();

    // ... (داخل public:)
    juce::AudioTransportSource* getTransportSource() { return playerAudio.getTransportSource(); }


private:
    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;

    PlayerAudio playerAudio;
    WaveformDisplay waveformDisplay{ playerAudio }; // << أضف هذا السطر

    juce::TextButton loadButton{ "Load" };
    juce::TextButton playButton{ "Play" };
    juce::TextButton pauseButton{ "Pause" };
    juce::TextButton stopButton{ "Stop" };
    juce::TextButton restartButton{ "Restart" };
    juce::ToggleButton loopButton{ "Loop" };
    juce::TextButton muteButton{ "Mute" };

    juce::Slider volumeSlider; // << تعديل
    juce::Slider speedSlider; // << 
    juce::Label speedLabel{ "SpeedLabel", "Speed:" }; // << تعديل
    std::unique_ptr<juce::FileChooser> fileChooser;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)
};
