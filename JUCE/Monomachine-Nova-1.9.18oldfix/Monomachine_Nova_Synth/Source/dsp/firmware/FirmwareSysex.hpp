// Clean-room, firmware-free OS SysEx inspection contract.
//
// This file never contains an OS image and never serializes one.  It only
// performs bounded transport-framing inspection of a user-selected .syx,
// derives its SHA-256, and retains a path/hash reference for project-state
// verification. It does not execute or authenticate an OS image.
#pragma once

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

namespace nova::firmware {

inline constexpr std::size_t kMaxFirmwareSysexBytes = 32u * 1024u * 1024u;
inline constexpr std::array<std::uint8_t, 7> kMonomachineOsHeader = {
    0xf0u, 0x00u, 0x20u, 0x3cu, 0x03u, 0x00u, 0x7eu
};

// Small self-contained SHA-256 implementation. It lets a saved project prove
// that the same external user file is present without embedding its contents.
class Sha256 final {
public:
    Sha256() { reset(); }

    void reset() noexcept
    {
        state_ = {0x6a09e667u, 0xbb67ae85u, 0x3c6ef372u, 0xa54ff53au,
                  0x510e527fu, 0x9b05688cu, 0x1f83d9abu, 0x5be0cd19u};
        buffered_ = 0;
        bits_ = 0;
    }

    void update(const std::uint8_t* data, std::size_t size) noexcept
    {
        if (data == nullptr || size == 0) return;
        bits_ += static_cast<std::uint64_t>(size) * 8u;
        while (size != 0) {
            const auto take = (std::min)(size, block_.size() - buffered_);
            for (std::size_t i = 0; i < take; ++i) block_[buffered_ + i] = data[i];
            buffered_ += take;
            data += take;
            size -= take;
            if (buffered_ == block_.size()) {
                transform(block_.data());
                buffered_ = 0;
            }
        }
    }

    std::array<std::uint8_t, 32> finish() noexcept
    {
        const auto bitCount = bits_;
        block_[buffered_++] = 0x80u;
        if (buffered_ > 56) {
            while (buffered_ < block_.size()) block_[buffered_++] = 0;
            transform(block_.data());
            buffered_ = 0;
        }
        while (buffered_ < 56) block_[buffered_++] = 0;
        for (int i = 7; i >= 0; --i)
            block_[buffered_++] = static_cast<std::uint8_t>(bitCount >> (i * 8));
        transform(block_.data());
        buffered_ = 0;

        std::array<std::uint8_t, 32> out{};
        for (std::size_t i = 0; i < state_.size(); ++i) {
            out[i * 4]     = static_cast<std::uint8_t>(state_[i] >> 24);
            out[i * 4 + 1] = static_cast<std::uint8_t>(state_[i] >> 16);
            out[i * 4 + 2] = static_cast<std::uint8_t>(state_[i] >> 8);
            out[i * 4 + 3] = static_cast<std::uint8_t>(state_[i]);
        }
        return out;
    }

private:
    static constexpr std::uint32_t rotr(std::uint32_t value, std::uint32_t shift) noexcept
    {
        return (value >> shift) | (value << (32u - shift));
    }

    static constexpr std::uint32_t choose(std::uint32_t x, std::uint32_t y, std::uint32_t z) noexcept
    {
        return (x & y) ^ (~x & z);
    }

    static constexpr std::uint32_t majority(std::uint32_t x, std::uint32_t y, std::uint32_t z) noexcept
    {
        return (x & y) ^ (x & z) ^ (y & z);
    }

    static constexpr std::uint32_t upper0(std::uint32_t x) noexcept
    {
        return rotr(x, 2) ^ rotr(x, 13) ^ rotr(x, 22);
    }

    static constexpr std::uint32_t upper1(std::uint32_t x) noexcept
    {
        return rotr(x, 6) ^ rotr(x, 11) ^ rotr(x, 25);
    }

    static constexpr std::uint32_t lower0(std::uint32_t x) noexcept
    {
        return rotr(x, 7) ^ rotr(x, 18) ^ (x >> 3);
    }

    static constexpr std::uint32_t lower1(std::uint32_t x) noexcept
    {
        return rotr(x, 17) ^ rotr(x, 19) ^ (x >> 10);
    }

