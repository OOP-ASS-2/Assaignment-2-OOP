
#include "PlayerGUI.h"

PlayerGUI::PlayerGUI()
{
    for (auto* btn : { &loadButton, &playButton, &pauseButton, &stopButton, &restartButton, &muteButton })
    {
        addAndMakeVisible(btn);
        btn->addListener(this);
    }
    addAndMakeVisible(waveformDisplay); // << أضف هذا السطر
    addAndMakeVisible(loopButton);
    loopButton.addListener(this);

    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue(0.5);
    volumeSlider.addListener(this);
    addAndMakeVisible(volumeSlider);

    // إعداد سلايدر السرعة
    addAndMakeVisible(speedSlider);
    speedSlider.setRange(0.5, 2.0, 0.01); // سرعة من 0.5x إلى 2.0x
    speedSlider.setValue(1.0); // القيمة الافتراضية
    speedSlider.addListener(this);

    // إعداد العنوان الصغير بجانب السلايدر
    addAndMakeVisible(speedLabel);
    speedLabel.attachToComponent(&speedSlider, true); // اربطه بالسلايدر

}

PlayerGUI::~PlayerGUI() {}

void PlayerGUI::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    playerAudio.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerGUI::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    playerAudio.getNextAudioBlock(bufferToFill);
}

void PlayerGUI::releaseResources()
{
    playerAudio.releaseResources();
}

void PlayerGUI::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
}

void PlayerGUI::resized()
{
    const int buttonWidth = 80;
    const int buttonHeight = 35;
    const int gap = 10;

    int x = gap;
    int y = gap;

    waveformDisplay.setBounds(gap, y, getWidth() - (gap * 2), 100); // << أضف هذا السطر
    y += 100 + gap; // انزل تحت شاشة الموجة

    loadButton.setBounds(x, y, 80, buttonHeight);
    x += 90;
    playButton.setBounds(x, y, 80, buttonHeight);
    x += 90;
    pauseButton.setBounds(x, y, 80, buttonHeight);
    x += 90;
    stopButton.setBounds(x, y, 80, buttonHeight);
    x += 90;
    restartButton.setBounds(x, y, 80, buttonHeight);
    x += 90;
    loopButton.setBounds(x, y, 60, buttonHeight);
    x += 80;
    muteButton.setBounds(x, y, 80, buttonHeight);

    y += buttonHeight + 10;
    volumeSlider.setBounds(gap, y, getWidth() - gap * 2, 30);

    y += 40; // انزل مسافة جديدة
    speedSlider.setBounds(gap + 50, y, getWidth() - gap * 2 - 50, 30); // << تعديل
}

void PlayerGUI::buttonClicked(juce::Button* button)
{
    if (button == &loadButton)
    {
        fileChooser = std::make_unique<juce::FileChooser>("Select an audio file...", juce::File{}, "*.wav;*.mp3");
        fileChooser->launchAsync(juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
            [this](const juce::FileChooser& fc)
            {
                auto file = fc.getResult();
                if (file.existsAsFile())
                {
                    playerAudio.loadFile(file);
                    juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::InfoIcon,
                        "File Info", playerAudio.getMetadata());
                }
            });
    }
    else if (button == &playButton) playerAudio.play();
    else if (button == &pauseButton) playerAudio.pause();
    else if (button == &stopButton) playerAudio.stop();
    else if (button == &restartButton) playerAudio.restart();
    else if (button == &loopButton) playerAudio.setLooping(loopButton.getToggleState());
    else if (button == &muteButton) playerAudio.toggleMute();
}

void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider)
    {
        playerAudio.setGain((float)slider->getValue());
    }
    else if (slider == &speedSlider) // << أضف هذا الشرط
    {
        playerAudio.setSpeed(slider->getValue());
    }
}
