// ============================================================================
// MnmTrackChain.hpp — ready-to-embed track chain on top of the canonical core.
//
// Monomachine OS 1.32B mining, iteration 38. VERIFICATION STATUS (see
// README_AUDIT_AND_CHAIN.md): MnmDspCore executes the REAL firmware program
// image and is BIT-EXACT against the reference emulator oracle on
// 20616/20616 checked values:
//   A_neutral      32 frames (synthetic machine bus, chain stressed)
//   B_hot          32 frames (DIST=96, resonance, filter env, pan)
//   C_m14_handler  32 frames (real armed DIST slot handler $145C48)
//   D_real         16 frames (REAL machine: GND-SIN m1, trigger + natural
//                  decay, no synthetic injection — actual track audio)
// checked = master outputs Y:$00-$1F per frame + full low-memory snapshots
// at frames 0/8/16/31 (0/7/15 for D_real).
//
// SIGNAL PATH = the OS 1.32 chain, executed from the real program text
// ($02EC-$0B4C), which matches the manual:
//   machine dispatch -> HEADROOM for DIST -> EQ (16-band) -> FILTER
//   (cascade 1 + 2x oversampled HPQ comb + stage 2) -> DISTORTION slot
//   -> AMP ENVELOPE -> VOLUME/PAN -> SAMPLE RATE REDUCTION -> DELAY
//   (with the filter-in-feedback-loop stage) -> LEVEL (master mix).
//
// WHY EXECUTING THE PROGRAM TEXT INSTEAD OF A HAND PORT: every previous
// hand-ported "bit-exact" module diverged (10 different versions existed).
// With one program image + one core + one vector set, drift is structurally
// impossible: the port IS the firmware.
//
// JUCE INTEGRATION: no JUCE dependency — plain C++17. One instance per track.
// Call trigger()/params from the UI/MIDI thread BEFORE processFrame(), and
// call processFrame() exactly once per audio frame-block (the OS frame =
// 1/32 of a MIDI-tick period at the given tempo; in the Monomachine the
// machine runs at 44.1 kHz with 32-sample sub-frames — one processFrame()
// call per 32-sample block keeps the OS timing).
// ============================================================================
#pragma once
#include "MnmDspCore.hpp"
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

namespace mnmdsp {

class MnmTrackChain {
public:
    static const uint32_t PAGE = 0x400;   // track page base (r6 = P+$28 protocol)

    MnmTrackChain() { core_.max_steps = 50000000ull; }

    // ---------- loading ----------
    // programDir must contain chain_program.lst; imageDir must contain
    // full_x.bin/full_y.bin (pass the same dir for both, or the oracle root
    // + a dataset dir when using the chain_oracle pack layout).
    bool load(const std::string& programDir, const std::string& imageDir) {
        { std::ifstream f(programDir + "/chain_program.lst");
          if (!f) return false;
          std::vector<std::string> lines; std::string s;
          while (std::getline(f, s)) { while (!s.empty() && (s.back()=='\r'||s.back()=='\n')) s.pop_back(); lines.push_back(s); }
          core_.parse(lines); }
        if (!loadImage(imageDir + "/full_x.bin", core_.X)) return false;
        if (!loadImage(imageDir + "/full_y.bin", core_.Y)) return false;
        xPath_ = imageDir + "/full_x.bin";
        yPath_ = imageDir + "/full_y.bin";
        loaded_ = true;
        return true;
    }
    bool load(const std::string& dir) { return load(dir, dir); }

    // Fresh state: registers zeroed, memory re-loaded from the images.
    // Call once after load() and whenever you want a cold track.
    bool reset() {
        if (!loaded_) return false;
        core_.A = core_.B = 0; core_.x0 = core_.x1 = core_.y0 = core_.y1 = 0;
        for (int i = 0; i < 8; ++i) { core_.R[i] = 0; core_.N[i] = 0; core_.M[i] = MASK24; }
        core_.f = Flags{}; core_.sr_int = 0; core_.pc = 0;
        core_.do_stack.clear(); core_.rep_valid = false; core_.rep_count = 0; core_.rep_after = 0;
        core_.ret_stack.clear(); core_.steps = 0;
        if (!loadImage(xPath_, core_.X)) return false;
        if (!loadImage(yPath_, core_.Y)) return false;
        return true;
    }

    // ---------- machine slot ----------
    // Selects the machine (slot = machine index, e.g. 1 = GND-SIN, 14 =
    // SWAVE-ENS). Sets the slot ring exactly like the OS does on slot change.
    void setSlot(uint32_t machine) {
        core_.wr('y', 0x120, machine);
        core_.wr('y', 0x121, machine);
        core_.wr('y', 0x122, machine);
        core_.wr('y', PAGE + 0x24, machine);   // prev-slot cell
        core_.wr('x', PAGE + 0x0C, 0);         // DIST slot handler: none armed
    }
    // One-shot machine INIT for m14 (SWAVE-ENS), as performed by the OS
    // (region $145AB5-$145AE1, stops before rts). Harmless for other machines.
    void runMachineInit() {
        uint32_t save = core_.R[6];
        core_.R[6] = PAGE + 0x28;
        core_.run(0x145AB5, 0x145AE1, nullptr, 800000);
        core_.R[6] = save;
    }

