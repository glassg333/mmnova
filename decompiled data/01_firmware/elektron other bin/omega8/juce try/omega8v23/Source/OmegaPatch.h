// Omega8Map.h — decoded parameter map of Studio Electronics Omega 8 / Omega CODE
// 176-byte patch format. Offsets verified against factory banks + Omega/CODE
// editor screenshot (REVELATION edit buffer): 78/78 checked values matched.
#pragma once
#include <array>
#include <cstdint>
#include <cstring>
#include <vector>
#include <string>

namespace omega8
{

constexpr int kPatchSize    = 176;
constexpr int kParamBytes   = 160;   // 0..159 are parameters, 160..175 name
// r18.7: 32 голоса — MULTI-слой: до 8 активных parts на одну ноту (8x4)
constexpr int kNumVoices    = 32;
constexpr int kArrRate      = 136;   // 8 bytes
constexpr int kArrPos       = 144;   // 8 bytes, 0x40 = centre
constexpr int kArrDepth     = 152;   // 8 bytes
constexpr int kNameOffs     = 160;

// Destination lists are 1-based; 0 = OFF (verified: ENV3 dest 16 == LF1R).
enum ContDest : int
{
    D_OFF = 0, D_FRE1, D_FRE2, D_12F, D_LEV1, D_LEV2, D_PW1, D_PW2, D_12P,
    D_FILT, D_RESO, D_LEVN, D_XMOD, D_EA1, D_EA3, D_EXT,
    D_LF1R, D_LF2R, D_LF1D, D_LF2D, D_12D, D_12R, D_PAND, D_PANR, D_PAN,
    D_COUNT
};

enum FilterType : int
{
    F_SEM_LP = 0, F_SEM_BP, F_SEM_HP, F_SEM_BR,
    F_MINI, F_AUX1, F_AUX2          // AUX1 = Oberheim, AUX2 = CS80 (HP+LP)
};

// ---------------------------------------------------------------------------
// Byte index table (names are documentation; engine reads by offset).
namespace off
{
constexpr int GLIDE_TIME = 0, GLIDE_FLAGS = 1, OCTAVE = 2, ENV3_AMT = 3,
    SUB_WAVE = 4, PRIOR = 7,
    LFO1_RATE = 8, LFO1_WAVSYNC = 9,
    LFO1_DEPTH1 = 10, LFO1_DEPTH2 = 11, LFO1_DEPTH3 = 12,
    LFO1_DEST1 = 13, LFO1_DEST2 = 14, LFO1_DEST3 = 15,
    UNISON = 16, VOICE_QTY = 17, VMODE = 19, MTRG = 20,
    LFO2_RATE = 21, LFO2_WAVSYNC = 22,
    LFO2_DEPTH1 = 23, LFO2_DEPTH2 = 24, LFO2_DEPTH3 = 25,
    LFO2_DEST1 = 26, LFO2_DEST2 = 27, LFO2_DEST3 = 28,
    OSC1_FREQ = 30, OSC2_FREQ = 31, VOLUME = 32, UNK36 = 36,
    PWM1 = 34, PWM2 = 35,
    OSC1_LEVEL = 37, OSC2_LEVEL = 38, NOISE_LEVEL = 39,
    HPF = 40, XMOD_DPTH = 41, XMOD_DEST = 42,
    FILT_TYPE = 43, CUTOFF = 44, RESO = 45, TRACKING = 46, ENV1_AMT = 47,
    DLY1 = 48,
    ENV1_ATK = 49, ENV1_DEC = 50, ENV1_DK2 = 51, ENV1_SUS = 52, ENV1_REL = 53,
    ENV2_ATK = 54, ENV2_DEC = 55, ENV2_DK2 = 56, ENV2_SUS = 57, ENV2_REL = 58,
    ENV3_ATK = 59, ENV3_DEC = 60, ENV3_DK2 = 61, ENV3_SUS = 62, ENV3_REL = 63,
    ENV3_DEST1 = 64, ENV3_DEST2 = 65, ENV3_DEST3 = 66,
    ENV3_AMT1 = 67, ENV3_AMT2 = 68, ENV3_AMT3 = 69,
    INVT = 72, WAVE_BITS = 74, WAVE2 = 75,
    EXT_IN = 76, MASTER_TUNE = 77, OSC2_FINE = 78, HPR = 79, WIN = 80,
    MODW_D1 = 86, MODW_D2 = 87, MODW_A1 = 88, MODW_A2 = 89,
    DYN_D1 = 90, DYN_D2 = 91, DYN_A1 = 92, DYN_A2 = 93,
    BEND_D1 = 94, BEND_D2 = 95, BEND_A1 = 96, BEND_A2 = 97,
    PRES_D1 = 98, PRES_D2 = 99, PRES_A1 = 100, PRES_A2 = 101,
    C1_D1 = 102, C1_D2 = 103, C1_A1 = 104, C1_A2 = 105,
    C2_D1 = 106, C2_D2 = 107, C2_A1 = 108, C2_A2 = 109,
    DYN1 = 112, DYN2 = 113, DYN3 = 114, OSC2_MODE = 115,
    PAN_SYNC = 116, DLY2 = 117, DLY3 = 118, PAN_KEY = 119, PAN_WAVE = 120,
    // r18.7: LFO edit page 2/3 (мануал: [WAVE][MODE mono/poly][SYNC midi clock] +
    // [KEY key trigger][QUAN quantized LFO]) — свободные байты 121..128.
    // sync: 0=SELF 1=DOWN 2=UP (midi clock — в VST индикатор, host-часы не берём);
    // key:  0=SELF 1=DN 2=UP 3=ALL (сброс фазы LFO по клавишам);
    // mode: 0=MONO 1=POLY (per-voice фаза); quan: 0=OFF 1=ON (глубина квантуется 1/12).
    LFO1_SYNC = 121, LFO1_KEY = 122, LFO1_MODE = 123, LFO1_QUAN = 124,
    LFO2_SYNC = 125, LFO2_KEY = 126, LFO2_MODE = 127, LFO2_QUAN = 128;
}

// ---------------------------------------------------------------------------
struct Patch
{
    std::array<uint8_t, kPatchSize> raw {};

