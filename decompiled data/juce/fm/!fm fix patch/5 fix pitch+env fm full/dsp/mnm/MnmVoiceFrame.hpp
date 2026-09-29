// =============================================================================
// MnmVoiceFrame.hpp — full voice frame of the Monomachine OS 1.32B track:
// prologue + pitch chain (transliterated kernel code) -> native bit-exact FM
// machine (pack 3/4/7 cores) -> filter tail $02EC-$0B4C (transliterated):
//   env2 -> history copy -> SVF cascade-1 (halfband 2x) -> EQ -> AMP env ->
//   gain ring (VOL^2 * pan, 16-step ramp) -> HPQ/LPQ resonance -> track delay
//   -> filter-envelope stage 2 -> output.
//
// Verification: 155 sets / 932 frames against the bit-exact DSP56300 emulator
// running the real OS kernel (see PROOF_TAIL_2026-09-29.md).
//
// Knob cells (absolute, r6 = $400 in the tail):
//   $404 EQF | x$40B EQG | $405 VOL | $406 PAN | $408 BASE | $409 HPQ
//   $40C FILT ATK | $40D FILT DEC | $40E WDTH | $410/$411 offsets
//   $413 LPQ | $414-$417 stage-2 ATK/DEC/BOFS/WOFS | $418-$41B env2
//   $428 TRIG (0/1/2/3) | $423 TEMPO | $429 pitch word | $42A tick
// Machine knobs: $42C-$433 (raw<<16), TUNE = $433.
// =============================================================================
#pragma once
#include <cstdint>
#include <cstring>
#include <cstdio>
#include <vector>
#include <string>

#include "mnm_dsp_kernel.hpp"
#include "mnm_voice_frame_code.h"
#include "MnmFmStat.hpp"     // native bit-exact FM STAT core (pack 3/7)

namespace mnm {

static const uint32_t PAGE = 0x400;          // tail page base (r6 in tail)
static const uint32_t MPAGE = 0x428;         // machine page base (r6 in PROC)

class VoiceFrame {
public:
    DSPKernel k;
    mnmfm::MnmFmStat fm;

    VoiceFrame() {
        // load the transliterated instruction stream
        for (int i = 0; i < kVoiceFrameCodeN; ++i)
            k.parseLine(kVoiceFrameCode[i].pc, kVoiceFrameCode[i].text);
        k.buildNextAddr();
        // native machine hooks (the FM machine body is not transliterated:
        // the dispatch jsr lands on the proven native core)
        k.onProc = [this](DSPKernel& kk) -> uint32_t { return hookProc(kk); };
        k.onConf = [this](DSPKernel& kk) -> uint32_t { return hookConf(kk); };
        k.onInit = [this](DSPKernel& kk) -> uint32_t { return hookInit(kk); };
        k.procAddr = 0x145D21;
        k.confAddr = 0x145D1D;
        k.initAddr = 0x145D12;
        k.nullAddr = 0x145D1C;
    }

    // ---- state load (seed = post-INIT/CONF frame-0 input) ------------------
    void loadSeed(const std::vector<std::pair<uint32_t, uint32_t>>& xy) {
        // xy: packed (addr, value) with addr = side*0x1000000 | cell
        for (const auto& p : xy) {
            uint32_t a = p.first & 0xFFFFFF;
            uint32_t v = p.second & 0xFFFFFF;
            if (p.first & 0x1000000) k.wr("y", a, v);
            else k.wr("x", a, v);
        }
    }
    void setWord(const char* space, uint32_t a, uint32_t v) { k.wr(space, a, v); }
    uint32_t getWord(const char* space, uint32_t a) { return k.rd(space, a); }

    // ---- machine dispatch pointer patches (beyond the pmem image) ----------
    void patchDispatch() {
        const uint32_t MIR_INIT = 0x1001AF + 0x10016B;
        const uint32_t MIR_CONF = 0x1001AF + 0x10018D;
        const uint32_t MIR_PROC = 0x1001AF + 0x1001AF;
        k.wr("x", MIR_PROC + 8, 0x145D21);      // FM STAT PROC
        k.wr("x", MIR_CONF + 8, 0x145D1D);      // FM STAT CONF
        k.wr("x", MIR_INIT + 8, 0x145D12);      // FM STAT INIT
        k.wr("x", MIR_PROC + 0, 0x145D1C);      // null machine (rts)
        k.wr("x", MIR_CONF + 0, 0x145D1C);
        k.wr("x", MIR_INIT + 0, 0x145D1C);
    }

