

#include "PlayerAudio.h"

// Constructor: بيشتغل أول ما البرنامج يفتح
PlayerAudio::PlayerAudio()
{
    // بيسجل الصيغ المشهورة زي WAV و MP3 عشان البرنامج يقدر يقرأهم
    formatManager.registerBasicFormats();
}

// Destructor: بيشتغل لما البرنامج بيتقفل عشان ينضف الذاكرة
PlayerAudio::~PlayerAudio()
{
    transportSource.setSource(nullptr);
}

// ==============================================================================
// دوال الصوت الأساسية (تبع JUCE)
// ==============================================================================

void PlayerAudio::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerAudio::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    transportSource.getNextAudioBlock(bufferToFill);
}

void PlayerAudio::releaseResources()
{
    transportSource.releaseResources();
}

// ==============================================================================
// دوال التحكم في التشغيل (الجزء المدموج)
// ==============================================================================

// أنا أخدت نسخة زميلك من دالة loadFile لأنها آمن وأفضل
void PlayerAudio::loadFile(const juce::File& file)
{
    if (file.existsAsFile())
    {
        if (auto* reader = formatManager.createReaderFor(file))
        {
            // بنوقف أي صوت شغال قبل ما نحمل الجديد
            transportSource.stop();
            // بنجهز الملف الجديد
            readerSource.reset(new juce::AudioFormatReaderSource(reader, true));
            transportSource.setSource(readerSource.get(), 0, nullptr, reader->sampleRate);
        }
    }
}

// --- كل دوال التحكم من الكودين ---
void PlayerAudio::play() { transportSource.start(); }
void PlayerAudio::pause() { transportSource.stop(); }
void PlayerAudio::stop() { transportSource.stop(); transportSource.setPosition(0.0); }
void PlayerAudio::restart() { transportSource.setPosition(0.0); transportSource.start(); }
void PlayerAudio::goToStart() { transportSource.setPosition(0.0); }

void PlayerAudio::goToEnd()
{
    if (readerSource)
        transportSource.setPosition(readerSource->getTotalLength() / readerSource->getAudioFormatReader()->sampleRate);
}

void PlayerAudio::setGain(float newGain)
{
    transportSource.setGain(newGain);

    // لو المستخدم غير الصوت، نتأكد إن البرنامج مش في وضع الكتم
    if (newGain > 0.0f)
    {
        isMuted = false;
    }
}

void PlayerAudio::setPosition(double position)
{
    transportSource.setPosition(position);
}

// الإضافة بتاعتك لخاصية اللوب
void PlayerAudio::setLooping(bool shouldLoop)
{
    if (readerSource != nullptr)
    {
        readerSource->setLooping(shouldLoop);
    }
}

void PlayerAudio::toggleMute()
{
    isMuted = !isMuted; // اعكس حالة الكتم (لو true تبقى false والعكس)

    if (isMuted)
    {
        // لو بنعمل كتم دلوقتي
        lastKnownVolume = transportSource.getGain(); // احفظ مستوى الصوت الحالي
        transportSource.setGain(0.0f); // خلي مستوى الصوت صفر (كتم)
    }
    else
    {
        // لو بنلغي الكتم
        transportSource.setGain(lastKnownVolume); // رجّع الصوت لآخر قيمة محفوظة
    }
}
