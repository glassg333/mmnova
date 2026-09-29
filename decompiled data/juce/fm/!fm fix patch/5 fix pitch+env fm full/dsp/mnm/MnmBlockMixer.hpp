// =============================================================================
// MnmBlockMixer.hpp — block-level ROUTING/MIXER of the Monomachine OS 1.32B
// (pack 9): the OS audio block loop around the pack-8 voice frame.
//
// Architecture (proven against the live kernel, see PROOF_ROUTING):
//   * The OS runs 2 DSPs x 3 sub-blocks per audio block; sub-block k = one
//     track (machine slot y:$120+k, page y:$123 = $528+$100k, 16 samples).
//   * The host header word (HRX) selects the buffer layout:
//       $180 -> buses $140/$1A0/$200, work $260, echo send $2CC
//       $160 -> buses $180/$1E0/$240, work $2A0, echo send $2FC
//       $140 -> buses $160/$1C0/$220, work $280, echo send $32C
//     (each bus = $60 words = 3 sub-blocks x 32 words).
//   * Per-track routing flags live at y:(page-$3) (= page+$25 of the tail
//     page), loaded into X:$2C4 each sub-block ($00CF-$00DD):
//       bit0/1/2 = add the frame bus Y:$0000-$1F into bus 1/2/3
//       bit3 + bits C/D/E = replace semantics (UNUSABLE: the branch path
//         reaches func_0001e2/0001ec without bsr, their rts pops an empty
//         system stack on real silicon; the host never sets bit3)
//       bit6/7 = codec input path (mono-dup / stereo copy into the work
//         region X:$2C3)
//       bit8 = end of block: sum buses 1+2+3 into bus 1 (MIX)
//       bitA = end of block: copy the X:$2CA/$2CB regions into the buses
//   * The frame tail (pack-8 code, T5/T6) produces the 32 output words into
//     Y:$0000-$1F, 16 echo-send words through X:(X:$2C9)+ and the host
//     stream through X:(X:$FF)+.
//
// Transliterated segments (mnm_block_loop_code.h):
//   $0087-$00CE mode tables | $00CF-$00FF per-track prologue (flags ->
//   X:$2C4, INIT dispatch $10016B, CONF $10018D) | $0143-$0163 mixer
//   (func_0001e2 add) | $01A3-$01E1 end of block (MIX $01AD / copy $01C2).
// The FM machine body is the native bit-exact core (packs 3/7/8).
// =============================================================================
#pragma once
#include <cstdint>
#include <vector>

#include "mnm_dsp_kernel.hpp"
#include "mnm_voice_frame_code.h"
#include "mnm_block_loop_code.h"
#include "MnmFmStat.hpp"

namespace mnm {

class BlockMixer {
public:
    DSPKernel k;
    mnmfm::MnmFmStat fm;

    BlockMixer() {
        for (int i = 0; i < kVoiceFrameCodeN; ++i)
            k.parseLine(kVoiceFrameCode[i].pc, kVoiceFrameCode[i].text);
        for (int i = 0; i < kBlockLoopCodeN; ++i)
            k.parseLine(kBlockLoopCode[i].pc, kBlockLoopCode[i].text);
        k.buildNextAddr();
        k.onProc = [this](DSPKernel& kk) -> uint32_t { return hookProc(kk); };
        k.onConf = [this](DSPKernel& kk) -> uint32_t { return hookConf(kk); };
        k.onInit = [this](DSPKernel& kk) -> uint32_t { return hookInit(kk); };
        k.procAddr = 0x145D21;
        k.confAddr = 0x145D1D;
        k.initAddr = 0x145D12;
        k.nullAddr = 0x145D1C;
    }

    // ---- dispatch pointer patches (beyond the pmem image / seed window) ----
    void patchDispatch() {
        const uint32_t MIR_INIT = 0x1001AF + 0x10016B;
        const uint32_t MIR_CONF = 0x1001AF + 0x10018D;
        const uint32_t MIR_PROC = 0x1001AF + 0x1001AF;
        for (int slot = 0; slot < 3; ++slot) {
            // slot 8 = FM STAT, slots 1..7/9+ unassigned -> null machine
            uint32_t off = (slot == 0) ? 8u : 0u;
            k.wr("x", MIR_PROC + off, off == 8 ? 0x145D21 : 0x145D1C);
            k.wr("x", MIR_CONF + off, off == 8 ? 0x145D1D : 0x145D1C);
            k.wr("x", MIR_INIT + off, off == 8 ? 0x145D12 : 0x145D1C);
        }
    }

    // ---- state load (seed = pre-block memory, X/Y $000-$700) ---------------
    void loadSeed(const std::vector<std::pair<uint32_t, uint32_t>>& xy) {
        for (const auto& p : xy) {
            uint32_t a = p.first & 0xFFFFFF;
            uint32_t v = p.second & 0xFFFFFF;
            if (p.first & 0x1000000) k.wr("y", a, v);
            else k.wr("x", a, v);
        }
    }
    void setWord(const char* space, uint32_t a, uint32_t v) { k.wr(space, a, v); }
    uint32_t getWord(const char* space, uint32_t a) { return k.rd(space, a); }