    uint8_t  at (int i) const noexcept { return raw[(size_t) i]; }
    int      i  (int i_) const noexcept { return raw[(size_t) i_]; }
    // stored 64-centre
    double   c64 (int o) const noexcept { return (int) raw[(size_t) o] - 64.0; }
    // bipolar display (stored = display + 64)
    int      s64 (int o) const noexcept { return (int) raw[(size_t) o] - 64; }

    void setName (const char* txt) noexcept
    {
        raw.fill(' ');
        if (txt != nullptr)
        {
            std::memset (&raw[kNameOffs], ' ', 16);
            size_t n = std::strlen (txt);
            if (n > 16) n = 16;
            std::memcpy (&raw[kNameOffs], txt, n);
        }
    }

    std::string name() const
    {
        // Только печатный ASCII: сырые байты >=128 давали мусор в UTF-8
        // («японские иероглифы» на LCD) — вырезаем всё не-ASCII.
        char buf[17];
        std::memcpy (buf, &raw[kNameOffs], 16);
        buf[16] = 0;
        std::string s;
        s.reserve (16);
        for (int i = 0; i < 16; ++i)
        {
            unsigned char c = (unsigned char) buf[i];
            if (c == 0) break;                                    // конец имени
            s.push_back ((c >= 32 && c <= 126) ? (char) c : ' '); // печатный ASCII
        }
        while (! s.empty() && s.back() == ' ') s.pop_back();
        while (! s.empty() && s.front() == ' ') s.erase (s.begin());
        return s;
    }

    static Patch init() noexcept
    {
        Patch p;
        p.raw.fill (0);
        // sensible INIT defaults (Omega-like)
        p.raw[off::OSC1_FREQ]   = 32;  p.raw[off::OSC2_FREQ]  = 32;
        p.raw[off::OSC1_LEVEL]  = 127; p.raw[off::OSC2_LEVEL] = 100;
        p.raw[off::VOLUME]      = 100;
        p.raw[off::PWM1]        = 64;  p.raw[off::PWM2]       = 64;
        p.raw[off::CUTOFF]      = 100; p.raw[off::RESO]       = 40;
        p.raw[off::TRACKING]    = 90;
        p.raw[off::ENV1_AMT]    = 40;
        p.raw[off::ENV2_SUS]    = 127;
        p.raw[off::ENV3_REL]    = 60;
        p.raw[off::MASTER_TUNE] = 64;
        p.raw[off::EXT_IN]      = 64;
        p.raw[off::HPR]         = 31;  p.raw[off::WIN]        = 63;
        p.raw[off::OCTAVE]      = 64;
        p.raw[off::WAVE_BITS]   = 0x03;                // tri+saw osc1
        p.raw[off::OSC2_MODE]   = 4;                   // NORM
        p.raw[off::DYN2]        = 127;
        p.raw[off::MODW_D1]     = 9;  p.raw[off::MODW_A1] = 64;   // wheel→filter
        for (int v = 0; v < 8; ++v)
        {
            p.raw[kArrPos  + v] = 64;
            p.raw[kArrDepth+ v] = 0;
            p.raw[kArrRate + v] = 40;
        }
        p.setName ("INIT OMEGA");
        return p;
    }

