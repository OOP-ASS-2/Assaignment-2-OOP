#include <JuceHeader.h>
#include "MainComponent.h"


class AudioMixerApp : public juce::JUCEApplication
{
public:
    const juce::String getApplicationName() override { return "JUCE Audio Mixer – Two Players"; }
    const juce::String getApplicationVersion() override { return "1.0"; }
    bool moreThanOneInstanceAllowed() override { return true; }


    void initialise(const juce::String&) override
    {
        mainWindow.reset(new MainWindow(getApplicationName(), new MainComponent(), *this));
    }

    void shutdown() override
    {
        mainWindow = nullptr;
    }

    void systemRequestedQuit() override { quit(); }

private:

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

            setUsingNativeTitleBar(true);


            setTitleBarTextCentred(true);
            setColour(DocumentWindow::textColourId, juce::Colours::white);


            setContentOwned(c, true);


            setResizable(true, true);


            centreWithSize(900, 550);


            setVisible(true);


            setName(" JUCE Audio Mixer");
        }


        void closeButtonPressed() override
        {
            juce::JUCEApplication::getInstance()->systemRequestedQuit();
        }

    private:
        JUCEApplication& app;
    };

    std::unique_ptr<MainWindow> mainWindow;
};


START_JUCE_APPLICATION(AudioMixerApp)
