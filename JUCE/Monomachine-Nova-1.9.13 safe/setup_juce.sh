#!/bin/bash
# 1.6.x: восстановление окружения синтакс-проверок после чистки /tmp
set -e
if [ ! -d /tmp/JUCE/modules ]; then
  cd /tmp
  curl -sL -o juce.zip https://github.com/juce-framework/JUCE/releases/download/8.0.4/juce-8.0.4-linux.zip
  rm -rf JUCE juce-8.0.4
  unzip -q juce.zip
  [ -d JUCE ] || mv juce-8.0.4 JUCE
fi
mkdir -p /tmp/stub/juce_audio_processors_headless
cat > /tmp/stub/juce_audio_processors_headless/juce_audio_processors_headless.h <<'EOF'
#pragma once
// 1.6.x syntax-check stub: заглушка несуществующего модуля для -fsyntax-only
#include <juce_audio_processors/juce_audio_processors.h>
EOF
echo "JUCE env ready"