    // ---- morphing / BLEND (automatable parameter) ------------------------
    // r16: в v10..v15 морф делался простым лерпом БАЙТОВ — у enum/флаговых
    // байтов получались мусорные промежуточные значения (например, TYPE между
    // SEM LP (0) и AUX1 (5) = SEM HP (2), WAVE_BITS — случайные биты, OCTAVE —
    // чужие нибблы). Поэтому «бленд стал работать хуже»: середина морфа =
    // битый патч. Теперь: непрерывные байты — лерп, enum/флаги — снап на 50%,
    // имя — целиком от доминирующей стороны (иначе лерп имени = кракозябры).
    static bool isEnumByte (int k) noexcept
    {
        switch (k)
        {
            case off::GLIDE_FLAGS: case off::OCTAVE: case off::SUB_WAVE:
            case off::PRIOR:
            case off::LFO1_WAVSYNC: case off::LFO1_DEST1:
            case off::LFO1_DEST2:   case off::LFO1_DEST3:
            case off::VMODE: case off::MTRG:
            case off::LFO2_WAVSYNC: case off::LFO2_DEST1:
            case off::LFO2_DEST2:   case off::LFO2_DEST3:
            case off::XMOD_DEST: case off::FILT_TYPE:
            case off::ENV3_DEST1:  case off::ENV3_DEST2: case off::ENV3_DEST3:
            case off::WAVE_BITS:   case off::WAVE2:
            case off::MODW_D1:  case off::MODW_D2:
            case off::DYN_D1:   case off::DYN_D2:
            case off::BEND_D1:  case off::BEND_D2:
            case off::PRES_D1:  case off::PRES_D2:
            case off::C1_D1:    case off::C1_D2:
            case off::C2_D1:    case off::C2_D2:
            case off::OSC2_MODE: case off::PAN_SYNC: case off::PAN_KEY:
            case off::PAN_WAVE:
                return true;
            default: return false;
        }
    }

