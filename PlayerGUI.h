

#pragma once

#include <JuceHeader.h>
#include "PlayerAudio.h" // مهم لكي تعرف الواجهة بوجود محرك الصوت

class PlayerGUI : public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener
{
public:
    // نستخدم هذا الـ Constructor لأنه يربط الواجهة بالمحرك الصوتي بطريقة صحيحة
    PlayerGUI(PlayerAudio& player);
    ~PlayerGUI() override;

    // --- دوال الواجهة الرسومية ---
    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    // --- دوال الاستجابة لأفعال المستخدم ---
    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;

    // مرجع إلى محرك الصوت للتحكم فيه (من تصميمك الصحيح)
    PlayerAudio& playerAudio;

    // --- مكونات الواجهة الرسومية (مدمجة من الكودين) ---
    juce::TextButton loadButton{ "Load" };
    juce::TextButton playButton{ "Play" };
    juce::TextButton pauseButton{ "Pause" };
    juce::TextButton stopButton{ "Stop" };
    juce::TextButton restartButton{ "Restart" };
    juce::TextButton startButton{ "Start" }; // Go to Start
    juce::TextButton endButton{ "End" }; // Go to End
    juce::ToggleButton loopButton{ "Loop" };   // الزر الخاص بك
    juce::TextButton muteButton{ "Mute" };

    juce::Slider volumeSlider;

    std::unique_ptr<juce::FileChooser> fileChooser;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)
};
