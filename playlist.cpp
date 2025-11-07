#include "Playlist.h"

Playlist::Playlist()
{
    addAndMakeVisible(listBox);
    addAndMakeVisible(addButton);
    addAndMakeVisible(removeButton);

    listBox.setModel(this);
    listBox.setColour(juce::ListBox::backgroundColourId, juce::Colours::darkgrey.darker());

    addButton.addListener(this);
    removeButton.addListener(this);

    removeButton.setEnabled(false);
}

Playlist::~Playlist()
{
}

void Playlist::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(40, 40, 40));
    g.setColour(juce::Colours::lightgrey);
    g.drawRect(getLocalBounds(), 1);
    g.setColour(juce::Colours::white);

    g.setFont(juce::Font(juce::FontOptions(18.0f, juce::Font::plain)));
    g.drawText("Playlist", 5, 5, 80, 20, juce::Justification::centredLeft);
}

void Playlist::resized()
{

    const int margin = 10;
    auto bounds = getLocalBounds().reduced(margin);


    const int buttonHeight = 30;
    auto buttonArea = bounds.removeFromBottom(buttonHeight);


    bounds.removeFromBottom(margin / 2);


    listBox.setBounds(bounds);


    auto buttonWidth = (buttonArea.getWidth() - (margin / 2)) / 2;
    addButton.setBounds(buttonArea.removeFromLeft(buttonWidth));

    buttonArea.removeFromLeft(margin / 2);

    removeButton.setBounds(buttonArea);
}



int Playlist::getNumRows()
{
    return files.size();
}

void Playlist::paintListBoxItem(int rowNumber, juce::Graphics& g,
    int width, int height, bool rowIsSelected)
{
    if (rowIsSelected)
        g.fillAll(juce::Colours::cornflowerblue.darker());

    g.setColour(juce::Colours::lightgrey);

    g.setFont(juce::Font(juce::FontOptions(14.0f, juce::Font::plain)));
    g.drawText(files[rowNumber].getFileNameWithoutExtension(),
        5, 0, width, height,
        juce::Justification::centredLeft, true);
}

void Playlist::selectedRowsChanged(int /* lastRowSelected */)
{
    removeButton.setEnabled(listBox.getSelectedRow() != -1);
}



void Playlist::buttonClicked(juce::Button* button)
{
    if (button == &addButton)
    {
        fileChooser = std::make_unique<juce::FileChooser>(
            "Select audio files to add to the playlist...",
            juce::File::getSpecialLocation(juce::File::userDocumentsDirectory),
            "*.mp3;*.wav;*.flac"
        );

        auto fileBrowserFlags = juce::FileBrowserComponent::openMode |
            juce::FileBrowserComponent::canSelectMultipleItems;

        fileChooser->launchAsync(fileBrowserFlags,
            [this](const juce::FileChooser& chooser)
            {
                auto results = chooser.getResults();
                if (results.size() > 0)
                {
                    loadFiles(results);
                }

                fileChooser.reset();
            });
    }
    else if (button == &removeButton)
    {
        int selectedRow = listBox.getSelectedRow();
        if (selectedRow != -1)
        {
            files.remove(selectedRow);
            listBox.updateContent();
            listBox.selectRow(-1);
        }
    }
}

void Playlist::loadFiles(const juce::Array<juce::File>& filesToLoad)
{
    bool filesAdded = false;
    for (const auto& fileToAdd : filesToLoad)
    {
        if (fileToAdd.existsAsFile())
        {
            files.add(fileToAdd);
            filesAdded = true;
        }
    }

    if (filesAdded)
    {
        listBox.updateContent();
    }
}

juce::File Playlist::getSelectedFile() const
{
    int selectedRow = listBox.getSelectedRow();
    if (selectedRow != -1 && selectedRow < files.size())
    {
        return files[selectedRow];
    }
    return juce::File();
}