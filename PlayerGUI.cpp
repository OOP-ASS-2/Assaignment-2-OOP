

#include "PlayerGUI.h"

PlayerGUI::PlayerGUI()
{
    // === بداية التصليح ===
    // المشكلة: عندنا أنواع أزرار مختلفة (TextButton و ToggleButton)
    // الحل: نجمعهم في مصفوفة من النوع الأب المشترك juce::Button
    juce::Button* allButtons[] = {
        &loadButton, &playButton, &pauseButton, &stopButton, &restartButton, &muteButton,
        &loopButton // << مسحنا reverseButton من هنا
    };

    // دلوقتي نقدر نلف عليهم كلهم مرة واحدة
    for (auto* btn : allButtons)
    {
        addAndMakeVisible(btn);
        btn->addListener(this);
    }
    // === نهاية التصليح ===

    // المكون ده مش زرار، ده شاشة عرض
    addAndMakeVisible(waveformDisplay);

    // إعداد سلايدر الصوت
    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue(0.5);
    volumeSlider.addListener(this);
    addAndMakeVisible(volumeSlider);

    // إعداد سلايدر السرعة
    addAndMakeVisible(speedSlider);
    speedSlider.setRange(0.5, 2.0, 0.01);
    speedSlider.setValue(1.0);
    speedSlider.addListener(this);

    // إعداد العناوين
    addAndMakeVisible(speedLabel);
    addAndMakeVisible(positionLabel);
    speedLabel.attachToComponent(&speedSlider, true);

    // << إضافة: ظبطنا إعدادات سلايدر التقدم >>
    addAndMakeVisible(positionSlider);
    positionSlider.setRange(0.0, 1.0, 0.001); // المدى من 0% إلى 100%
    positionSlider.addListener(this);

    // << إضافة: ظبطنا إعدادات التسمية بتاعة الوقت >>
    positionLabel.setText("00:00 / 00:00", juce::dontSendNotification);
    positionLabel.setJustificationType(juce::Justification::centred);

    // << إضافة: شغلنا المؤقت عشان يحدث السلايدر 30 مرة في الثانية >>
    startTimerHz(30);
}

PlayerGUI::~PlayerGUI()
{
    stopTimer(); // << إضافة: نقفل المؤقت لما البرنامج يقفل
}

// ... (دوال prepareToPlay, getNextAudioBlock, releaseResources زي ما هي) ...
void PlayerGUI::prepareToPlay(int samplesPerBlockExpected, double sampleRate) { playerAudio.prepareToPlay(samplesPerBlockExpected, sampleRate); }
void PlayerGUI::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) { playerAudio.getNextAudioBlock(bufferToFill); }
void PlayerGUI::releaseResources() { playerAudio.releaseResources(); }

// ... (دالة paint زي ما هي) ...
void PlayerGUI::paint(juce::Graphics& g) { g.fillAll(juce::Colours::black); }

// ... (دالة getOutputAudioSource زي ما هي) ...
juce::AudioSource* PlayerGUI::getOutputAudioSource() { return playerAudio.getOutputAudioSource(); }


void PlayerGUI::resized()
{
    const int buttonHeight = 35;
    const int gap = 10;
    int y = gap;

    // 1. شاشة الموجة
    waveformDisplay.setBounds(gap, y, getWidth() - (gap * 2), 100);
    y += 100 + gap;

    // 2. سلايدر التقدم والتسمية بتاعته
    positionSlider.setBounds(gap, y, getWidth() - (gap * 2), 30);
    positionLabel.setBounds(gap, y + 25, getWidth() - (gap * 2), 20); // تحت السلايدر
    y += 40 + gap; // انزل مسافة

    // 3. صف الأزرار
    int x = gap;
    int buttonWidth = 80; // عرض الزر
    loadButton.setBounds(x, y, buttonWidth, buttonHeight); x += buttonWidth + gap;
    playButton.setBounds(x, y, buttonWidth, buttonHeight); x += buttonWidth + gap;
    pauseButton.setBounds(x, y, buttonWidth, buttonHeight); x += buttonWidth + gap;
    stopButton.setBounds(x, y, buttonWidth, buttonHeight); x += buttonWidth + gap;
    restartButton.setBounds(x, y, buttonWidth, buttonHeight); x += buttonWidth + gap;
    loopButton.setBounds(x, y, 70, buttonHeight); x += 70 + gap; // زر أصغر
    muteButton.setBounds(x, y, 70, buttonHeight); x += 70 + gap; // زر أصغر
    // << تم مسح سطر reverseButton من هنا >>
    y += buttonHeight + gap; // انزل سطر

    // 4. سلايدر الصوت
    volumeSlider.setBounds(gap, y, getWidth() - gap * 2, 30);
    y += 40;

    // 5. سلايدر السرعة
    speedSlider.setBounds(gap + 50, y, getWidth() - gap * 2 - 50, 30);
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
                    if (playerAudio.loadFile(file)) // اتأكد إنه اتحمل
                    {
                        playerAudio.play(); // شغل الصوت تلقائيًا
                        juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::InfoIcon,
                            "File Info", playerAudio.getMetadata());
                    }
                }
            });
    }
    // ... (باقي الأزرار زي ما هي) ...
    else if (button == &playButton) playerAudio.play();
    else if (button == &pauseButton) playerAudio.pause();
    else if (button == &stopButton) playerAudio.stop();
    else if (button == &restartButton) playerAudio.restart();
    else if (button == &loopButton) playerAudio.setLooping(loopButton.getToggleState());
    else if (button == &muteButton) playerAudio.toggleMute();
    // << تم مسح الـ else if بتاع reverseButton من هنا >>
}

void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider)
    {
        playerAudio.setGain((float)slider->getValue());
    }
    else if (slider == &speedSlider)
    {
        playerAudio.setSpeed(slider->getValue());
        // << تم مسح السطر بتاع reverseButton من هنا >>
    }
    // << إضافة: خلينا المستخدم يقدر يحرك السلايدر >>
    else if (slider == &positionSlider)
    {
        // اتأكد إن المستخدم هو اللي بيحرك السلايدر (مش المؤقت)
        if (slider->isMouseButtonDown())
        {
            // حول القيمة (من 0 لـ 1) إلى ثواني
            double newPositionInSeconds = slider->getValue() * playerAudio.getTransportSource()->getLengthInSeconds();
            playerAudio.getTransportSource()->setPosition(newPositionInSeconds);
        }
    }
}

// << إضافة: الدالة اللي بتحدث السلايدر كل شوية >>
void PlayerGUI::timerCallback()
{
    // اتأكد إن الصوت شغال
    if (playerAudio.getTransportSource()->isPlaying())
    {
        double currentPos = playerAudio.getTransportSource()->getCurrentPosition();
        double duration = playerAudio.getTransportSource()->getLengthInSeconds();

        if (duration > 0.0)
        {
            // 1. حدث السلايدر
            // (بنستخدم dontSendNotification عشان ده ما يتسببش في استدعاء sliderValueChanged)
            positionSlider.setValue(currentPos / duration, juce::dontSendNotification);

            // 2. حدث التسمية بتاعة الوقت
            auto formatTime = [](double seconds) {
                int mins = (int)(seconds / 60);
                int secs = (int)seconds % 60;
                return juce::String::formatted("%02d:%02d", mins, secs);
                };

            positionLabel.setText(formatTime(currentPos) + " / " + formatTime(duration), juce::dontSendNotification);
        }
    }
}
