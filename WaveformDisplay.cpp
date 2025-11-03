#include "WaveformDisplay.h"

WaveformDisplay::WaveformDisplay(PlayerAudio& audioPlayer) : playerAudio(audioPlayer)
{
    // ابدأ الـ Timer ليقوم بتحديث الشاشة 30 مرة في الثانية
    startTimerHz(30);
}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint(juce::Graphics& g)
{
    // 1. تلوين الخلفية
    g.fillAll(juce::Colours::darkgrey.darker()); // خلفية داكنة
    g.setColour(juce::Colours::grey);
    g.drawRect(getLocalBounds(), 1.0f); // إطار بسيط

    // 2. رسم الموجة
    auto& thumbnail = playerAudio.getThumbnail(); // الحصول على الموجة من محرك الصوت
    if (thumbnail.getNumChannels() > 0)
    {
        g.setColour(juce::Colours::cornflowerblue); // لون الموجة
        // ارسم الموجة لتملأ المكون
        thumbnail.drawChannels(g,
            getLocalBounds().reduced(2), // اترك هامش بسيط
            0.0, // زمن البداية
            thumbnail.getTotalLength(), // زمن النهاية
            1.0f); // عامل التكبير
    }
    else
    {
        // إذا لم يتم تحميل ملف
        g.setColour(juce::Colours::lightgrey);
        g.drawFittedText("Load a file to see the waveform", getLocalBounds(), juce::Justification::centred, 1);
    }

    // 3. رسم مؤشر التشغيل
    drawPlaybackPosition(g);
}

void WaveformDisplay::resized()
{
    // لا نحتاج لشيء هنا لأننا نرسم فقط
}

void WaveformDisplay::timerCallback()
{
    // هذه الدالة تجبر المكون على إعادة رسم نفسه
    // هذا ضروري لتحريك "مؤشر التشغيل"
    repaint();
}

void WaveformDisplay::drawPlaybackPosition(juce::Graphics& g)
{
    auto duration = playerAudio.getTransportSource()->getLengthInSeconds();
    if (duration > 0.0)
    {
        // احصل على مكان التشغيل الحالي
        auto currentPos = playerAudio.getTransportSource()->getCurrentPosition();

        // حول مكان التشغيل (بالثواني) إلى مكان على الشاشة (بالبكسل)
        auto drawX = (float)(currentPos / duration) * (float)getWidth();

        // ارسم خط أحمر رفيع
        g.setColour(juce::Colours::red);
        g.drawLine(drawX, 0.0f, drawX, (float)getHeight(), 2.0f);
    }
}
