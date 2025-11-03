

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    addAndMakeVisible(player1);
    addAndMakeVisible(player2);

    addAndMakeVisible(masterVolumeSlider);
    masterVolumeSlider.setRange(0.0, 1.0, 0.01);
    masterVolumeSlider.setValue(masterGain); // << تعديل: استخدمنا المتغير بتاعنا
    masterVolumeSlider.addListener(this);

    setSize(900, 600);
    setAudioChannels(0, 2);

    mixerSource.addInputSource(player1.getOutputAudioSource(), false);
    mixerSource.addInputSource(player2.getOutputAudioSource(), false);

    // << تم مسح السطر الغلط mixerSource.setGain(0.75f); من هنا >>
}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    // 1. اطلب من الميكسر يملى البافر بالصوت المدموج
    mixerSource.getNextAudioBlock(bufferToFill);

    // 2. << إضافة: طبق الصوت الرئيسي على البافر ده >>
    bufferToFill.buffer->applyGain(masterGain);
}

void MainComponent::releaseResources()
{
    mixerSource.releaseResources();
}

//==============================================================================
void MainComponent::paint(juce::Graphics& g)
{
    // ... (الكود زي ما هو) ...
    g.fillAll(juce::Colour(20, 60, 60));
    g.setColour(juce::Colours::yellow);
    g.setFont(juce::Font(34.0f, juce::Font::bold));
    g.drawFittedText("Audio Player", getLocalBounds().removeFromTop(60), juce::Justification::centred, 1);
    g.setColour(juce::Colours::darkgrey);
    g.drawLine(40.0f, 70.0f, getWidth() - 40.0f, 70.0f, 2.0f);
}

void MainComponent::resized()
{
    // ... (الكود زي ما هو) ...
    auto area = getLocalBounds().reduced(20);
    area.removeFromTop(100);
    auto playersArea = area.removeFromTop(area.getHeight() - 50);
    auto masterSliderArea = area;
    auto topHalf = playersArea.removeFromTop(playersArea.getHeight() / 2);
    auto bottomHalf = playersArea;
    topHalf.reduce(0, 10);
    bottomHalf.reduce(0, 10);
    player1.setBounds(topHalf);
    player2.setBounds(bottomHalf);
    masterVolumeSlider.setBounds(masterSliderArea.reduced(getWidth() / 4, 0));
}

void MainComponent::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &masterVolumeSlider)
    {
        // << تعديل: حدث المتغير بتاعنا بدل ما نكلم الميكسر >>
        masterGain = (float)slider->getValue();
    }
}
