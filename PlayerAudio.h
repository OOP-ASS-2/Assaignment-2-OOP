
#pragma once
#include <JuceHeader.h>

class PlayerAudio
{
public:
    PlayerAudio();
    ~PlayerAudio();

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
    void releaseResources();

    bool loadFile(const juce::File& file);
    void play();
    void pause();
    void stop();
    void restart();
    void setGain(float newGain);
    void setLooping(bool shouldLoop);
    void toggleMute();
    void setSpeed(double newSpeed);
    juce::String getMetadata() const;
    // دوال جديدة خاصة بالـ Waveform
    juce::AudioThumbnail& getThumbnail();
    juce::AudioTransportSource* getTransportSource();
    
private:
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    juce::ResamplingAudioSource resamplingSource{ &transportSource, false }; // << تعديل
    // -- مكونات الـ Waveform --
    juce::AudioThumbnailCache thumbnailCache{ 5 };
    juce::AudioThumbnail thumbnail{ 512, formatManager, thumbnailCache };
    bool isMuted = false;
    float lastKnownVolume = 0.5f;
};
