
#pragma once

#include <JuceHeader.h>
#include "PlayerAudio.h" // لازم نعمل include للملف ده عشان يعرف يعني إيه PlayerAudio
#include "PlayerGUI.h"   // ولازم ده عشان يعرف يعني إيه PlayerGUI

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public juce::AudioAppComponent
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    //==============================================================================
    // Your private member variables go here...

    // هنا بنعمل أوبجكت من كلاس الصوت عشان يشيل كل اللوجيك بتاع تشغيل الملفات
    PlayerAudio playerAudio;

    // وهنا بنعمل أوبجكت الواجهة، وبنديله الأوبجكت بتاع الصوت (playerAudio)
    // عشان لما ندوس على زرار في الواجهة، يعرف يكلم مين عشان يشغل أو يوقف الصوت
    PlayerGUI   playerGUI{ playerAudio };


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
