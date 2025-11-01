#include "MainComponent.h"

//==============================================================================
// Constructor
MainComponent::MainComponent()
{
    // إضافة واجهة المشغلين
    addAndMakeVisible(player1);
    addAndMakeVisible(player2);

    // إعداد حجم النافذة
    setSize(900, 600);

    // تفعيل الصوت (0 مداخل - 2 مخارج)
    setAudioChannels(0, 2);
}

// Destructor
MainComponent::~MainComponent()
{
    shutdownAudio();
}

//==============================================================================
// Audio functions
void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    player1.getNextAudioBlock(bufferToFill);
    player2.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    player1.releaseResources();
    player2.releaseResources();
}

//==============================================================================
// UI
void MainComponent::paint(juce::Graphics& g)
{
    // الخلفية العامة
    g.fillAll(juce::Colour(20, 60, 60)); // لون داكن أنيق

    // إعداد الخط واللون للعنوان
    g.setColour(juce::Colours::yellow);
    g.setFont(juce::Font(34.0f, juce::Font::bold));

    // رسم العنوان في الأعلى بالمنتصف
    g.drawFittedText("Audio Player",
        getLocalBounds().removeFromTop(60),
        juce::Justification::centred,
        1);

    // خط فاصل أنيق تحت العنوان
    g.setColour(juce::Colours::darkgrey);
    g.drawLine(40.0f, 70.0f, getWidth() - 40.0f, 70.0f, 2.0f);
}

void MainComponent::resized()
{
    // نسيب شوية فراغ من الحواف
    auto area = getLocalBounds().reduced(20);

    // نسيب مساحة فوق للعنوان
    area.removeFromTop(100);

    // ندي مساحة متساوية للبلايرين
    auto topHalf = area.removeFromTop(area.getHeight() / 2);
    auto bottomHalf = area;

    // نسيب فراغ بسيط بين البلايرين
    topHalf.reduce(0, 10);
    bottomHalf.reduce(0, 10);

    // نوزع البلايرين في النص
    player1.setBounds(topHalf);
    player2.setBounds(bottomHalf);
}
