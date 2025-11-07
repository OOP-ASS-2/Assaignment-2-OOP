#pragma once

#include <JuceHeader.h>

class Playlist : public juce::Component,
    public juce::Button::Listener,
    public juce::ListBoxModel
{
public:
    Playlist();
    ~Playlist() override;

    void paint(juce::Graphics& g) override;
    void resized() override;


    int getNumRows() override;
    void paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected) override;
    void selectedRowsChanged(int lastRowSelected) override;

    void buttonClicked(juce::Button* button) override;


    void loadFiles(const juce::Array<juce::File>& filesToLoad);
    juce::File getSelectedFile() const;

private:
    juce::ListBox listBox;
    juce::TextButton addButton{ "ADD" };
    juce::TextButton removeButton{ "REMOVE" };

    juce::Array<juce::File> files;


    std::unique_ptr<juce::FileChooser> fileChooser;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Playlist)
};