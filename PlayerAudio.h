

#pragma once
#include <JuceHeader.h>

class PlayerAudio
{
public:
    // Constructor and Destructor
    PlayerAudio();
    ~PlayerAudio(); // مهم لإدارة الذاكرة

    // --- دوال الصوت الأساسية ---
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
    void releaseResources();

    // --- دوال التحكم في التشغيل (مدمجة من الكودين) ---
    void loadFile(const juce::File& file);
    void play();
    void pause();
    void stop();
    void restart();
    void goToStart();
    void goToEnd();
    void setGain(float newGain);

    // الدالة دي من نسختك أنت
    void setPosition(double position);

    // ودي كمان، أهم إضافة بتاعتك (زرار اللوب)
    void setLooping(bool shouldLoop);

    void toggleMute();

private:
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;

    bool isMuted = false;       // متغير عشان نعرف حالة الكتم الحالية
    float lastKnownVolume = 0.5f; // متغير عشان نحفظ آخر مستوى صوت قبل الكتم
};
