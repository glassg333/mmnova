#pragma once

#include <array>
#include <atomic>
#include <algorithm>
#include <cmath>
#include <cstdint>

namespace monomachine {

// A compact, audio-thread safe multi-segment envelope. The editor owns the point
// order; the DSP only reads the atomics, so dragging a point never touches the
// audio thread's allocations or locks.
class MSEG {
public:
    static constexpr int kMaxPoints = 32;

    MSEG() { reset(); }

    void reset() noexcept {
        pointCount.store(4, std::memory_order_relaxed);
        setPoint(0, 0.00f, 0.00f);
        setPoint(1, 0.24f, 1.00f);
        setPoint(2, 0.62f, 0.18f);
        setPoint(3, 1.00f, 0.72f);
        for (int i = 4; i < kMaxPoints; ++i) setPoint(i, 1.0f, 0.0f);
    }

    int size() const noexcept { return pointCount.load(std::memory_order_acquire); }
    void setPointCount(int count) noexcept { pointCount.store(std::clamp(count, 2, kMaxPoints), std::memory_order_release); }

    float x(int index) const noexcept {
        return index >= 0 && index < kMaxPoints ? xs[static_cast<size_t>(index)].load(std::memory_order_relaxed) : 0.0f;
    }
    float y(int index) const noexcept {
        return index >= 0 && index < kMaxPoints ? ys[static_cast<size_t>(index)].load(std::memory_order_relaxed) : 0.0f;
    }

    void setPoint(int index, float px, float py) noexcept {
        if (index < 0 || index >= kMaxPoints) return;
        xs[static_cast<size_t>(index)].store(std::clamp(px, 0.0f, 1.0f), std::memory_order_release);
        ys[static_cast<size_t>(index)].store(std::clamp(py, 0.0f, 1.0f), std::memory_order_release);
    }

    bool addPoint(float px, float py) noexcept {
        const int n = size();
        if (n >= kMaxPoints) return false;
        int at = 0;
        while (at < n && x(at) < px) ++at;
        for (int i = n; i > at; --i) setPoint(i, x(i - 1), y(i - 1));
        setPoint(at, px, py);
        pointCount.store(n + 1, std::memory_order_release);
        return true;
    }

    bool removePoint(int index) noexcept {
        const int n = size();
        if (n <= 2 || index <= 0 || index >= n - 1) return false;
        for (int i = index; i < n - 1; ++i) setPoint(i, x(i + 1), y(i + 1));
        pointCount.store(n - 1, std::memory_order_release);
        return true;
    }

    // Linear segments are deliberately predictable: the curve itself is edited
    // by points, while the matrix supplies polarity and depth in percent.
    float value(float phase) const noexcept {
        const int n = std::clamp(size(), 2, kMaxPoints);
        phase = std::clamp(phase, 0.0f, 1.0f);
        if (phase <= x(0)) return y(0);
        for (int i = 1; i < n; ++i) {
            const float right = x(i);
            if (phase <= right) {
                const float left = x(i - 1);
                const float span = std::max(0.000001f, right - left);
                const float t = std::clamp((phase - left) / span, 0.0f, 1.0f);
                return y(i - 1) + (y(i) - y(i - 1)) * t;
            }
        }
        return y(n - 1);
    }

private:
    std::array<std::atomic<float>, kMaxPoints> xs{};
    std::array<std::atomic<float>, kMaxPoints> ys{};
    std::atomic<int> pointCount{4};
};

} // namespace monomachine