    void transform(const std::uint8_t* bytes) noexcept
    {
        static constexpr std::array<std::uint32_t, 64> k = {
            0x428a2f98u,0x71374491u,0xb5c0fbcfu,0xe9b5dba5u,0x3956c25bu,0x59f111f1u,0x923f82a4u,0xab1c5ed5u,
            0xd807aa98u,0x12835b01u,0x243185beu,0x550c7dc3u,0x72be5d74u,0x80deb1feu,0x9bdc06a7u,0xc19bf174u,
            0xe49b69c1u,0xefbe4786u,0x0fc19dc6u,0x240ca1ccu,0x2de92c6fu,0x4a7484aau,0x5cb0a9dcu,0x76f988dau,
            0x983e5152u,0xa831c66du,0xb00327c8u,0xbf597fc7u,0xc6e00bf3u,0xd5a79147u,0x06ca6351u,0x14292967u,
            0x27b70a85u,0x2e1b2138u,0x4d2c6dfcu,0x53380d13u,0x650a7354u,0x766a0abbu,0x81c2c92eu,0x92722c85u,
            0xa2bfe8a1u,0xa81a664bu,0xc24b8b70u,0xc76c51a3u,0xd192e819u,0xd6990624u,0xf40e3585u,0x106aa070u,
            0x19a4c116u,0x1e376c08u,0x2748774cu,0x34b0bcb5u,0x391c0cb3u,0x4ed8aa4au,0x5b9cca4fu,0x682e6ff3u,
            0x748f82eeu,0x78a5636fu,0x84c87814u,0x8cc70208u,0x90befffau,0xa4506cebu,0xbef9a3f7u,0xc67178f2u
        };
        std::array<std::uint32_t, 64> w{};
        for (std::size_t i = 0; i < 16; ++i)
            w[i] = (std::uint32_t(bytes[i * 4]) << 24)
                 | (std::uint32_t(bytes[i * 4 + 1]) << 16)
                 | (std::uint32_t(bytes[i * 4 + 2]) << 8)
                 |  std::uint32_t(bytes[i * 4 + 3]);
        for (std::size_t i = 16; i < w.size(); ++i)
            w[i] = lower1(w[i - 2]) + w[i - 7] + lower0(w[i - 15]) + w[i - 16];

        auto a = state_[0]; auto b = state_[1]; auto c = state_[2]; auto d = state_[3];
        auto e = state_[4]; auto f = state_[5]; auto g = state_[6]; auto h = state_[7];
        for (std::size_t i = 0; i < w.size(); ++i) {
            const auto t1 = h + upper1(e) + choose(e, f, g) + k[i] + w[i];
            const auto t2 = upper0(a) + majority(a, b, c);
            h = g; g = f; f = e; e = d + t1;
            d = c; c = b; b = a; a = t1 + t2;
        }
        state_[0] += a; state_[1] += b; state_[2] += c; state_[3] += d;
        state_[4] += e; state_[5] += f; state_[6] += g; state_[7] += h;
    }

