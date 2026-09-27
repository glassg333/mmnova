#!/bin/bash
# ===== 1.8.4: ЕДИНСТВЕННЫЙ источник JUCE+шим. ВЫЗЫВАТЬ ПЕРВОЙ СТРОКОЙ любого билда. =====
# Правило юзера: JUCE НИКОГДА не хранить в workspace (лимит снапшота 128MB/10k файлов).
# /tmp зачищается средой ЧАЩЕ, чем ожидается (даже посреди хода) -- поэтому скрипт идемпотентный.
set -e
if [ ! -d /tmp/JUCE/modules ]; then
  echo "=== JUCE download $(date +%M:%S)"
  curl -sL -o /tmp/juce.zip https://github.com/juce-framework/JUCE/releases/download/8.0.4/juce-8.0.4-linux.zip
  unzip -q /tmp/juce.zip -d /tmp && rm -f /tmp/juce.zip
  [ -d /tmp/JUCE ] || mv /tmp/JUCE* /tmp/JUCE 2>/dev/null || true
fi
[ -d /tmp/JUCE/modules ] || { echo "SETUP_JUCE FAILED"; exit 1; }
# шим обязателен: проектный JuceHeader.h тянет juce_audio_processors_headless (нет в JUCE 8.0.4 zip)
mkdir -p /tmp/hdtest/shim
cp -n /home/user/batches/hdtest/shim/JuceHeader.h /tmp/hdtest/shim/ 2>/dev/null || true
[ -f /tmp/hdtest/shim/JucePluginDefines.h ] || cp /home/user/mmnova/JUCE/Monomachine-Nova-1.8.3/Monomachine_Nova_Synth/JuceLibraryCode/JucePluginDefines.h /tmp/hdtest/shim/ 2>/dev/null || true
[ -f /tmp/hdtest/shim/JucePluginDefines.h ] || { echo "SETUP_JUCE: no JucePluginDefines source"; exit 1; }
echo "setup_juce OK: $(ls /tmp/JUCE/modules | wc -l) modules"
