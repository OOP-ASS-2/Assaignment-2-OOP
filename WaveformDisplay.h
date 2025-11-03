#pragma once

#include <JuceHeader.h>
#include "PlayerAudio.h" // نحتاج هذا للوصول إلى PlayerAudio

class WaveformDisplay : public juce::Component,
    public juce::Timer
{
public:
    // نمرر له محرك الصوت بالكامل
    WaveformDisplay(PlayerAudio& audioPlayer);
    ~WaveformDisplay() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

    // هذه الدالة سيتم استدعاؤها بشكل متكرر لتحديث الرسم
    void timerCallback() override;

private:
    PlayerAudio& playerAudio; // مرجع لمحرك الصوت

    // دالة لرسم مؤشر التشغيل
    void drawPlaybackPosition(juce::Graphics& g);
};
