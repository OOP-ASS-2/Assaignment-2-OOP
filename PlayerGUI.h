
#pragma once
#include <JuceHeader.h>
#include "PlayerAudio.h"
#include "WaveformDisplay.h"

// << تعديل: خلينا الكلاس "يستمع للوقت" عشان نقدر نحدث السلايدر
class PlayerGUI : public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener,
    public juce::Timer // << أضفنا وراثة المؤقت ده
{
public:
    PlayerGUI();
    ~PlayerGUI() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
    void releaseResources();

    juce::AudioTransportSource* getTransportSource() { return playerAudio.getTransportSource(); }
    juce::AudioSource* getOutputAudioSource();


private:
    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;
    void timerCallback() override; // << إضافة: الإعلان عن دالة المؤقت

    PlayerAudio playerAudio;
    WaveformDisplay waveformDisplay{ playerAudio };

    juce::TextButton loadButton{ "Load" };
    juce::TextButton playButton{ "Play" };
    juce::TextButton pauseButton{ "Pause" };
    juce::TextButton stopButton{ "Stop" };
    juce::TextButton restartButton{ "Restart" };
    juce::ToggleButton loopButton{ "Loop" };
    juce::TextButton muteButton{ "Mute" };
    // << تم مسح زر reverseButton الغلط من هنا >>

    juce::Slider volumeSlider;
    juce::Slider speedSlider;
    juce::Label speedLabel{ "SpeedLabel", "Speed:" };
    std::unique_ptr<juce::FileChooser> fileChooser;
    juce::Slider positionSlider;
    juce::Label positionLabel; // << ده السطر اللي كان ناقص عندك قبل كده

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)
};