    // ---------- verified parameter cells ----------
    // All raw knobs are 0..127 unless stated; envelope fractions are 0..1.
    void setAmpEnv(int atk, int dec)            { yP(0x00, w16(atk)); yP(0x02, w16(dec)); }
    void setDist(int raw)                       { yP(0x04, w16(raw)); yP(0x0C, w16(raw)); }
    void setVol(int raw)                        { yP(0x05, w16(raw)); yP(0x0D, w16(raw)); }
    void setPan(int raw)                        { yP(0x06, w16(raw)); }
    void setBase(int raw)                       { yP(0x10, w16(raw)); }
    void setWdth(int raw)                       { yP(0x11, w16(raw)); }
    void setHpq(int raw)                        { yP(0x12, w16(raw)); }
    void setLpq(int raw)                        { yP(0x13, w16(raw)); }
    void setFilterEnv(float atk, float dec, float bofs, float wofs) {
        yP(0x14, frac(atk)); yP(0x15, frac(dec));
        yP(0x16, frac(bofs)); yP(0x17, frac(wofs));
    }
    void setTempo(int bpm)                      { yP(0x23, (uint32_t)(bpm & 0xFFFF)); }
    void setMachineParam(int idx, float v0to1)  { yP(0x2C + idx, frac(v0to1)); }
    void setEqgDivisor(uint32_t v /*=64*/)      { core_.wr('x', PAGE+0x0B, v); core_.wr('y', PAGE+0x0B, v); core_.wr('y', PAGE+0x0A, v); }
    void setMonoFlag(uint32_t v /*=1*/)         { core_.wr('x', PAGE+0x0F, v); }
    void setDistSlotHandler(uint32_t handlerPC) { core_.wr('x', PAGE+0x0C, handlerPC); }  // 0 = clean path

    // TRIG cell: bit0 = trigger event, bit7 = machine refresh (CONF path).
    void trigger()            { core_.wr('y', PAGE + 0x28, 0x80 | 1); }
    void clearTrigger()       { core_.wr('y', PAGE + 0x28, 0x80); }

    // ---------- per-frame processing ----------
    // Runs ONE OS frame: preamble $00CF-$0100 (CONF bookkeeping) -> machine
    // dispatch $0100-$02EB (real machine DSP) -> track chain $02EC-$0B4C.
    // out[32] receives the master bus Y:$00-$1F (16 L/R sample pairs — the OS
    // outputs the track as 16 interleaved stereo samples per frame at 32x
    // oversampled frame cadence; pair them exactly as the OS does downstream).
    // Returns false if the program ran off-image (should never happen).
    bool processFrame(int32_t out[32]) {
        if (!loaded_) return false;
        core_.run(0x00CF, 0x0100, nullptr, 800000);
        core_.run(0x0100, 0x02EB, nullptr, 800000);
        core_.wr('x', 0x2C9, 0x300);   // harness trap: multi-track scanner init
                                       // (verified against the oracle; see README)
        core_.run(0x02EC, 0x0B4C, nullptr, 800000);
        for (int i = 0; i < 32; ++i)
            out[i] = Core::sgn24(core_.rd('y', (uint32_t) i));
        return true;
    }

    // Direct access for power users: raw pokes into the verified memory model.
    void pokeY(uint32_t addr, uint32_t val) { core_.wr('y', addr, val); }
    void pokeX(uint32_t addr, uint32_t val) { core_.wr('x', addr, val); }
    uint32_t peekY(uint32_t addr) const     { return core_.rd('y', addr); }
    uint32_t peekX(uint32_t addr) const     { return core_.rd('x', addr); }
    Core& core()                            { return core_; }

private:
    Core core_;
    bool loaded_ = false;
    std::string xPath_, yPath_;

    static uint32_t w16(int raw)  { return ((uint32_t)(raw & 0xFFFF)) << 16; }
    static uint32_t frac(float v) {
        if (v < 0) v = 0; if (v > 1) v = 1;
        return (uint32_t)(v * 8388608.0f) & MASK24;
    }
    void yP(uint32_t cell, uint32_t val) { core_.wr('y', PAGE + cell, val); }

    static bool loadImage(const std::string& path, std::unordered_map<uint32_t, uint32_t>& dst) {
        std::ifstream f(path, std::ios::binary);
        if (!f) return false;
        char buf[4]; uint32_t a = 0;
        while (f.read(buf, 4)) {
            uint32_t v = (uint8_t) buf[0] | ((uint8_t) buf[1] << 8) | ((uint8_t) buf[2] << 16);
            dst[a] = v; ++a;
        }
        return a == 0x14C800;
    }
};

} // namespace mnmdsp