    static Patch lerp (const Patch& a, const Patch& b, float t) noexcept
    {
        if (t <= 0.0f) return a;
        if (t >= 1.0f) return b;
        const bool pickB = t >= 0.5f;
        Patch o;
        for (int k = 0; k < kPatchSize; ++k)
        {
            if (k >= kParamBytes)              // имя: без «лерпа кракозябр»
            { o.raw[(size_t) k] = pickB ? b.raw[(size_t) k] : a.raw[(size_t) k]; continue; }
            if (isEnumByte (k))
            { o.raw[(size_t) k] = pickB ? b.raw[(size_t) k] : a.raw[(size_t) k]; continue; }
            const float va = (float) a.raw[(size_t) k];
            const float vb = (float) b.raw[(size_t) k];
            o.raw[(size_t) k] = (uint8_t) (va + (vb - va) * t + 0.5f);
        }
        return o;
    }
};

// ---------------------------------------------------------------------------
// Bank file loading ------------------------------------------------------
// Factory syx: 8-byte header + 128 * 176 (7-bit clean).
// SMF .mid: skip MTrk chunks, concatenate F0 payloads.
// r17: multi syx (Omega8FactoryMulti.syx): 8-byte header + 128 * 80:
//   запись = 8 parts x [patch(0..127, 0x7F=none), bank(0/1), vol, pan]
//          + 32 байта key/vel зоны (не используем) + 16-байт имя.
//   Проверено по записям: multi 0/1/2/3/100/127 (включая полный
//   8-part 'USER(8MONOMULTI)'). Слот получает первый активный part
//   из текущего банка (best effort — другие ROM-банки в VST не загружены).
struct MultiPart
{
    uint8_t patch = 0x7F;
    uint8_t bnk   = 0x7F;
    uint8_t vol   = 0;
    uint8_t pan   = 0;
};

struct Bank
{
    std::vector<Patch> patches;
    std::string sourceName;
    bool ok = false;
    bool isMulti = false;                                  // r17
    std::vector<std::array<MultiPart, 8>> multiParts;      // r17
    std::vector<std::string> multiNames;                   // r17
};

namespace detail
{
inline void eatSysexFrom (const std::vector<uint8_t>& d, size_t& p, std::vector<uint8_t>& msg)
{
    msg.clear();
    if (p >= d.size() || d[p] != 0xF0) { ++p; return; }
    while (p < d.size())
    {
        uint8_t c = d[p++];
        if (c == 0xF7 || c == 0xF0) { if (c == 0xF0) { msg.push_back (c); continue; } break; }
        msg.push_back (c);
    }
}
} // namespace detail

inline Bank loadBank (const std::vector<uint8_t>& data)
{
    namespace d = detail;
    Bank bank;

    std::vector<uint8_t> msgs;
    bool isSmf = data.size() > 4 && std::memcmp (data.data(), "MThd", 4) == 0;

    if (isSmf)
    {
        size_t p = 8 + 6;                       // MThd(8) + len(4) + format/ntrk/div(6)
        // find first MTrk
        while (p + 8 <= data.size() && std::memcmp (&data[p], "MTrk", 4) != 0) ++p;
        while (p + 8 <= data.size())
        {
            uint32_t len = (uint32_t) ((data[p+4] << 24) | (data[p+5] << 16) | (data[p+6] << 8) | data[p+7]);
            size_t q = p + 8, end = q + len;
            if (end > data.size()) break;
            // scan track for F0
            while (q < end)
            {
                if (data[q] == 0xF0)
                {
                    size_t start = q;
                    ++q;
                    while (q < end && data[q] != 0xF7) ++q;
                    if (q < end) ++q;
                    msgs.insert (msgs.end(), data.begin() + start, data.begin() + q);
                }
                else ++q;
            }
            p = end;
            while (p + 8 <= data.size() && std::memcmp (&data[p], "MTrk", 4) != 0) ++p;
        }
    }
    else
    {
        msgs = data;
    }

    // walk messages: layout is  F0 [SMF VLQ length]  8-byte header  N*176  [F7]
    auto considerStream = [&] (const std::vector<uint8_t>& s)
    {
        if (s.size() < 9 + kPatchSize || s[0] != 0xF0)
            return;

        size_t off = 1;                                   // past F0
        // SMF stores a VLQ length right after F0 (first byte >= 0x80);
        // .syx streams start with the 8-byte header (first byte < 0x80).
        if (off < s.size() && s[off] >= 0x80)
        {
            do { ++off; } while (off < s.size() && s[off - 1] >= 0x80);
        }
        if (off + 8 >= s.size()) return;
        off += 8;                                         // 8-byte Omega header

        size_t total = s.size() - off;
        if (total > 0 && s.back() == 0xF7) --total;        // trailing F7

        // r17: мульти-банк: 128 записей по 80 байт (было: тихий reject и
        // восприятие «крашит»; теперь парсим честно).
        if (total == 128 * 80 && total % kPatchSize != 0)
        {
            for (size_t i = 0; i < 128; ++i)
            {
                const uint8_t* r = s.data() + off + i * 80;
                std::array<MultiPart, 8> parts;
                for (int p = 0; p < 8; ++p)
                {
                    parts[(size_t) p].patch = r[p * 4 + 0];
                    parts[(size_t) p].bnk   = r[p * 4 + 1];
                    parts[(size_t) p].vol   = r[p * 4 + 2];
                    parts[(size_t) p].pan   = r[p * 4 + 3];
                }
                std::string nm;
                for (int c = 0; c < 16; ++c)
                {
                    unsigned char ch = r[64 + c];
                    if (ch == 0) break;
                    nm.push_back ((ch >= 32 && ch <= 126) ? (char) ch : ' ');
                }
                while (! nm.empty() && nm.back() == ' ') nm.pop_back();
                bank.isMulti = true;
                bank.multiParts.push_back (parts);
                bank.multiNames.push_back (nm);
            }
            if (! bank.multiParts.empty()) bank.ok = true;
            return;
        }

        if (total < kPatchSize || total % kPatchSize != 0)
            return;                                       // not a 176-byte patch stream

        size_t n = total / kPatchSize;
        size_t room = 128 - bank.patches.size();
        if (n > room) n = room;
        for (size_t i = 0; i < n; ++i)
        {
            Patch pt;
            std::memcpy (pt.raw.data(), s.data() + off + i * kPatchSize, kPatchSize);
            bool clean = true;
            for (auto b : pt.raw) if (b > 127) { clean = false; break; }
            if (! clean) continue;
            bank.patches.push_back (pt);
        }
    };

    if (isSmf)
    {
        size_t p = 0;
        std::vector<uint8_t> msg;
        while (p < data.size()) { d::eatSysexFrom (data, p, msg); if (! msg.empty()) considerStream (msg); }
    }
    else
    {
        considerStream (msgs);
    }

    bank.ok = ! bank.patches.empty() || (bank.isMulti && ! bank.multiParts.empty());
    return bank;
}

} // namespace omega8
