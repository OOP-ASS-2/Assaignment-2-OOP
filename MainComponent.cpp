#include "MainComponent.h"

MainComponent::MainComponent()
{
    addAndMakeVisible(player1);
    addAndMakeVisible(player2);
    addAndMakeVisible(playlist);

    player1.setPlaylist(&playlist);
    player2.setPlaylist(&playlist);

    addAndMakeVisible(masterVolumeSlider);
    masterVolumeSlider.setRange(0.0, 1.0, 0.01);
    masterVolumeSlider.setValue(masterGain);
    masterVolumeSlider.addListener(this);


    addAndMakeVisible(crossfaderSlider);
    crossfaderSlider.setRange(0.0, 1.0, 0.01);
    crossfaderSlider.setValue(0.5);
    crossfaderSlider.addListener(this);

    player1.setPlayerGain(0.5f);
    player2.setPlayerGain(0.5f);


    setSize(900, 600);
    setAudioChannels(0, 2);

    mixerSource.addInputSource(player1.getOutputAudioSource(), false);
    mixerSource.addInputSource(player2.getOutputAudioSource(), false);
}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    mixerSource.getNextAudioBlock(bufferToFill);
    bufferToFill.buffer->applyGain(masterGain);
}

void MainComponent::releaseResources()
{
    mixerSource.releaseResources();
}

void MainComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(20, 60, 60));
    g.setColour(juce::Colours::yellow);

    g.setFont(juce::Font(juce::FontOptions("Arial", 34.0f, juce::Font::bold)));
    g.drawFittedText("Audio Player", getLocalBounds().removeFromTop(60),
        juce::Justification::centred, 1);

    g.setColour(juce::Colours::darkgrey);
    g.drawLine(40.0f, 70.0f, getWidth() - 40.0f, 70.0f, 2.0f);
}

void MainComponent::resized()
{
    const int margin = 10;

    auto bounds = getLocalBounds();

    bounds.removeFromTop(60);


    auto bottomArea = bounds.removeFromBottom(80).reduced(margin * 2, margin);
    masterVolumeSlider.setBounds(bottomArea.removeFromBottom(40));
    crossfaderSlider.setBounds(bottomArea);

    auto mainArea = bounds.reduced(margin);


    auto playlistArea = mainArea.removeFromLeft(mainArea.getWidth() / 4);


    mainArea.removeFromLeft(margin);

    auto playersArea = mainArea;


    playlist.setBounds(playlistArea);


    auto playerHeight = (playersArea.getHeight() - margin) / 2;

    auto player1Area = playersArea.removeFromTop(playerHeight);


    playersArea.removeFromTop(margin);
    
    auto player2Area = playersArea;

    player1.setBounds(player1Area);
    player2.setBounds(player2Area);
}

void MainComponent::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &masterVolumeSlider)
    {
        masterGain = (float)slider->getValue();
    }
    else if (slider == &crossfaderSlider)
    {
        float faderValue = (float)slider->getValue();

        player1.setPlayerGain(1.0f - faderValue);
        player2.setPlayerGain(faderValue);
    }
}
