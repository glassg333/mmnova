#!/usr/bin/env python3
"""Create deterministic stereo sources for Scintillate knob tests."""

import argparse
import math
import os
import random
import struct
import wave


SR = 48_000
TAU = math.tau


def clamp(value, low=-1.0, high=1.0):
    return max(low, min(high, value))


def smooth(value):
    value = max(0.0, min(1.0, value))
    return value * value * (3.0 - 2.0 * value)


def write_wav(path, frames):
    peak = max(1.0e-9, max(max(abs(left), abs(right)) for left, right in frames))
    gain = min(0.82 / peak, 1.0)
    with wave.open(path, "wb") as output:
        output.setnchannels(2)
        output.setsampwidth(2)
        output.setframerate(SR)
        packed = bytearray()
        for left, right in frames:
            left = math.tanh(left * gain * 1.35) / math.tanh(1.35)
            right = math.tanh(right * gain * 1.35) / math.tanh(1.35)
            packed.extend(struct.pack("<hh", int(clamp(left) * 32767), int(clamp(right) * 32767)))
        output.writeframes(packed)


def render_full():
    duration = 50.0
    total = int(duration * SR)
    section_length = 10.0
    phases = [0.0] * 40
    rng = random.Random(91273)
    frames = []

    for index in range(total):
        t = index / SR
        section = min(4, int(t / section_length))
        local = t - section * section_length
        root = 88.0 * math.pow(2.0, 0.34 * math.sin(TAU * t / 17.0))
        if section == 0:
            # Bright harmonic material: a controlled fundamental with moving partials.
            mono = 0.0
            for harmonic in range(1, 15):
                frequency = root * harmonic * (1.0 + 0.006 * math.sin(t * 0.31 + harmonic))
                phases[harmonic - 1] = (phases[harmonic - 1] + TAU * frequency / SR) % TAU
                envelope = (1.0 / math.pow(harmonic, 0.78)) * (0.70 + 0.30 * math.sin(t * 0.43 + harmonic))
                mono += math.sin(phases[harmonic - 1]) * envelope
            side = 0.16 * math.sin(TAU * root * 1.003 * t + 0.8)
        elif section == 1:
            # Inharmonic metallic cluster with slow FM and non-integer ratios.
            ratios = (1.0, 1.4142, 1.7321, 2.2361, 2.7183, 3.1416, 3.6055, 4.2716)
            mono = 0.0
            for offset, ratio in enumerate(ratios):
                mod = 1.0 + 0.035 * math.sin(TAU * (0.09 + offset * 0.013) * t)
                frequency = root * ratio * mod
                phases[15 + offset] = (phases[15 + offset] + TAU * frequency / SR) % TAU
                mono += math.sin(phases[15 + offset] + 2.2 * math.sin(TAU * 0.17 * t + offset)) / math.sqrt(offset + 1.0)
            side = 0.28 * math.sin(TAU * root * 1.73 * t + math.sin(t * 0.7))
        elif section == 2:
            # Vocal-like formant sweep: saw harmonics weighted by three moving formants.
            pitch = 126.0 * math.pow(2.0, 0.08 * math.sin(TAU * local / 6.0))
            formants = (430.0 + 150.0 * math.sin(t * 0.31),
                        1150.0 + 340.0 * math.sin(t * 0.23 + 1.0),
                        2450.0 + 500.0 * math.sin(t * 0.17 + 2.0))
            mono = 0.0
            for harmonic in range(1, 24):
                frequency = pitch * harmonic
                phases[25 + harmonic % 8] = (phases[25 + harmonic % 8] + TAU * frequency / SR) % TAU
                weight = sum(math.exp(-((frequency - f) / (f * 0.16)) ** 2) for f in formants)
                mono += math.sin(phases[25 + harmonic % 8]) * weight / math.pow(harmonic, 0.92)
            breath = (rng.random() * 2.0 - 1.0) * (0.04 + 0.04 * math.sin(t * 0.5) ** 2)
            mono += breath
            side = 0.12 * math.sin(TAU * pitch * 2.01 * t)
        elif section == 3:
            # Transients, chirps and noise expose the rate/decay behaviour.
            beat = local % 1.0
            half = local % 0.5
            kick = math.sin(TAU * (48.0 + 110.0 * math.exp(-beat * 18.0)) * beat) * math.exp(-beat * 9.0)
            snare = (rng.random() * 2.0 - 1.0) * math.exp(-((beat - 0.5) * 30.0) ** 2) * 0.75
            hat = (rng.random() * 2.0 - 1.0) * math.exp(-half * 75.0) * 0.22
            chirp = math.sin(TAU * (350.0 + 3200.0 * half) * half) * math.exp(-half * 19.0)
            mono = 0.72 * kick + 0.48 * snare + hat + 0.28 * chirp
            side = 0.35 * chirp * math.sin(TAU * 0.25 * local)
        else:
            # Hybrid climax: harmonic body, metallic upper band and gated bursts.
            mono = 0.0
            for harmonic in range(1, 19):
                frequency = root * harmonic * (1.0 + 0.02 * math.sin(t * 0.8 + harmonic))
                phases[harmonic - 1] = (phases[harmonic - 1] + TAU * frequency / SR) % TAU
                mono += math.sin(phases[harmonic - 1]) / math.pow(harmonic, 0.84)
            for offset, ratio in enumerate((1.37, 1.91, 2.63, 3.77, 5.11)):
                frequency = root * ratio * (1.0 + 0.06 * math.sin(t * 0.37 + offset))
                phases[15 + offset] = (phases[15 + offset] + TAU * frequency / SR) % TAU
                mono += 0.33 * math.sin(phases[15 + offset] + math.sin(t * 2.0)) / (offset + 1.0)
            burst = math.sin(TAU * (180.0 + 1800.0 * (local % 1.25)) * (local % 1.25))
            mono += burst * math.exp(-((local % 1.25) * 14.0)) * 0.32
            side = 0.24 * math.sin(TAU * root * 2.37 * t + 1.2)

        fade = smooth(min(1.0, local / 0.22)) * smooth(min(1.0, (section_length - local) / 0.22))
        mono *= fade * (0.72 + 0.20 * math.sin(t * 0.19) ** 2)
        frames.append((mono + side, mono - side))

    return frames


def render_impulses():
    duration = 12.0
    total = int(duration * SR)
    phases = [0.0] * 18
    frames = []
    for index in range(total):
        t = index / SR
        position = t % 2.0
        burst = math.exp(-position * 26.0)
        root = 92.0 + 28.0 * math.sin(t * 0.4)
        body = 0.0
        for harmonic in range(1, 18):
            frequency = root * harmonic * (1.0 + 0.018 * math.sin(t * 0.7 + harmonic))
            phases[harmonic % len(phases)] = (phases[harmonic % len(phases)] + TAU * frequency / SR) % TAU
            body += math.sin(phases[harmonic % len(phases)]) / math.pow(harmonic, 0.84)
        chirp = math.sin(TAU * (120.0 + 5000.0 * position) * position) * math.exp(-position * 18.0)
        mono = burst * (0.95 * body + 0.35 * chirp)
        pan = 0.5 + 0.38 * math.sin(t * 0.71)
        frames.append((mono * (1.0 - pan), mono * pan))
    return frames


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--out-dir", default=os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
                        )
    args = parser.parse_args()
    os.makedirs(args.out_dir, exist_ok=True)
    write_wav(os.path.join(args.out_dir, "scintillate_test_source_50s.wav"), render_full())
    write_wav(os.path.join(args.out_dir, "scintillate_test_impulses_12s.wav"), render_impulses())


if __name__ == "__main__":
    main()
