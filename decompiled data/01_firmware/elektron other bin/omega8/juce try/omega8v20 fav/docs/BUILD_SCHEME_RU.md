# Схема рабочих проектов Monomachine Nova (для будущих плагинов)

Срисовано с ветки/main репо `glassg333/mmnova` — версии **1.9.9 good or no idk stable**
(обе: FX и Synth), **MonoJuice 0.8.0** и билдера `JUCE/juce + vs builder`.
Проверено: именно в таком виде Projucer (`C:\JUCE\Projucer.exe`) их грузит,
а `build_vst3.ps1` пересохраняет и собирает.

## 1. Корень .jucer — ТОЛЬКО `<JUCERPROJECT>` (старая схема с
`<JUCEPROJECTFORMATVERSION>` + `<JUCEPROJECT>` НЕ грузится — Projucer падает
с "Failed to load the project file")

```xml
<?xml version='1.0' encoding='utf-8'?>
<JUCERPROJECT id="..." name="..." projectType="audioplug" version="1.9.9"
    jucerFormatVersion="1" cppLanguageStandard="17"
    companyName="..." bundleIdentifier="com...."
    pluginName="..." pluginDesc="..." pluginManufacturer="..."
    pluginManufacturerCode="MnOn" pluginCode="NvSy"
    pluginFormats="buildVST3"
    pluginCharacteristicsValue="pluginIsSynth,pluginWantsMidiIn"
    pluginVST3Category="Instrument,Synth"
    useAppConfig="1" addUsingNamespaceToJuceHeader="0"
    defines="JUCE_VST3_CAN_REPLACE_VST2=0">
  <MAINGROUP id="..." name="...">
    <GROUP id="g1" name="Source">
      <FILE id="f1" name="PluginProcessor.cpp" compile="1" resource="0" file="Source/PluginProcessor.cpp" />
      <!-- ... .h compile="0" ... -->
    </GROUP>
  </MAINGROUP>
  <EXPORTFORMATS>...</EXPORTFORMATS>
  <MODULES>...</MODULES>
  <JUCEOPTIONS JUCE_WEB_BROWSER="0" JUCE_USE_CURL="0" JUCE_VST3_CAN_REPLACE_VST2="0" />
</JUCERPROJECT>
```

### Ключевые отличия синт vs FX (что менять)
| | Синт (Nova Synth) | FX (Nova FX / MonoJuice) |
|---|---|---|
| `pluginCharacteristicsValue` | `pluginIsSynth,pluginWantsMidiIn` | `pluginWantsMidiIn,pluginProducesMidiOut` (FX без synth) |
| `pluginVST3Category` | `Instrument,Synth` | `Fx` |
| `pluginCode` | 4 символа, напр. `NvSy` | 4 символа, напр. `MnFx` |

## 2. MODULES — ровно этот список, все `useGlobalPath="1"` (билдер сам
переключает в useGlobalPath=1/useLocalCopy=0, "модули уже общие"):

juce_audio_basics, juce_audio_devices, juce_audio_formats,
**juce_audio_plugin_client** (обязателен для плагинов!),
juce_audio_processors, **juce_audio_processors_headless** (есть в их JUCE),
juce_audio_utils, juce_core, juce_data_structures, juce_events,
juce_graphics, juce_gui_basics, juce_gui_extra

```xml
<MODULE id="juce_core" showAllCode="1" useLocalCopy="0" useGlobalPath="1" />
```

## 3. EXPORTFORMATS — VS2022 (+ XCODE_MAC, LINUX_MAKE для вида)

```xml
<VS2022 targetFolder="Builds/VisualStudio2022">
  <CONFIGURATIONS>
    <CONFIGURATION name="Debug"   isDebug="1" targetName="<ИмяПлагина>"
                   optimisation="1" winArchitecture="x64" extraCompilerFlags="/utf-8" />
    <CONFIGURATION name="Release" isDebug="0" targetName="<ИмяПлагина>"
                   optimisation="3" winArchitecture="x64"
                   linkTimeOptimisation="0" extraCompilerFlags="/utf-8" />
  </CONFIGURATIONS>
  <MODULEPATHS>
    <MODULEPATH id="juce_audio_plugin_client" />
    <MODULEPATH id="juce_audio_processors" />
    <MODULEPATH id="juce_audio_utils" />
    <MODULEPATH id="juce_audio_formats" />
    <MODULEPATH id="juce_audio_devices" />
    <MODULEPATH id="juce_audio_basics" />
    <MODULEPATH id="juce_audio_processors_headless" path="C:/JUCE/modules" />
  </MODULEPATHS>
</VS2022>
```

