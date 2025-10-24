

#include "PlayerGUI.h"

// نستخدم الـ Constructor بتاعك لأنه الأفضل، فهو يستقبل محرك الصوت ليربطه بالواجهة
PlayerGUI::PlayerGUI(PlayerAudio& player) : playerAudio(player)
{
    // نجمع كل الأزرار (بتاعتك وبتاعة زميلك) في مصفوفة واحدة لتنظيم الكود
    juce::Button* allButtons[] = {
        &loadButton, &playButton, &pauseButton, &stopButton,
        &restartButton, &startButton, &endButton, &loopButton, &muteButton
    };

    // نمر على كل الأزرار مرة واحدة لإضافتها للشاشة وتفعيل الاستجابة للضغطات
    for (auto* button : allButtons)
    {
        addAndMakeVisible(button);
        button->addListener(this);
    }

    // إعداد سلايدر التحكم في مستوى الصوت
    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue(0.5); // قيمة ابتدائية
    volumeSlider.addListener(this);
    addAndMakeVisible(volumeSlider);
}

PlayerGUI::~PlayerGUI()
{
    // الـ Destructor مش محتاج يعمل حاجة حاليًا
}

// ==============================================================================
// دوال الواجهة الرسومية (Drawing and Layout)
// ==============================================================================

void PlayerGUI::paint(juce::Graphics& g)
{
    // لا نحتاج لرسم أي شيء هنا لأن الخلفية مرسومة في الـ MainComponent
}

void PlayerGUI::resized()
{
    // ننظم الأزرار في صفين لجعل الواجهة أنظف وأوضح
    const int buttonWidth = 80;
    const int buttonHeight = 35;
    const int gap = 10;
    int x = gap;
    int y = gap;

    // الصف الأول من الأزرار
    loadButton.setBounds(x, y, 100, buttonHeight); // زر التحميل أكبر قليلًا
    x += loadButton.getWidth() + gap;
    playButton.setBounds(x, y, buttonWidth, buttonHeight);
    x += buttonWidth + gap;
    pauseButton.setBounds(x, y, buttonWidth, buttonHeight);
    x += buttonWidth + gap;
    stopButton.setBounds(x, y, buttonWidth, buttonHeight);

    // الصف الثاني من الأزرار
    x = gap; // نبدأ من الأول
    y += buttonHeight + gap; // ننزل للصف التالي
    restartButton.setBounds(x, y, buttonWidth, buttonHeight);
    x += buttonWidth + gap;
    startButton.setBounds(x, y, buttonWidth, buttonHeight);
    x += buttonWidth + gap;
    endButton.setBounds(x, y, buttonWidth, buttonHeight);
    x += buttonWidth + gap;
    loopButton.setBounds(x, y, buttonWidth, buttonHeight); // زر اللوب بتاعك
    x += buttonWidth + gap;
    muteButton.setBounds(x, y, buttonWidth, buttonHeight); // <-- حط الزرار الجديد

    // سلايدر الصوت في الأسفل
    volumeSlider.setBounds(gap, y + buttonHeight + gap, getWidth() - (gap * 2), 30);
}

// ==============================================================================
// دوال الاستجابة لأفعال المستخدم (Event Handlers)
// ==============================================================================

void PlayerGUI::buttonClicked(juce::Button* button)
{
    // --- زر تحميل الملفات (كود موحد) ---
    if (button == &loadButton)
    {
        fileChooser = std::make_unique<juce::FileChooser>(
            "Select an audio file...",
            juce::File{},
            "*.wav;*.mp3");

        fileChooser->launchAsync(
            juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
            [this](const juce::FileChooser& fc)
            {
                auto file = fc.getResult();
                if (file.existsAsFile())
                {
                    playerAudio.loadFile(file);
                }
            });
    }
    // --- أزرار التحكم في التشغيل (من شغل زميلك) ---
    else if (button == &playButton) { playerAudio.play(); }
    else if (button == &pauseButton) { playerAudio.pause(); }
    else if (button == &stopButton) { playerAudio.stop(); }
    else if (button == &restartButton) { playerAudio.restart(); }
    else if (button == &startButton) { playerAudio.goToStart(); }
    else if (button == &endButton) { playerAudio.goToEnd(); }
    // --- زر اللوب (من شغلك) ---
    else if (button == &loopButton)
    {
        // نرسل حالة الزر (مضغوط أم لا) لمحرك الصوت لتفعيل أو إلغاء التكرار
        playerAudio.setLooping(loopButton.getToggleState());
    }
    else if (button == &muteButton)
    {
        playerAudio.toggleMute();
    }
}

void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider)
    {
        playerAudio.setGain((float)slider->getValue());
    }
}