    std::array<std::uint32_t, 8> state_{};
    std::array<std::uint8_t, 64> block_{};
    std::size_t buffered_ = 0;
    std::uint64_t bits_ = 0;
};

inline std::string sha256Hex(const std::uint8_t* bytes, std::size_t size)
{
    Sha256 digest;
    digest.update(bytes, size);
    const auto result = digest.finish();
    std::ostringstream out;
    out << std::hex << std::setfill('0');
    for (const auto byte : result) out << std::setw(2) << static_cast<unsigned>(byte);
    return out.str();
}

inline std::string sha256Hex(const std::vector<std::uint8_t>& bytes)
{
    return sha256Hex(bytes.empty() ? nullptr : bytes.data(), bytes.size());
}

struct SysexInspection {
    bool valid = false;
    std::string error;
    std::size_t fileBytes = 0;
    std::size_t dataPackets = 0;
    std::size_t ignoredPackets = 0;
    std::size_t decodedTransportBytes = 0;
    std::uint32_t firstAddress = 0;
    std::uint32_t endAddress = 0;
};

inline bool hasMonomachineOsHeader(const std::vector<std::uint8_t>& bytes, std::size_t begin) noexcept
{
    if (begin + kMonomachineOsHeader.size() > bytes.size()) return false;
    for (std::size_t i = 0; i < kMonomachineOsHeader.size(); ++i)
        if (bytes[begin + i] != kMonomachineOsHeader[i]) return false;
    return true;
}

// Inspects the 7-bit SysEx transport only. Decompression, DSP records and the
// OS itself are intentionally not stored by this routine.
inline SysexInspection inspectMonomachineOsSysex(const std::vector<std::uint8_t>& bytes)
{
    SysexInspection result;
    result.fileBytes = bytes.size();
    if (bytes.empty()) { result.error = "empty SysEx file"; return result; }
    if (bytes.size() > kMaxFirmwareSysexBytes) { result.error = "SysEx file exceeds safety limit"; return result; }

    bool seenData = false;
    std::uint32_t expectedAddress = 0;
    std::size_t pos = 0;
    while (pos < bytes.size()) {
        while (pos < bytes.size() && bytes[pos] != 0xf0u) ++pos;
        if (pos == bytes.size()) break;
        const auto packetBegin = pos++;
        while (pos < bytes.size() && bytes[pos] != 0xf7u) {
            if (bytes[pos] > 0x7fu) { result.error = "SysEx packet contains a non-7-bit data byte"; return result; }
            ++pos;
        }
        if (pos == bytes.size()) { result.error = "unterminated SysEx packet"; return result; }
        const auto packetEnd = pos++; // points to F7

        if (!hasMonomachineOsHeader(bytes, packetBegin)) {
            ++result.ignoredPackets;
            continue;
        }
        // Header (7), transport checksum (2), six 4-bit address nibbles,
        // and at least one packed triplet followed by F7.
        constexpr std::size_t kAddressAt = 9;
        constexpr std::size_t kPayloadAt = 15;
        if (packetEnd < packetBegin + kPayloadAt + 3) {
            result.error = "Monomachine OS packet is too short";
            return result;
        }
        std::uint32_t address = 0;
        for (std::size_t nibble = 0; nibble < 6; ++nibble) {
            const auto value = bytes[packetBegin + kAddressAt + nibble];
            if (value > 0x0fu) { result.error = "OS packet address is not nibble encoded"; return result; }
            address = (address << 4) | value;
        }
        const auto payloadBytes = packetEnd - (packetBegin + kPayloadAt);
        if (payloadBytes % 3 != 0) { result.error = "OS packet packed payload is not triplet aligned"; return result; }
        if (!seenData) {
            seenData = true;
            expectedAddress = address;
            result.firstAddress = address;
        }
        if (address != expectedAddress) { result.error = "OS packet address sequence is not contiguous"; return result; }
        const auto unpacked = static_cast<std::uint32_t>((payloadBytes / 3) * 2);
        if (unpacked > std::numeric_limits<std::uint32_t>::max() - expectedAddress) {
            result.error = "OS packet address overflows";
            return result;
        }
        expectedAddress += unpacked;
        result.decodedTransportBytes += static_cast<std::size_t>(unpacked);
        ++result.dataPackets;
    }
    if (!seenData) { result.error = "no Monomachine OS data packets found"; return result; }
    result.endAddress = expectedAddress;
    result.valid = true;
    return result;
}

// This is the complete persisted capability shape. Transport inspection
// metrics are deliberately LoadResult-only diagnostics, not project state.
struct SourceReference {
    std::string path;
    std::string sha256;

    bool empty() const noexcept { return path.empty() || sha256.empty(); }
};

struct LoadResult {
    SourceReference reference;
    SysexInspection inspection;
    std::string error;

    bool ok() const noexcept { return error.empty() && inspection.valid && !reference.empty(); }
};

inline LoadResult inspectUserFirmwareFile(const std::filesystem::path& path)
{
    LoadResult result;
    std::error_code ec;
    const auto bytesOnDisk = std::filesystem::file_size(path, ec);
    if (ec || bytesOnDisk == 0) { result.error = "cannot read selected SysEx file"; return result; }
    if (bytesOnDisk > kMaxFirmwareSysexBytes) { result.error = "selected SysEx file exceeds safety limit"; return result; }

    std::ifstream input(path, std::ios::binary);
    if (!input) { result.error = "cannot open selected SysEx file"; return result; }
    std::vector<std::uint8_t> bytes(static_cast<std::size_t>(bytesOnDisk));
    input.read(reinterpret_cast<char*>(bytes.data()), static_cast<std::streamsize>(bytes.size()));
    if (!input || static_cast<std::size_t>(input.gcount()) != bytes.size()) {
        result.error = "cannot fully read selected SysEx file";
        return result;
    }

    result.inspection = inspectMonomachineOsSysex(bytes);
    if (!result.inspection.valid) { result.error = result.inspection.error; return result; }
    result.reference.path = path.lexically_normal().string();
    result.reference.sha256 = sha256Hex(bytes);
    // bytes intentionally die here: no firmware byte vector escapes this call.
    return result;
}

inline bool verifyUserFirmwareReference(const SourceReference& reference, std::string* error = nullptr)
{
    if (reference.empty()) {
        if (error != nullptr) *error = "firmware source reference is empty";
        return false;
    }
    const auto inspected = inspectUserFirmwareFile(std::filesystem::path(reference.path));
    if (!inspected.ok()) {
        if (error != nullptr) *error = inspected.error;
        return false;
    }
    if (inspected.reference.sha256 != reference.sha256) {
        if (error != nullptr) *error = "selected SysEx SHA-256 differs from saved project reference";
        return false;
    }
    return true;
}

} // namespace nova::firmware
