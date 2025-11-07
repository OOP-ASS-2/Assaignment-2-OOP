#include "PlayerGUI.h"
#include <cmath> 

PlayerGUI::PlayerGUI() : waveformDisplay(playerAudio)
{

    juce::Button* buttons[] = { &loadButton, &playButton, &pauseButton, &stopButton, &restartButton, &goToEndButton, &loopButton, &muteButton , &seekBackwardButton, &seekForwardButton , &abLoopStartButton, &abLoopEndButton, &abLoopToggleButton };
    for (auto* btn : buttons)
    {
        addAndMakeVisible(btn);
        btn->addListener(this);
    }

    addAndMakeVisible(waveformDisplay);

    addAndMakeVisible(volumeSlider);
    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue(0.5);
    volumeSlider.addListener(this);

    addAndMakeVisible(speedSlider);
    speedSlider.setRange(0.5, 2.0, 0.01);
    speedSlider.setValue(1.0);
    speedSlider.addListener(this);

    addAndMakeVisible(speedLabel);
    addAndMakeVisible(positionLabel);
    speedLabel.attachToComponent(&speedSlider, true);

    addAndMakeVisible(positionSlider);
    positionSlider.setRange(0.0, 1.0, 0.001);
    positionSlider.addListener(this);

    positionLabel.setText("00:00 / 00:00", juce::dontSendNotification);
    positionLabel.setJustificationType(juce::Justification::centred);


    addAndMakeVisible(metadataLabel);
    metadataLabel.setColour(juce::Label::textColourId, juce::Colours::cyan);

    metadataLabel.setFont(juce::Font(juce::FontOptions(12.0f, juce::Font::plain)));
    metadataLabel.setJustificationType(juce::Justification::topLeft);
    metadataLabel.setText("Metadata: No file loaded.", juce::dontSendNotification);


    loopStartPoint = -1.0;
    loopEndPoint = -1.0;

    startTimerHz(30);
}

PlayerGUI::~PlayerGUI()
{
}

void PlayerGUI::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(40, 40, 40));
    g.setColour(juce::Colours::lightgrey);
    g.drawRect(getLocalBounds(), 1);
    g.setColour(juce::Colours::white);

    g.setFont(juce::Font(juce::FontOptions(18.0f, juce::Font::plain)));
    g.drawText("Player", 5, 5, 80, 20, juce::Justification::centredLeft);
}

void PlayerGUI::resized()
{

    auto bounds = getLocalBounds().reduced(5);


    auto buttonArea = bounds.removeFromTop(proportionOfHeight(0.3f));

    auto row1 = buttonArea.removeFromTop(buttonArea.getHeight() / 2).reduced(0, 2);
    auto btnWidth1 = row1.getWidth() / 5;
    loadButton.setBounds(row1.removeFromLeft(btnWidth1).reduced(2));
    playButton.setBounds(row1.removeFromLeft(btnWidth1).reduced(2));
    pauseButton.setBounds(row1.removeFromLeft(btnWidth1).reduced(2));
    stopButton.setBounds(row1.removeFromLeft(btnWidth1).reduced(2));
    restartButton.setBounds(row1.removeFromLeft(btnWidth1).reduced(2));

    auto row2 = buttonArea.reduced(0, 2);
    auto btnWidth2 = row2.getWidth() / 8;
    seekBackwardButton.setBounds(row2.removeFromLeft(btnWidth2).reduced(2));
    seekForwardButton.setBounds(row2.removeFromLeft(btnWidth2).reduced(2));
    goToEndButton.setBounds(row2.removeFromLeft(btnWidth2).reduced(2));
    loopButton.setBounds(row2.removeFromLeft(btnWidth2).reduced(2));
    muteButton.setBounds(row2.removeFromLeft(btnWidth2).reduced(2));
    abLoopStartButton.setBounds(row2.removeFromLeft(btnWidth2).reduced(2));
    abLoopEndButton.setBounds(row2.removeFromLeft(btnWidth2).reduced(2));
    abLoopToggleButton.setBounds(row2.removeFromLeft(btnWidth2).reduced(2));



    auto middleArea = bounds.removeFromTop(proportionOfHeight(0.4f));
    waveformDisplay.setBounds(middleArea.removeFromTop(middleArea.getHeight() * 0.7f));
    metadataLabel.setBounds(middleArea.reduced(5));


    auto posArea = bounds.removeFromTop(bounds.getHeight() * 0.5f);


    positionLabel.setBounds(posArea.removeFromTop(posArea.getHeight() * 0.5f));

    positionSlider.setBounds(posArea.reduced(5, 0));


    auto volSpeedArea = bounds;
    auto leftHalf = volSpeedArea.removeFromLeft(volSpeedArea.getWidth() / 2);
    auto rightHalf = volSpeedArea;


    volumeSlider.setBounds(leftHalf.reduced(5));
    speedSlider.setBounds(rightHalf.reduced(5));
}