    // ---- native machine contract -------------------------------------------
    // (pack-7 harness: INIT/CONF run explicitly, proc via the kernel jsr)
    void machineInit() {
        fm.init();
        // kernel-side effects of the firmware INIT: (r6=$428) pointers +
        // wiped machine state; the seed already carries the exact state,
        // so this is only used for fresh (non-seed) setups.
    }
    void machineConf() {
        uint32_t knob[8];
        for (int i = 0; i < 8; ++i)
            knob[i] = k.rd("y", MPAGE + 0x04 + i) >> 16;   // raw 0..127
        fm.conf(knob);
    }

    uint32_t hookProc(DSPKernel& kk) {
        // machine contract at PROC entry (pack-7 harness hook):
        // linear M regs, r6 = machine page, r7 = $100
        for (int r = 0; r < 8; ++r) kk.M[r] = 0xFFFFFF;
        kk.R[6] = MPAGE;
        kk.R[7] = 0x100;
        // machine reads its scratch/page state: sync kernel -> core
        for (int i = 0; i < 0x100; ++i) {
            fm.mem.X[i] = kk.rd("x", i);
            fm.mem.Y[i] = kk.rd("y", i);
        }
        for (int j = 0; j < 0x40; ++j)
            fm.mem.st[j] = kk.rd("y", MPAGE + j);
        uint64_t pitchA = uint64_t(kk.A & 0xFFFFFFFFFFFFull);
        uint32_t out[32];
        fm.proc(pitchA, out);
        // machine writes scratch ($00-$0FF), page ($428-$467) and the
        // 32 output words ($100-$121): sync core -> kernel
        for (int i = 0; i < 0x100; ++i) {
            kk.wr("x", i, fm.mem.X[i]);
            kk.wr("y", i, fm.mem.Y[i]);
        }
        for (int j = 0; j < 0x40; ++j)
            kk.wr("y", MPAGE + j, fm.mem.st[j]);
        for (int i = 0; i < 32; ++i)
            kk.wr("y", 0x100 + i, out[i]);
        // rts
        if (kk.retStack.empty()) return 0;
        uint32_t t = kk.retStack.back();
        kk.retStack.pop_back();
        return t;
    }
    uint32_t hookConf(DSPKernel&) {
        machineConf();
        if (k.retStack.empty()) return 0;
        uint32_t t = k.retStack.back();
        k.retStack.pop_back();
        return t;
    }
    uint32_t hookInit(DSPKernel&) {
        machineInit();
        if (k.retStack.empty()) return 0;
        uint32_t t = k.retStack.back();
        k.retStack.pop_back();
        return t;
    }

    // ---- one frame ----------------------------------------------------------
    void frame(uint32_t trig) {
        machineConf();               // CONF per frame, BEFORE the frame
                                     // (harness contract, idempotent)
        k.wr("y", PAGE + 0x28, trig & 0xFFFFFF);
        k.run(0x0100, 0x02EC);
        k.wr("x", 0x2C9, 0x300);     // harness: pin the master write pointer
        k.run(0x02EC, 0x0B4C);
    }

    // ---- sync the native FM core from the kernel state (seed continuation) --
    void syncCoreFromKernel() {
        for (int i = 0; i < 0x100; ++i) {
            fm.mem.X[i] = k.rd("x", i);
            fm.mem.Y[i] = k.rd("y", i);
        }
        for (int j = 0; j < 0x40; ++j)
            fm.mem.st[j] = k.rd("y", MPAGE + j);
    }

    // ---- convenience knob access (raw 0..127, written raw<<16) --------------
    void setKnob(uint32_t cell, uint32_t raw) {
        k.wr("y", cell, (raw & 0xFFFF) << 16);
    }
    void setKnobX(uint32_t cell, uint32_t raw) {
        k.wr("x", cell, (raw & 0xFFFF) << 16);
    }
    // stereo output of the frame: the tail leaves the ramped L/R in the
    // gain ring (Y$20-$3F) and the master block receives via the OS routing;
    // for the plugin the wrapper exposes the ring directly.
    void readRing(int32_t* ring32) const {
        for (int i = 0; i < 32; ++i) {
            uint32_t v = k.Y[0x20 + i];
            ring32[i] = (v & 0x800000) ? int32_t(v) - 0x1000000 : int32_t(v);
        }
    }
};

} // namespace mnm
