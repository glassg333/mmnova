// 1.6.40: LAYOUT EDITOR -- отдельное приложение «только интерфейс» (без аудио/DAW).
// Открывает тот же редактор поверх живого процессора (ручки работают, звук не звучит)
// и сразу включает LAYOUT EDIT. Расклад пишется в MonomachineNova/layout.json --
// тот же файл, что использует плагин: передвинул здесь -- в плагине стало так.
#include <JuceHeader.h>
#include "PluginEditor.h"

class LayoutEditorWindow final : public juce::DocumentWindow {
public:
    explicit LayoutEditorWindow(MonomachineNovaAudioProcessor& p):DocumentWindow("Monomachine Nova LAYOUT EDITOR (MENU -> LAYOUT EDIT)",juce::Colours::black,closeButton){
        setUsingNativeTitleBar(true);
        editor=std::make_unique<MonomachineNovaAudioProcessorEditor>(p);
        setContentNonOwned(editor.get(),true);
        const auto b=getContentComponentBorder();
        centreWithSize(editor->getWidth()+b.getLeft()+b.getRight(),editor->getHeight()+b.getTop()+b.getBottom());
        setVisible(true);
        editor->startLayoutEdit(); // 1.6.40: сразу режим правки расклада
    }
    void closeButtonPressed() override { juce::JUCEApplication::getInstance()->systemRequestedQuit(); }
private:
    std::unique_ptr<MonomachineNovaAudioProcessorEditor> editor;
};
class LayoutEditorApplication final : public juce::JUCEApplication {
public:
    const juce::String getApplicationName() override { return "Monomachine Nova Layout Editor"; }
    const juce::String getApplicationVersion() override { return "1.6.40"; }
    bool moreThanOneInstanceAllowed() override { return true; }
    void initialise(const juce::String&) override {
        processor=std::make_unique<MonomachineNovaAudioProcessor>(); // параметры живые, аудио не запускается
        window=std::make_unique<LayoutEditorWindow>(*processor);
    }
    void shutdown() override { window.reset(); processor.reset(); }
    void systemRequestedQuit() override { quit(); }
    void anotherInstanceStarted(const juce::String&) override {}
private:
    std::unique_ptr<MonomachineNovaAudioProcessor> processor;
    std::unique_ptr<LayoutEditorWindow> window;
};
START_JUCE_APPLICATION(LayoutEditorApplication)
