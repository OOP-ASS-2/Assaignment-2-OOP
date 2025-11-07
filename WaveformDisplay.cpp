
#include "WaveformDisplay.h"

WaveformDisplay::WaveformDisplay(PlayerAudio& audioPlayer) : playerAudio(audioPlayer)
{

    startTimerHz(30);
}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint(juce::Graphics& g)
{

    g.fillAll(juce::Colours::darkgrey.darker());
    g.setColour(juce::Colours::grey);
    g.drawRect(getLocalBounds(), 1.0f);


    auto& thumbnail = playerAudio.getThumbnail();
    if (thumbnail.getNumChannels() > 0)
    {
        g.setColour(juce::Colours::cornflowerblue);

        thumbnail.drawChannels(g,
            getLocalBounds().reduced(2),
            0.0,
            thumbnail.getTotalLength(),
            1.0f);
    }
    else
    {

        g.setColour(juce::Colours::lightgrey);
        g.drawFittedText("Load a file to see the waveform", getLocalBounds(), juce::Justification::centred, 1);
    }


    drawPlaybackPosition(g);
}

void WaveformDisplay::resized()
{

}

void WaveformDisplay::timerCallback()
{

    repaint();
}

void WaveformDisplay::drawPlaybackPosition(juce::Graphics& g)
{
    auto duration = playerAudio.getTransportSource()->getLengthInSeconds();
    if (duration > 0.0)
    {

        auto currentPos = playerAudio.getTransportSource()->getCurrentPosition();


        auto drawPosition = (currentPos / duration) * getWidth();


        g.setColour(juce::Colours::yellow);
        g.drawVerticalLine((int)drawPosition, 0.0f, (float)getHeight());
    }
}
