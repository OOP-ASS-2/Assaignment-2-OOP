
#include "PlayerGUI.h"
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
	g.fillAll(juce::Colours::darkgrey);
}
PlayerGUI::PlayerGUI()
{
	// Add buttons
	for (auto* btn : { &loadButton, &restartButton , &stopButton , &playButton , &loopButton , &muteButton })
	{
		btn->addListener(this);
		addAndMakeVisible(btn);
	}

	// Volume slider
	volumeSlider.setRange(0.0, 1.0, 0.01);
	volumeSlider.setValue(0.5);
	volumeSlider.addListener(this);
	addAndMakeVisible(volumeSlider);
}
void PlayerGUI::resized()
{
	int y = 20;
	loadButton.setBounds(20, y, 100, 40);
	restartButton.setBounds(140, y, 80, 40);
	stopButton.setBounds(240, y, 80, 40);
	playButton.setBounds(340, y, 80, 40);
	loopButton.setBounds(440, y, 80, 40);
	muteButton.setBounds(540, y, 80, 40);
	

	volumeSlider.setBounds(20, 100, getWidth() - 40, 30);
}


PlayerGUI::~PlayerGUI()
{
}

void PlayerGUI::buttonClicked(juce::Button* button)
{
	if (button == &loadButton)
	{
		juce::FileChooser chooser("Select audio files...",
			juce::File{},
			"*.wav;*.mp3");

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

	else if (button == &restartButton)
	{
		playerAudio.setPosition(0.0);
		playerAudio.start();
	}

	else if (button == &stopButton)
	{
		playerAudio.stop();
		
	}

	else if (button == &playButton) {
		playerAudio.start();
	}
	
	else if (button == &loopButton)
	{
		stutes = !stutes;
		loopButton.setButtonText(stutes ? "Loop: ON" : "Loop: OFF");
		playerAudio.setLooping(stutes);
	}
	
	else if (button == &muteButton)
	{
		if (!isMuted)
		{	
			lastVolume = (float)volumeSlider.getValue();
			playerAudio.setGain(0.0f);
			isMuted = true;
			muteButton.setButtonText("Unmute");
		}
		else
		{
			playerAudio.setGain(lastVolume);
			isMuted = false;
			muteButton.setButtonText("Mute");
		}
	}


}


void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
	if (slider == &volumeSlider)
		playerAudio.setGain((float)slider->getValue());
}
