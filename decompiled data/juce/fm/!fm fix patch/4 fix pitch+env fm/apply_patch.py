#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""apply_patch.py — ПАК 8 «FM CLOSED»: ставит бит-точные FM-ядра + точную
AMP-энвелопу во ВСЕ деревья плагина и патчит хост/UI/дефолты.

Запуск:
    python3 apply_patch.py <путь>
где <путь> — корень репозитория (внутри папка JUCE) ИЛИ конкретное дерево
.../JUCE/Monomachine-Nova-*/Monomachine_Nova_Synth (или ..._FX).

Что делает:
  1. Копирует в <Source>/dsp/mnm/ 10 файлов (бит-точные ядра FM пака 7 —
     без изменений; новая обёртка MnmFm.hpp с привязкой к SR хоста; точная
     AMP-энвелопа MnmEnvExact/MnmEnvExactTables; MnmKernel с точной
     стейт-машиной огибающей).
  2. Патчит NovaDSP.h: битая строка blockBuf (деревья не компилировались!),
     ветка kKernelAlgorithm -> точная энвелопа, setTempo -> закон HOLD,
     MOD ENV остаётся на старом пути.
  3. Патчит PluginEditor.cpp: дискретные подписи 1FRQ/2FRQ/3FRQ (24 значения
     ОС) и минус-конвенция ENV/FB (дефолты -80/-30 как на железе).
  4. Патчит models/machine_definitions.hpp: дефолты дескрипторов
     STAT 3C 40 50 1E 50 40 62 40 / PAR 3C 40 50 40 66 50 62 40 /
     DYN 40 40 40 40 4A 50 1E 40.

