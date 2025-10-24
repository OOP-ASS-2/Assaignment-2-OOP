

#include "MainComponent.h"

//==============================================================================
// Constructor: The code that runs when the component is first created
// بالعربي: الكود اللي بيشتغل أول ما الواجهة بتتعمل
MainComponent::MainComponent()
{
    // Make the player GUI component visible
    // بنظهر الواجهة الرسومية للمشغل
    addAndMakeVisible(playerGUI);

    // Set the size of our main window
    // بنحدد حجم النافذة الرئيسية
    setSize(500, 250);

    // We want to output audio, so we set up 2 output channels (stereo) and 0 inputs
    // بنجهز كارت الصوت عشان نطلع صوت (2 مخرجات) ومش هنسجل أي حاجة (0 مدخلات)
    setAudioChannels(0, 2);
}

// Destructor: The code that runs when the component is destroyed (e.g., app closes)
// بالعربي: الكود اللي بيشتغل لما البرنامج بيتقفل
MainComponent::~MainComponent()
{
    // This shuts down the audio device and releases it
    // بنوقف الصوت تمامًا ونحرر كارت الصوت
    shutdownAudio();
}

//==============================================================================
// Audio Callback Functions: These are called by the audio thread
// دوال الصوت: دي بتتنفذ بشكل مستمر عشان معالجة الصوت
//==============================================================================

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    // Pass this information to our playerGUI component, so it can prepare its audio source
    // بنبلغ المشغل بتاعنا بمعدل العينة وحجم البلوك عشان يجهز نفسه لتشغيل الصوت
    playerAudio.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    // The audio device is asking for the next block of audio data.
    // We ask our playerGUI component to fill this buffer with audio.
    // كارت الصوت بيطلب مننا الدفعة التالية من بيانات الصوت، فاحنا بنطلب من المشغل يملأها
    playerAudio.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    // This is called when the audio device stops.
    // We tell our playerGUI component to release any memory or resources it was using.
    // لما الصوت بيقف، بنقول للمشغل يمسح أي حاجة كان حاجزها في الذاكرة
    playerAudio.releaseResources();
}

//==============================================================================
// UI Rendering Functions: These handle drawing the component on the screen
// دوال الواجهة الرسومية: دي مسئولة عن رسم الواجهة على الشاشة
//==============================================================================

void MainComponent::paint(juce::Graphics& g)
{
    // Fill the background with a dark grey color
    // بنلون الخلفية باللون الرمادي الغامق
    g.fillAll(juce::Colours::darkgrey);
}

void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // We want our playerGUI to fill the entire window, so we set its bounds to match.
    // لما حجم النافذة بيتغير، بنخلي واجهة المشغل تكبر أو تصغر معاها عشان تملأ المساحة كلها
    playerGUI.setBounds(getLocalBounds());
}
