
#include "PlayerAudio.h"

PlayerAudio::PlayerAudio()
{
    formatManager.registerBasicFormats();
}

PlayerAudio::~PlayerAudio()
{
    transportSource.setSource(nullptr);
}

void PlayerAudio::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resamplingSource.prepareToPlay(samplesPerBlockExpected, sampleRate); // << أضف هذا السطر
}

void PlayerAudio::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    resamplingSource.getNextAudioBlock(bufferToFill); // << تعديل
}

void PlayerAudio::releaseResources()
{
    transportSource.releaseResources();
    resamplingSource.releaseResources(); // << تعديل
}

void PlayerAudio::setSpeed(double newSpeed)
{
    // هذه الدالة تتحكم في نسبة سرعة الصوت
    resamplingSource.setResamplingRatio(newSpeed);
}

bool PlayerAudio::loadFile(const juce::File& file)
{
    if (file.existsAsFile())
    {
        if (auto* reader = formatManager.createReaderFor(file))
        {
            transportSource.stop();
            transportSource.setSource(nullptr);
            readerSource.reset(new juce::AudioFormatReaderSource(reader, true));
            transportSource.setSource(readerSource.get(), 0, nullptr, reader->sampleRate);

            // << أضف هذا السطر >>
            // اطلب من الـ thumbnail أن يقرأ الملف
            thumbnail.setSource(new juce::FileInputSource(file));

            return true;
        }
    }
    return false;
}

void PlayerAudio::play() { transportSource.start(); }
void PlayerAudio::pause() { transportSource.stop(); }
void PlayerAudio::stop() { transportSource.stop(); transportSource.setPosition(0.0); }
void PlayerAudio::restart() { transportSource.setPosition(0.0); transportSource.start(); }
void PlayerAudio::setGain(float newGain) { transportSource.setGain(newGain); }
void PlayerAudio::setLooping(bool shouldLoop)
{
    if (readerSource.get() != nullptr)
        readerSource->setLooping(shouldLoop);
}

void PlayerAudio::toggleMute()
{
    isMuted = !isMuted;
    if (isMuted)
    {
        lastKnownVolume = transportSource.getGain();
        transportSource.setGain(0.0f);
    }
    else
    {
        transportSource.setGain(lastKnownVolume);
    }
}


juce::String PlayerAudio::getMetadata() const
{
    if (readerSource == nullptr)
        return "No file loaded.";

    auto* reader = readerSource->getAudioFormatReader();
    juce::String info;

    // الحل: نستخدم juce::String() لضمان تحويل الأرقام إلى نصوص
    info << "Sample Rate: " << juce::String(reader->sampleRate) << " Hz\n";
    info << "Length: " << juce::String(transportSource.getLengthInSeconds()) << " sec\n";
    info << "Channels: " << juce::String(reader->numChannels) << "\n"; // << تم إصلاح هذا السطر

    return info;
}

juce::AudioThumbnail& PlayerAudio::getThumbnail()
{
    return thumbnail;
}

juce::AudioTransportSource* PlayerAudio::getTransportSource()
{
    return &transportSource;
}