Повторный запуск безопасен: совпавшие файлы/патчи пропускаются с пометкой
"already identical/already applied".
"""
import os, sys, glob, shutil, hashlib

HERE = os.path.dirname(os.path.abspath(__file__))
FILES = ["MnmFm.hpp", "MnmFmDsp.hpp", "MnmFmStat.hpp", "MnmFmPar.hpp",
         "MnmFmDyn.hpp", "MnmFmSineTable.h", "MnmFmPitch.h",
         "MnmKernel.hpp", "MnmEnvExact.hpp", "MnmEnvExactTables.hpp"]


def sha16(p):
    return hashlib.sha256(open(p, 'rb').read()).hexdigest()[:16]


def pack_file(name):
    for cand in (os.path.join(HERE, "dsp", "mnm", name), os.path.join(HERE, name)):
        if os.path.exists(cand):
            return cand
    return os.path.join(HERE, "dsp", "mnm", name)


def find_roots(arg):
    """Возвращает список <Source>-каталогов (Synth и FX)."""
    p = os.path.abspath(arg)
    if os.path.basename(p) == "Source" and os.path.isdir(os.path.join(p, "dsp")):
        return [p]
    if os.path.basename(p) == "dsp":
        return [os.path.dirname(p)]
    out = []
    cand = os.path.join(p, "Monomachine_Nova_Synth", "Source")
    if os.path.isdir(cand):
        out.append(cand)
    cand = os.path.join(p, "Monomachine_Nova_FX", "Source")
    if os.path.isdir(cand):
        out.append(cand)
    if out:
        return out
    # корень репозитория: ВСЕ деревья под JUCE/ (Synth + FX), рекурсивно
    for hit in sorted(glob.glob(os.path.join(p, "JUCE", "*", "Monomachine_Nova_*", "Source"))):
        if os.path.isdir(os.path.join(hit, "dsp", "mnm")) or os.path.isdir(os.path.join(hit, "dsp")):
            out.append(hit)
    return out


# --------------------------------------------------------------------------
# Текстовые патчи. (файл, old, new, описание). None в old = только проверка.
# --------------------------------------------------------------------------
def nova_patches():
    P = []
    # R1: битая строка (в 1.9.8/1.9.9 файл не компилировался)
    P.append(("NovaDSP.h",
              "float blockBufonomachine::mnm::kBlock]{};",
              "float blockBuf[monomachine::mnm::kBlock]{};",
              "fix broken blockBuf declaration"))
    # R2: on/off — HOLD теперь внутри точной энвелопы (темп-закон)
    P.append(("NovaDSP.h",
              "    void on(){gate=true;stage=1;if(mode==kKernelAlgorithm){kernelEnv.trigger();kernelHold=p[1]>0.0f;kernelHoldRemaining=static_cast<int>(sr*norm(p[1])*2);}}\n"
              "    void off(){gate=false;kernelHold=false;if(mode==kKernelAlgorithm)kernelEnv.release();if(stage)stage=4;}",
              "    void on(){gate=true;stage=1;if(mode==kKernelAlgorithm)kernelEnv.trigger();} // pack 8: HOLD lives inside the exact engine (firmware tempo law)\n"
              "    void off(){gate=false;if(mode==kKernelAlgorithm)kernelEnv.release();if(stage)stage=4;}",
              "exact engine: on/off without host-side hold"))
    # R3: tick() — точная стейт-машина
    P.append(("NovaDSP.h",
              "            kernelEnv.setParameters(p[0],p[2],p[3],0.0f);\n"
              "            if(kernelHold){\n"
              "                if(kernelHoldRemaining>0){\n"
              "                    --kernelHoldRemaining;\n"
              "                    if(kernelEnv.currentState()==monomachine::mnm::AmpEnvelope::State::Attack)kernelEnv.tick();\n"
              "                    else kernelEnv.holdPeak();\n"
              "                    return kernelEnv.value();\n"
              "                }\n"
              "                kernelHold=false;kernelEnv.resumeAfterHold();\n"
              "            }\n"
              "            kernelEnv.tick();\n"
              "            stage=(kernelEnv.currentState()==monomachine::mnm::AmpEnvelope::State::Other)?0:1;\n"
              "            return kernelEnv.value();",
              "            // PACK 8: bit-exact firmware track envelope (P:$088E-$08D7).\n"
              "            // p[0..3] = AMP page ATK/HOLD/DEC/REL; HOLD follows the firmware's\n"
              "            // tempo law inside the engine; retrigger keeps the level (dip).\n"
              "            kernelEnv.setExact(p[0],p[1],p[2],p[3],tempoBpm);\n"
              "            const float kv=kernelEnv.tick();\n"
              "            stage=(kernelEnv.currentState()==monomachine::mnm::AmpEnvelope::State::Other)?0:1;\n"
              "            return kv;",
              "exact engine tick (attack wrap, sign law, tempo HOLD, de-zipper)"))
    # R4: setTempo реально сохраняет темп
    P.append(("NovaDSP.h",
              "    void setTempo(float){}",
              "    float tempoBpm=120.0f;\n"
              "    void setTempo(float bpm){if(bpm>0.0f)tempoBpm=bpm;} // pack 8: feeds the firmware HOLD law",
              "setTempo stores bpm"))
    # R5: MOD ENV остаётся на обычном пути (точный движок = только TRACK AMP)
    P.append(("NovaDSP.h",
              "        filterModEnv.configure(AmpEnvelope::kKernelAlgorithm, {0.0f, 0.0f, 0.0f});",
              "        filterModEnv.configure(0, {0.0f, 0.0f, 0.0f}); // pack 8: MOD ENV stays on the plain path; exact engine = TRACK AMP only",
              "MOD ENV keeps legacy path"))
    return P


def editor_patches():
    P = []
    # R6: дискретные подписи FRQ (ТЗ №23)
    P.append(("PluginEditor.cpp",
              '        if((machine==8||machine==9)&&(label=="1FRQ"||label=="2FRQ"||label=="3FRQ"))return juce::String(monomachine::getFmListedRatio(static_cast<uint8_t>(raw/4)),2);',
              '        // PACK 8 (TZ23): FM+STAT/PAR 1FRQ/2FRQ/3FRQ are DISCRETE on the OS:\n'
              '        // n = floor(((K<<16)+$8000)*48/2^24) (measured at $145D38/$145DAD);\n'
              '        // screen labels = audio ratio / 2 (raw 60 -> "1/2", 80 -> "1", 102 -> "2").\n'
              '        if((machine==8||machine==9)&&(label=="1FRQ"||label=="2FRQ"||label=="3FRQ")){\n'
              '            static const char* const kFmRatioLabels[24]={"1/64","1/32","1/16","3/32","1/8","5/32","3/16","1/4","5/16","3/8","7/16","1/2","5/8","3/4","7/8","1","1.25","1.5","1.75","2","2.5","3","3.5","4"};\n'
              '            const int K=juce::jlimit(0,127,raw);\n'
              '            return juce::String(kFmRatioLabels[juce::jlimit(0,23,(((K<<16)+0x8000)*48)>>24)]);\n'
              '        }',
              "discrete FRQ labels (24 OS values)"))
    # R7: минус-конвенция ENV/FB (ТЗ №23)
    P.append(("PluginEditor.cpp",
              '        if(machine==10&&knob==4)return juce::String(std::pow(2.0f,(v-32)/24),2);',
              '        if(machine==10&&knob==4)return juce::String(std::pow(2.0f,(v-32)/24),2);\n'
              '        // PACK 8 (TZ23): the OS shows the FM ENV/FB knobs negative\n'
              '        // (measured defaults: STAT 1ENV -80, 1FB -30).\n'
              '        if((machine==8||machine==9||machine==10)&&(label.contains("ENV")||label.contains("FEN")||label.contains("VEN")||label=="1FB"||label=="2FB"))return juce::String(-raw);',
              "negative ENV/FB display"))
    return P


def definitions_patches():
    P = []
    # R8: дефолты дескрипторов (ТЗ №23)
    P.append(("models/machine_definitions.hpp",
              '                {"1FRQ", "Carrier 1 Frequency", 0, 127, 16}, {"1FIN", "Carrier 1 Fine Detune", 0, 127, 64},\n'
              '                {"1ENV", "Modulator 1 Envelope", 0, 127, 0}, {"1FB", "Carrier 1 Feedback", 0, 127, 0},\n'
              '                {"2FRQ", "Modulator 2 Frequency", 0, 127, 32}, {"2VOL", "Modulator 2 Level", 0, 127, 64},\n'
              '                {"TONE", "Harmonic Tone / Filter", 0, 127, 64}, {"TUNE", "Master Pitch Tune", 0, 127, 64}',
              '                {"1FRQ", "Carrier 1 Frequency", 0, 127, 60}, {"1FIN", "Carrier 1 Fine Detune", 0, 127, 64},\n'
              '                {"1ENV", "Modulator 1 Envelope", 0, 127, 80}, {"1FB", "Carrier 1 Feedback", 0, 127, 30},\n'
              '                {"2FRQ", "Modulator 2 Frequency", 0, 127, 80}, {"2VOL", "Modulator 2 Level", 0, 127, 64},\n'
              '                {"TONE", "Harmonic Tone / Filter", 0, 127, 98}, {"TUNE", "Master Pitch Tune", 0, 127, 64}',
              "FM+STAT defaults 3C 40 50 1E 50 40 62 40"))
    P.append(("models/machine_definitions.hpp",
              '                {"1FRQ", "Operator 1 Frequency", 0, 127, 16}, {"1ENV", "Operator 1 Envelope", 0, 127, 64},\n'
              '                {"2FRQ", "Operator 2 Frequency", 0, 127, 32}, {"2ENV", "Operator 2 Envelope", 0, 127, 64},\n'
              '                {"3FRQ", "Operator 3 Frequency", 0, 127, 48}, {"3ENV", "Operator 3 Envelope", 0, 127, 64},\n'
              '                {"TONE", "Harmonic Tone", 0, 127, 64}, {"TUNE", "Master Pitch Tune", 0, 127, 64}',
              '                {"1FRQ", "Operator 1 Frequency", 0, 127, 60}, {"1ENV", "Operator 1 Envelope", 0, 127, 80},\n'
              '                {"2FRQ", "Operator 2 Frequency", 0, 127, 80}, {"2ENV", "Operator 2 Envelope", 0, 127, 64},\n'
              '                {"3FRQ", "Operator 3 Frequency", 0, 127, 102}, {"3ENV", "Operator 3 Envelope", 0, 127, 80},\n'
              '                {"TONE", "Harmonic Tone", 0, 127, 98}, {"TUNE", "Master Pitch Tune", 0, 127, 64}',
              "FM+PAR defaults 3C 40 50 40 66 50 62 40"))
    P.append(("models/machine_definitions.hpp",
              '                {"1FRQ", "Op 1 Frequency Ratio", 0, 127, 16}, {"1FEN", "Op 1 Frequency Env Depth", 0, 127, 0},\n'
              '                {"1VOL", "Op 1 FM Level", 0, 127, 64}, {"1VEN", "Op 1 Level Env Depth", 0, 127, 0},\n'
              '                {"2FRQ", "Op 2 Frequency Ratio", 0, 127, 32}, {"2ENV", "Op 2 Env Decay/Depth", 0, 127, 80},\n'
              '                {"2FB", "Op 2 Feedback Loop", 0, 127, 30}, {"TUNE", "Master Pitch Tune", 0, 127, 64}',
              '                {"1FRQ", "Op 1 Frequency Ratio", 0, 127, 64}, {"1FEN", "Op 1 Frequency Env Depth", 0, 127, 64},\n'
              '                {"1VOL", "Op 1 FM Level", 0, 127, 64}, {"1VEN", "Op 1 Level Env Depth", 0, 127, 64},\n'
              '                {"2FRQ", "Op 2 Frequency Ratio", 0, 127, 74}, {"2ENV", "Op 2 Env Decay/Depth", 0, 127, 80},\n'
              '                {"2FB", "Op 2 Feedback Loop", 0, 127, 30}, {"TUNE", "Master Pitch Tune", 0, 127, 64}',
              "FM+DYN defaults 40 40 40 40 4A 50 1E 40"))
    return P


def apply_text_patches(source, log):
    fname_hits = {}
    for fname, old, new, desc in (nova_patches() + editor_patches() + definitions_patches()):
        path = os.path.join(source, fname)
        if not os.path.exists(path):
            fname_hits[fname] = fname_hits.get(fname, 0) + 0
            continue
        text = open(path, "r", encoding="utf-8", newline="").read()
        if new in text:
            log.append("   = %-28s already applied: %s" % (fname, desc))
            continue
        if old not in text:
            log.append("   ! %-28s PATTERN NOT FOUND: %s" % (fname, desc))
            continue
        open(path, "w", encoding="utf-8", newline="").write(text.replace(old, new, 1))
        log.append("   + %-28s patched: %s" % (fname, desc))


def main():
    if len(sys.argv) < 2:
        print(__doc__)
        return 1
    roots = find_roots(sys.argv[1])
    if not roots:
        print("не нашёл Source-каталоги; укажи путь к репозиторию или дереву")
        return 1
    print("ПАК 8 «FM CLOSED» — цели:")
    for r in roots:
        print("   " + r)
    for src in roots:
        print("\n[%s]" % src)
        mnm = os.path.join(src, "dsp", "mnm")
        os.makedirs(mnm, exist_ok=True)
        for name in FILES:
            dst = os.path.join(mnm, name)
            pf = pack_file(name)
            if os.path.exists(dst) and sha16(dst) == sha16(pf):
                print("   = dsp/mnm/%-24s already identical" % name)
            else:
                shutil.copyfile(pf, dst)
                print("   + dsp/mnm/%-24s installed (%s…)" % (name, sha16(pf)[:8]))
        log = []
        apply_text_patches(src, log)
        for line in log:
            print(line)
    print("\nГотово. Собери плагин в своём билдере; повторный запуск безвреден.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