**Настройки вывода удачного синта** (Release x64): `buildVST3`,
`optimisation="3"`, `linkTimeOptimisation="0"`, `winArchitecture="x64"`,
`extraCompilerFlags="/utf-8"`, `targetFolder="Builds/VisualStudio2022"`,
`defines="JUCE_VST3_CAN_REPLACE_VST2=0"`.

## 4. Источники
- **Первый include в главном заголовке — `<JuceHeader.h>`** (как `NovaData.h`,
  `NovaDSP.h` в рабочем проекте). Без него `juce_TargetPlatform.h` падает с
  `error C1189: No global header file was included!`.
  У остальных — только прямые модули (`#include <juce_audio_processors/...>`)
  и свои заголовки; `juce::` везде явно (`addUsingNamespaceToJuceHeader="0"`).

## 5. Билдер `JUCE/juce + vs builder/`
- `build_vst3.bat` — перетащить папку проекта (где лежит .jucer), либо запустить без аргументов.
- `build_vst3.ps1` — последовательность:
  1) найти `*.jucer` в папке;
  2) переключить все `<MODULE>` на `useGlobalPath="1" useLocalCopy="0"`;
  3) `Projucer --resave <jucer>` (глобальный путь модулей `C:\JUCE\modules`);
  4) MSBuild Release x64: `C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\amd64\MSBuild.exe`;
  5) копия `.vst3` в `C:\Program Files\Common Files\VST3\!build test`;
  6) опции: `DeleteBuilds, NoPdb, NoManifest, CleanModules, CloseHosts, Formats=[VST3]`.
- `build_config.json` — хранит пути Projucer/MSBuild/Vst3Dir и т.д.

## 6. monomod — полная эмуляция прошивки (для сравнения путей)
`decompiled data/monomod/` — VST-оболочка вокруг **dsp56300** (эмулятор DSP56300),
запускает реальную прошивку Elektron (`Elektron_SFX6-60_OS1.32B.syx`), CMake-сборка,
`src/core/firmware/Firmware.cpp` + своя DSP-часть. Такой путь возможен ТОЛЬКО когда
существует готовое ядро эмулятора CPU и прошивка открыта. У Omega 8/CODE ядра
эмулятора нет, а наша прошивка (7-bit образ, без известной кодировки) не поддаётся
даже строковому анализу — поэтому для Omega выбран путь «реконструкция, максимально
близкая к патчам» (карта 176 байт, сверенная со скриншотом редактора 78/78).
От мономода срисовать можно только ОБОЛОЧКУ и схему сборки (пункты 1–5).

## 5. Ошибки MSVC из сборок пользователя и фиксы (2026-09-29)

| Ошибка | Причина | Фикс |
|---|---|---|
| C1189 "No global header file was included!" | первый include в главном заголовке не `<JuceHeader.h>` | `PluginProcessor.h`: `#include <JuceHeader.h>` первым |
| C2228 `.hpS1` / C2296 `&` на float | опечатки: указатель вместо `->`, `(float)wb & 7.0f` | `v->hpS1`; строка `float w1 = ...` удалена (не использовалась) |
| C2039 `setInteractionStateKey` | такого метода в `juce::Slider` нет (вымышленный API) | строка удалена |
| C2039 `BinaryData::Omega8FactoryA_CS_syx` | имя символа BinaryData зависит от версии Projucer (разные JUCE генерируют по-разному) | банк вшит через генерируемый `Source/OmegaBankData.h` (`namespace omega8_bank`), BinaryData больше не используется |
| C2373 `getProgramName` | в .cpp не совпадал `const` с объявлением в .h | `const juce::String ...::getProgramName` |
| C2660 `loadFileAsData` | API требует `MemoryBlock&`, а не возвращает значение | `juce::MemoryBlock data; f.loadFileAsData (data)` |
| C2039 `pitchWheelValue` | правильное имя — `getPitchWheelValue()` | заменено |
| C2661 `getBus (true)` | сигнатура `getBus (bool isInput, int index)` | `getBus (true, 0)` |
