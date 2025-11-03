
#include <JuceHeader.h>
#include "MainComponent.h"

//==============================================================================
// Application Class
//==============================================================================
class AudioMixerApp : public juce::JUCEApplication
{
public:
    const juce::String getApplicationName() override { return "JUCE Audio Mixer – Two Players"; }
    const juce::String getApplicationVersion() override { return "1.0"; }
    bool moreThanOneInstanceAllowed() override { return true; }

    // عند بدء التشغيل
    void initialise(const juce::String&) override
    {
        mainWindow.reset(new MainWindow(getApplicationName(), new MainComponent(), *this));
    }

    // عند إغلاق التطبيق
    void shutdown() override
    {
        mainWindow = nullptr;
    }

    void systemRequestedQuit() override { quit(); }

private:
    //==============================================================================
    // Main Window Class
    class MainWindow : public juce::DocumentWindow
    {
    public:
        MainWindow(juce::String name, juce::Component* c, JUCEApplication& a)
            : DocumentWindow(name,
                juce::Desktop::getInstance().getDefaultLookAndFeel()
                .findColour(ResizableWindow::backgroundColourId),
                DocumentWindow::allButtons),
            app(a)
        {
            // استخدم شريط العنوان الأصلي بتاع الويندوز
            setUsingNativeTitleBar(true);

            // خلي النص في النص باللون الأبيض
            setTitleBarTextCentred(true);
            setColour(DocumentWindow::textColourId, juce::Colours::white);

            // حط المحتوى الرئيسي (MainComponent)
            setContentOwned(c, true);

            // خليه قابل للتكبير والتصغير
            setResizable(true, true);

            // خلي حجم النافذة مناسب والعنوان ما يتقطعش
            centreWithSize(900, 550);

            // خليه مرئي بعد ما يتجهز
            setVisible(true);

            // اسم العنوان فعلياً
            setName(" JUCE Audio Mixer");
        }

        // لما المستخدم يقفل البرنامج
        void closeButtonPressed() override
        {
            juce::JUCEApplication::getInstance()->systemRequestedQuit();
        }

    private:
        JUCEApplication& app;
    };

    std::unique_ptr<MainWindow> mainWindow;
};

//==============================================================================
// Entry point
//==============================================================================
START_JUCE_APPLICATION(AudioMixerApp)