void PlayerGUI::buttonClicked(juce::Button* b)
{
    if (b == &loadButton)
    {
        if (playlist != nullptr)
        {
            auto selectedFile = playlist->getSelectedFile();
            if (selectedFile.existsAsFile())
            {
                playerAudio.stop();

                if (playerAudio.loadFile(selectedFile))
                {
                    waveformDisplay.loadURL(selectedFile);
                    metadataLabel.setText(playerAudio.getMetadata(), juce::dontSendNotification);
                    loopStartPoint = -1.0;
                    loopEndPoint = -1.0;
                    abLoopStartButton.setButtonText("A");
                    abLoopEndButton.setButtonText("B");
                    abLoopToggleButton.setToggleState(false, juce::dontSendNotification);
                }
            }
            else
            {
                juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,
                    "No File Selected",
                    "Please select a file from the playlist first.");
            }
        }
    }
    else if (b == &playButton) playerAudio.play();
    else if (b == &pauseButton) playerAudio.pause();
    else if (b == &stopButton) playerAudio.stop();
    else if (b == &restartButton) playerAudio.restart();
    else if (b == &goToEndButton) playerAudio.goToEnd();
    else if (b == &loopButton) playerAudio.setLooping(b->getToggleState());
    else if (b == &muteButton) playerAudio.toggleMute();
    else if (b == &seekBackwardButton)
    {
        double currentPos = playerAudio.getTransportSource()->getCurrentPosition();
        double newPos = std::max(0.0, currentPos - 10.0); // std::max ???? ??????? ??? ?????
        playerAudio.getTransportSource()->setPosition(newPos);
    }
    else if (b == &seekForwardButton)
    {
        double currentPos = playerAudio.getTransportSource()->getCurrentPosition();
        double duration = playerAudio.getTransportSource()->getLengthInSeconds();
        double newPos = std::min(duration, currentPos + 10.0); // std::min ???? ??????? ??? ????? ??????
        playerAudio.getTransportSource()->setPosition(newPos);
    }
    else if (b == &abLoopStartButton)
    {
        loopStartPoint = playerAudio.getTransportSource()->getCurrentPosition();
        abLoopStartButton.setButtonText(formatTime(loopStartPoint));

        if (loopEndPoint > 0.0 && loopStartPoint >= loopEndPoint)
        {
            loopEndPoint = -1.0;
            abLoopEndButton.setButtonText("B");
            abLoopToggleButton.setToggleState(false, juce::dontSendNotification);
        }
    }
    else if (b == &abLoopEndButton)
    {
        double currentPos = playerAudio.getTransportSource()->getCurrentPosition();

        if (loopStartPoint >= 0.0 && currentPos > loopStartPoint)
        {
            loopEndPoint = currentPos;
            abLoopEndButton.setButtonText(formatTime(loopEndPoint));
        }
        else
        {

            loopEndPoint = -1.0;
            abLoopEndButton.setButtonText("B");
            abLoopToggleButton.setToggleState(false, juce::dontSendNotification);
        }
    }
    else if (b == &abLoopToggleButton)
    {
        if (!b->getToggleState())
        {


            playerAudio.getTransportSource()->setPosition(0.0);
            loopStartPoint = -1.0;
            loopEndPoint = -1.0;
            abLoopStartButton.setButtonText("A");
            abLoopEndButton.setButtonText("B");
        }
        else
        {

            if (loopStartPoint < 0.0 || loopEndPoint <= loopStartPoint)
            {

                b->setToggleState(false, juce::dontSendNotification);
                juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,
                    "A-B Loop Error",
                    "Please set valid A and B points first. (A must be set before B)");
            }

        }
    }
}

void PlayerGUI::sliderValueChanged(juce::Slider* s)
{
    if (s == &volumeSlider)
        playerAudio.setGain((float)s->getValue());
    else if (s == &speedSlider)
        playerAudio.setSpeed(s->getValue());
    else if (s == &positionSlider && s->isMouseButtonDown())
    {
        double pos = s->getValue() * playerAudio.getTransportSource()->getLengthInSeconds();
        playerAudio.getTransportSource()->setPosition(pos);
    }
}

void PlayerGUI::timerCallback()
{
    auto* t = playerAudio.getTransportSource();
    double duration = t->getLengthInSeconds();
    double currentPosition = t->getCurrentPosition();

    bool isABLooping = abLoopToggleButton.getToggleState();

    if (isABLooping && loopStartPoint >= 0.0 && loopEndPoint > loopStartPoint)
    {

        if (currentPosition >= loopEndPoint)
        {

            t->setPosition(loopStartPoint);

            currentPosition = loopStartPoint;
        }
    }

    if (t->isPlaying())
    {
        if (duration > 0)
        {
            positionSlider.setValue(currentPosition / duration, juce::dontSendNotification);
        }
    }


    auto formatTime = [](double seconds)
        {
            int totalSeconds = (int)std::round(seconds);
            int minutes = totalSeconds / 60;
            int remainingSeconds = totalSeconds % 60;
            return juce::String::formatted("%02d:%02d", minutes, remainingSeconds);
        };

    juce::String timeText;
    if (duration > 0)
    {
        timeText << formatTime(currentPosition) << " / " << formatTime(duration);
    }
    else
    {
        timeText = "00:00 / 00:00";
    }

    positionLabel.setText(timeText, juce::dontSendNotification);
}



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

juce::AudioSource* PlayerGUI::getOutputAudioSource()
{
    return playerAudio.getOutputAudioSource();
}

void PlayerGUI::setPlaylist(Playlist* playlistPtr)
{
    playlist = playlistPtr;
}
