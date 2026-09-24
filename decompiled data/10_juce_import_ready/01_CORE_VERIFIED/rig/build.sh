#!/bin/sh
# Сборка измерительного стенда: исполняет НАСТОЯЩЕЕ ядро OS 1.32B в эмуляторе dsp56k.
# Требует: исходники dsp56kEmu (github.com/dsp56300/dsp56300) в $EMU,
#          патчи из rig/EMULATOR_PATCHES.md применённые к $EMU,
#          образы памяти OS 1.32B (dsp1_pmem.bin / dsp1_xmem.bin / dsp1_ymem.bin).
# usage: ./build.sh <path-to-dsp56300-source>
set -e
EMU=${1:?path to dsp56300 source tree}
SRC=$EMU/source/dsp56kEmu
FLAGS="-std=c++20 -O1 -DDSP56300_NO_JIT -I$EMU/source/asmjit -I$EMU/source"
for f in dsp memory disasm opcodes agu error registers peripherals instructioncache; do
  g++ $FLAGS -c $SRC/$f.cpp -o $f.o
done
for f in $SRC/../dsp56kBase/logging.cpp $SRC/memorybuffer.cpp $SRC/omfloader.cpp $SRC/types.cpp $SRC/utils.cpp $SRC/esai.cpp $SRC/esaiclock.cpp $SRC/esxi.cpp $SRC/hdi08.cpp $SRC/essi.cpp $SRC/dma.cpp $SRC/timers.cpp $SRC/../dsp56kBase/mmuhelper.cpp; do
  g++ $FLAGS -c $f -o $(basename $f .cpp).o
done
g++ $FLAGS -c stub_jit.cpp -o stub_jit.o
g++ $FLAGS -c rig.cpp -o rig.o
g++ dsp.o memory.o disasm.o opcodes.o agu.o error.o registers.o peripherals.o rig.o stub_jit.o \
    logging.o memorybuffer.o omfloader.o types.o utils.o esai.o esaiclock.o esxi.o hdi08.o essi.o \
    dma.o timers.o mmuhelper.o instructioncache.o -o rig
echo "built ./rig — запуски: ./rig map | ./rig env ATK DEC SUS REL N | ./rig sig [dist freq]"