    // ---- OS block flow ------------------------------------------------------
    // S1: header poll + mode tables ($0087-$00CE). HRX = the host header.
    void runHeader(uint32_t hrx) {
        k.wr("x", 0xFFFFEB, hrx);
        k.run(0x0087, 0x00CF);
    }
    // One sub-block: prologue (flags -> X:$2C4, INIT/CONF dispatch) + the
    // pack-8 voice frame + the mixer ($0143-$0163).
    void runSubBlock(int kidx) {
        k.wr("y", 0x123, 0x528 + 0x100 * uint32_t(kidx));  // y:$123 (OS advance)
        k.wr("y", 0x124, uint32_t(kidx));                  // y:$124 (OS counter)
        k.run(0x00CF, 0x0100);    // prologue: X:$2C4 = y:(page-$3), INIT, CONF
        k.run(0x0100, 0x02EC);    // frame pre: input path, func_000262, PROC
        k.run(0x02EC, 0x0B4C);    // frame tail (pack-8) incl. T5/T6
        k.run(0x0143, 0x0163);    // mixer: add Y:$0000-$1F into buses per flags
    }
    // End of block ($01A3-$01E1): the mailbox reset always runs, then the
    // flag dispatch picks MIX (bit8) / copy (bitA) / neither; every path
    // ends with jmp $0087, so running to $0087 terminates naturally.
    void runEnd() {
        k.run(0x01A3, 0x0087);
    }

private:
    // ---- native machine contract (dynamic page: mp = y:$123) ---------------
    static uint32_t popRet(DSPKernel& kk) {
        if (kk.retStack.empty()) return 0;
        uint32_t t = kk.retStack.back();
        kk.retStack.pop_back();
        return t;
    }
    uint32_t hookInit(DSPKernel& kk) {
        uint32_t mp = kk.rd("y", 0x123);
        // firmware INIT body ($145D12-$145D1C) with r6 = mp:
        for (int i = 0; i < 24; ++i) kk.wr("y", mp + 0x11 + uint32_t(i), 0);
        kk.wr("y", mp + 0x10, 0);
        kk.wr("y", mp + 0x14, 0);
        kk.wr("y", mp + 0x16, 0);
        fm.init();
        return popRet(kk);
    }
    uint32_t hookConf(DSPKernel& kk) {
        uint32_t mp = kk.rd("y", 0x123);
        // firmware CONF body ($145D1D-$145D20):
        kk.wr("y", mp + 0x2A, 0x7FFFFF);
        uint32_t knob[8];
        for (int i = 0; i < 8; ++i)
            knob[i] = kk.rd("y", mp + 0x04 + uint32_t(i)) >> 16;  // raw 0..127
        fm.conf(knob);
        return popRet(kk);
    }
    uint32_t hookProc(DSPKernel& kk) {
        uint32_t mp = kk.rd("y", 0x123);
        for (int r = 0; r < 8; ++r) kk.M[r] = 0xFFFFFF;
        kk.R[6] = mp;
        kk.R[7] = 0x100;
        for (int i = 0; i < 0x100; ++i) {
            fm.mem.X[i] = kk.rd("x", uint32_t(i));
            fm.mem.Y[i] = kk.rd("y", uint32_t(i));
        }
        for (int j = 0; j < 0x40; ++j)
            fm.mem.st[j] = kk.rd("y", mp + uint32_t(j));
        uint64_t pitchA = uint64_t(kk.A & 0xFFFFFFFFFFFFull);
        uint32_t out[32];
        fm.proc(pitchA, out);
        for (int i = 0; i < 0x100; ++i) {
            kk.wr("x", uint32_t(i), fm.mem.X[i]);
            kk.wr("y", uint32_t(i), fm.mem.Y[i]);
        }
        for (int j = 0; j < 0x40; ++j)
            kk.wr("y", mp + uint32_t(j), fm.mem.st[j]);
        for (int i = 0; i < 32; ++i)
            kk.wr("y", 0x100 + uint32_t(i), out[i]);
        // The real machine's rts leaves its accumulators in the CPU: the
        // frame code carries B (e.g. into the $04FA history copy) and A.
        // The core keeps 48-bit accumulators; the kernel's are 56-bit SIGNED,
        // so the 48-bit value must be sign-extended (sext48) — otherwise the
        // data-limit checker sees a huge positive value and saturates.
        auto sext48 = [](uint64_t v) -> int64_t {
            v &= 0xFFFFFFFFFFFFull;
            return (int64_t)((v ^ (1ull << 47)) - (1ull << 47));
        };
        kk.A = sext48(fm.A);
        kk.B = sext48(fm.B);
        return popRet(kk);
    }
};

} // namespace mnm
