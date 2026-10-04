#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>

namespace nova {
// DFB has a user-adjustable low-side BASE bend and a separate safety guard.
//
// DYNAMICS has one exact non-destructive anchor: RAW=63 is unity. BASE CURVE
// shapes RAW 0..62 toward that anchor, RAW=64 is the first controllable
// over-unity GUARD entry, and RAW=65 resumes the retained RAW/64 upper path.
// GUARD is a user-facing governor, not a claimed recovered firmware formula.
// Every guard branch, including final write clipping, is bypassed through 63;
// it starts at the discrete RAW=64 entry. DYNAMICS=OFF retains raw / 63 as an
// explicit legacy A/B.
class DelayFeedbackDynamics final {
public:
    static constexpr float kUnityFeedback = 1.0f;
    static constexpr float kRaw63Reference = 63.0f;
    static constexpr float kRaw64Hold = 64.0f;
    static constexpr float kRaw65Growth = 65.0f;
    static constexpr float kDefaultBaseCurve = 0.55f;
    // RAW=64 enters GUARD just above unity; matching RAW=65 removes a notch.
    static constexpr float kDefaultBaseHoldAt64 = kRaw65Growth / 64.0f;
    static constexpr float kLoopPlateauLevel = 0.05f;
    static constexpr float kDefaultPlateauLevelAt127 = 3.46f;
    static constexpr float kDefaultGuardLevelStart = 0.0f;
    static constexpr float kDefaultGuardOffset = 0.18f;
    static constexpr float kDefaultGuardCurve = 1.08f;
    static constexpr float kDefaultGuardAmount = 2.0f;
    static constexpr float kDefaultAttackMilliseconds = 1.0f;
    static constexpr float kDefaultReleaseMilliseconds = 1.0f;
    static constexpr float kGuardRawBypass = 63.0f;
    static constexpr float kGuardRawArm = 64.0f;
    static constexpr float kMaximumRawFeedback = 127.0f;

    // BASE BEND is deliberately compact: lower values approach exact unity at
    // RAW=63; HOLD @64 is the controllable entry into the RAW>=64 guard zone.
    struct Base {
        float lowCurve = kDefaultBaseCurve;
        float holdAt64 = kDefaultBaseHoldAt64;
    };

    // The two explicit raw anchors make the tail window controllable from
    // RAW=64 to RAW=127. OFFSET moves both boundaries together. CURVE and
    // AMOUNT remain separate pressure shaping controls.
    struct Guard {
        float levelStart = kDefaultGuardLevelStart;       // LVL START @ RAW 64
        float levelStartAt127 = kDefaultGuardLevelStart;  // LVL START @ RAW 127
        float levelOffset = kDefaultGuardOffset;          // common user offset
        float levelCurve = kDefaultGuardCurve;
        float amount = kDefaultGuardAmount;
    };

    struct Governor {
        float plateauLevel = kLoopPlateauLevel;           // PLATEAU @ RAW 64
        float plateauLevelAt127 = kDefaultPlateauLevelAt127; // PLATEAU @ RAW 127
        float attackMilliseconds = kDefaultAttackMilliseconds;
        float releaseMilliseconds = kDefaultReleaseMilliseconds;
    };

    struct GuardWindow {
        float start = kDefaultGuardLevelStart;
        float plateau = kLoopPlateauLevel;
        float gate = 0.0f;
    };

    static constexpr float kMinimumBaseCurve = 0.10f;
    static constexpr float kMaximumBaseCurve = 2.00f;
    static constexpr float kMinimumBaseHoldAt64 = kUnityFeedback;
    static constexpr float kMaximumBaseHoldAt64 = kRaw65Growth / 64.0f;
    static constexpr float kMinimumGuardLevelStart = 0.0f;
    static constexpr float kMaximumGuardLevelStart = 4.0f;
    static constexpr float kMinimumGuardLevelOffset = -2.0f;
    static constexpr float kMaximumGuardLevelOffset = 2.0f;
    static constexpr float kMinimumGuardCurve = 0.10f;
    static constexpr float kMaximumGuardCurve = 4.0f;
    static constexpr float kMinimumGuardAmount = 0.0f;
    static constexpr float kMaximumGuardAmount = 2.0f;
    static constexpr float kMinimumPlateauLevel = 0.05f;
    static constexpr float kMaximumPlateauLevel = 4.0f;
    static constexpr float kMinimumFollowerMilliseconds = 1.0f;
    static constexpr float kMaximumAttackMilliseconds = 1000.0f;
    static constexpr float kMaximumReleaseMilliseconds = 5000.0f;

    void prepare(double sampleRate) noexcept {
        sampleRate_ = std::isfinite(sampleRate) && sampleRate > 1000.0
            ? static_cast<float>(sampleRate) : 44100.0f;
        refreshFollowerCoefficients();
        rebuildBaseLut();
        rebuildGuardDriveLut();
        reset();
    }

    void reset() noexcept {
        activeFeedback_ = kUnityFeedback;
        effectiveFeedback_ = kUnityFeedback;
        loopLevel_ = 0.0f;
        lastRawFeedback_ = 0.0f;
    }

    // RISE and ZERO TAIL IDs remain serialized compatibility fields. Temporal
    // behaviour belongs to the level follower's ATTACK and RELEASE.
    void setControls(bool enabled, float legacyRiseSeconds, float legacyZeroTailSeconds) noexcept {
        enabled_ = enabled;
        (void)legacyRiseSeconds;
        (void)legacyZeroTailSeconds;
        if (!enabled_) loopLevel_ = 0.0f;
    }

    void setBase(Base requested) noexcept {
        const Base next = sanitiseBase(requested);
        if (sameBase(next, base_)) return;
        base_ = next;
        rebuildBaseLut();
    }
    Base base() const noexcept { return base_; }

    void setGuard(Guard requested) noexcept {
        const Guard next = sanitiseGuard(requested);
        if (sameGuard(next, guard_)) return;
        guard_ = next;
        rebuildGuardDriveLut();
    }
    Guard guard() const noexcept { return guard_; }

    // The follower coefficients and raw/level-drive LUT are refreshed only at
    // control-rate snapshots. Audio processing interpolates the LUT, never
    // evaluating pow() in the delay recurrence hot path.
    void setGovernor(Governor requested) noexcept {
        const Governor next = sanitiseGovernor(requested);
        if (sameGovernor(next, governor_)) return;
        governor_ = next;
        refreshFollowerCoefficients();
        rebuildGuardDriveLut();
    }
    Governor governor() const noexcept { return governor_; }

    // TrackChain snapshots every block. Rebuilding the 64x257 LUT for an
    // unchanged APVTS snapshot would waste several million pow() calls/sec.
    // Update both state groups atomically and rebuild once only on a real edit.
    void setGuardAndGovernor(Guard requestedGuard, Governor requestedGovernor) noexcept {
        const Guard nextGuard = sanitiseGuard(requestedGuard);
        const Governor nextGovernor = sanitiseGovernor(requestedGovernor);
        const bool guardChanged = !sameGuard(nextGuard, guard_);
        const bool governorChanged = !sameGovernor(nextGovernor, governor_);
        if (!guardChanged && !governorChanged) return;
        guard_ = nextGuard;
        governor_ = nextGovernor;
        if (governorChanged) refreshFollowerCoefficients();
        rebuildGuardDriveLut();
    }

    // FB CLIP owns the optional level-dependent guard as well as the final
    // per-core write stabiliser. Turning it off remains a direct A/B path.
    void setLoopGuard(bool enabled) noexcept {
        loopGuard_ = enabled;
        if (!loopGuard_) loopLevel_ = 0.0f;
    }

    // RAW <= 63 is a true guard-and-clip bypass. RAW=64 is the first fully
    // armed entry, so a unity RAW=63 tail cannot carry stale or active guard
    // pressure into the feedback path.
    void observeLoop(float left, float right) noexcept {
        const float gate = rawGateFor(lastRawFeedback_);
        if (!enabled_ || !loopGuard_ || gate <= 0.0f) {
            loopLevel_ = 0.0f;
            return;
        }
        const float l = std::isfinite(left) ? std::abs(left) : 0.0f;
        const float r = std::isfinite(right) ? std::abs(right) : 0.0f;
        const float target = std::max(l, r) * gate;
        const float alpha = target > loopLevel_ ? attackAlpha_ : releaseAlpha_;
        loopLevel_ += (target - loopLevel_) * alpha;
        if (std::abs(target - loopLevel_) < 1.0e-7f) loopLevel_ = target;
    }

    float process(float rawFeedback) noexcept {
        const float raw = sanitiseRaw(rawFeedback);
        lastRawFeedback_ = raw;
        if (!enabled_) {
            activeFeedback_ = raw / 63.0f;
            effectiveFeedback_ = activeFeedback_;
            return effectiveFeedback_;
        }

        // The base LUT has no audio-rate pow(): it curves only the requested
        // low side, keeps 63 as reference, softly holds 64, and resumes the
        // upper RAW/64 path at 65.
        activeFeedback_ = baseCoefficientFromLut(raw);
        effectiveFeedback_ = guardedFeedback(raw, activeFeedback_);
        return effectiveFeedback_;
    }

    bool enabled() const noexcept { return enabled_; }
    float activeFeedback() const noexcept { return activeFeedback_; }
    float effectiveFeedback() const noexcept { return effectiveFeedback_; }
    float loopLevel() const noexcept { return loopLevel_; }
    float lastRawFeedback() const noexcept { return lastRawFeedback_; }

    static Base sanitiseBase(Base requested) noexcept {
        const auto finite=[](float value, float fallback) noexcept {
            return std::isfinite(value) ? value : fallback;
        };
        Base result;
        result.lowCurve = std::clamp(finite(requested.lowCurve, result.lowCurve),
                                     kMinimumBaseCurve, kMaximumBaseCurve);
        result.holdAt64 = std::clamp(finite(requested.holdAt64, result.holdAt64),
                                     kMinimumBaseHoldAt64, kMaximumBaseHoldAt64);
        return result;
    }

    static Guard sanitiseGuard(Guard requested) noexcept {
        const auto finite=[](float value, float fallback) noexcept {
            return std::isfinite(value) ? value : fallback;
        };
        Guard result;
        result.levelStart = std::clamp(finite(requested.levelStart, result.levelStart),
                                       kMinimumGuardLevelStart, kMaximumGuardLevelStart);
        result.levelStartAt127 = std::clamp(finite(requested.levelStartAt127, result.levelStartAt127),
                                            kMinimumGuardLevelStart, kMaximumGuardLevelStart);
        result.levelOffset = std::clamp(finite(requested.levelOffset, result.levelOffset),
                                        kMinimumGuardLevelOffset, kMaximumGuardLevelOffset);
        result.levelCurve = std::clamp(finite(requested.levelCurve, result.levelCurve),
                                       kMinimumGuardCurve, kMaximumGuardCurve);
        result.amount = std::clamp(finite(requested.amount, result.amount),
                                   kMinimumGuardAmount, kMaximumGuardAmount);
        return result;
    }

    static Governor sanitiseGovernor(Governor requested) noexcept {
        const auto finite=[](float value, float fallback) noexcept {
            return std::isfinite(value) ? value : fallback;
        };
        Governor result;
        result.plateauLevel = std::clamp(finite(requested.plateauLevel, result.plateauLevel),
                                         kMinimumPlateauLevel, kMaximumPlateauLevel);
        result.plateauLevelAt127 = std::clamp(finite(requested.plateauLevelAt127, result.plateauLevelAt127),
                                              kMinimumPlateauLevel, kMaximumPlateauLevel);
        result.attackMilliseconds = std::clamp(finite(requested.attackMilliseconds, result.attackMilliseconds),
                                                kMinimumFollowerMilliseconds, kMaximumAttackMilliseconds);
        result.releaseMilliseconds = std::clamp(finite(requested.releaseMilliseconds, result.releaseMilliseconds),
                                                 kMinimumFollowerMilliseconds, kMaximumReleaseMilliseconds);
        return result;
    }

    static float baseCoefficientForRaw(float rawFeedback) noexcept {
        return baseCoefficientForRaw(rawFeedback, Base{});
    }
    static float baseCoefficientForRaw(float rawFeedback, Base requested) noexcept {
        const Base base = sanitiseBase(requested);
        const float raw = sanitiseRaw(rawFeedback);
        const float reference = kUnityFeedback;
        if (raw <= kRaw63Reference) {
            if (raw <= 0.0f) return 0.0f;
            return reference * std::pow(raw / kRaw63Reference, base.lowCurve);
        }
        if (raw <= kRaw64Hold)
            return reference + (base.holdAt64 - reference) * (raw - kRaw63Reference);
        if (raw < kRaw65Growth)
            return base.holdAt64 + ((kRaw65Growth / 64.0f) - base.holdAt64)
                                 * (raw - kRaw64Hold);
        return raw / 64.0f;
    }

    static bool guardOwnsRaw(float rawFeedback) noexcept {
        return sanitiseRaw(rawFeedback) >= kGuardRawArm;
    }
    static float rawGateFor(float rawFeedback) noexcept {
        return guardOwnsRaw(rawFeedback) ? 1.0f : 0.0f;
    }

    // The raw-64 and raw-127 boundaries are interpolated only at/above the
    // discrete arm point. A common OFFSET moves the safety window without
    // altering its span. If PLATEAU is moved to/below START, it safely disables.
    static GuardWindow guardWindowForRaw(float rawFeedback, Guard requested,
                                         Governor requestedGovernor) noexcept {
        const Guard guard = sanitiseGuard(requested);
        const Governor governor = sanitiseGovernor(requestedGovernor);
        const float raw = sanitiseRaw(rawFeedback);
        const float unit = std::clamp((raw - kGuardRawArm)
                                      / (kMaximumRawFeedback - kGuardRawArm), 0.0f, 1.0f);
        const auto lerp=[](float from, float to, float amount) noexcept {
            return from + (to - from) * amount;
        };
        GuardWindow result;
        result.start = std::clamp(lerp(guard.levelStart, guard.levelStartAt127, unit)
                                  + guard.levelOffset,
                                  kMinimumGuardLevelStart, kMaximumGuardLevelStart);
        result.plateau = std::clamp(lerp(governor.plateauLevel, governor.plateauLevelAt127, unit)
                                    + guard.levelOffset,
                                    kMinimumGuardLevelStart, kMaximumPlateauLevel);
        result.gate = rawGateFor(raw);
        return result;
    }

    static float guardDriveForRawLevel(float rawFeedback, float loopLevel,
                                       Guard requested, Governor requestedGovernor) noexcept {
        const Guard guard = sanitiseGuard(requested);
        const Governor governor = sanitiseGovernor(requestedGovernor);
        return guardDriveForSanitisedState(rawFeedback, loopLevel, guard, governor);
    }

    // At or below RAW=64 the coefficient stays exact BASE. Above unity the
    // guard uses the current RAW's interpolated window and never keeps adding
    // pressure beyond the selected plateau window.
    static float guardedCoefficient(float rawFeedback, float coefficient, float loopLevel,
                                    Guard requested, Governor requestedGovernor,
                                    bool guardEnabled = true) noexcept {
        const float safeCoefficient = std::isfinite(coefficient) ? coefficient : 0.0f;
        if (!guardEnabled || safeCoefficient <= kUnityFeedback) return safeCoefficient;
        const Guard guard = sanitiseGuard(requested);
        const Governor governor = sanitiseGovernor(requestedGovernor);
        const float drive = guardDriveForSanitisedState(rawFeedback, loopLevel, guard, governor);
        return guardedCoefficientUnchecked(safeCoefficient, guard.amount * drive);
    }

private:
    static constexpr int kBaseLutSize = 128; // RAW 0..127, inclusive steps
    static constexpr int kGuardDriveLutSize = 257;
    static constexpr int kGuardRawLutSize = 65; // RAW 63..127, inclusive

    static bool sameBase(const Base& a, const Base& b) noexcept {
        return a.lowCurve == b.lowCurve && a.holdAt64 == b.holdAt64;
    }
    static bool sameGuard(const Guard& a, const Guard& b) noexcept {
        return a.levelStart == b.levelStart && a.levelStartAt127 == b.levelStartAt127
            && a.levelOffset == b.levelOffset && a.levelCurve == b.levelCurve
            && a.amount == b.amount;
    }
    static bool sameGovernor(const Governor& a, const Governor& b) noexcept {
        return a.plateauLevel == b.plateauLevel && a.plateauLevelAt127 == b.plateauLevelAt127
            && a.attackMilliseconds == b.attackMilliseconds
            && a.releaseMilliseconds == b.releaseMilliseconds;
    }
    static float sanitiseRaw(float rawFeedback) noexcept {
        return std::clamp(std::isfinite(rawFeedback) ? rawFeedback : 0.0f,
                          0.0f, kMaximumRawFeedback);
    }
    float baseCoefficientFromLut(float rawFeedback) const noexcept {
        const float position = std::clamp(sanitiseRaw(rawFeedback), 0.0f, kMaximumRawFeedback);
        const int index = std::clamp(static_cast<int>(position), 0, kBaseLutSize - 1);
        const int next = std::min(index + 1, kBaseLutSize - 1);
        const float fraction = position - static_cast<float>(index);
        return baseLut_[static_cast<std::size_t>(index)]
             + fraction * (baseLut_[static_cast<std::size_t>(next)] - baseLut_[static_cast<std::size_t>(index)]);
    }
    void rebuildBaseLut() noexcept {
        for (int index = 0; index < kBaseLutSize; ++index)
            baseLut_[static_cast<std::size_t>(index)] =
                baseCoefficientForRaw(static_cast<float>(index), base_);
    }
    static float guardDriveForSanitisedState(float rawFeedback, float loopLevel,
                                             const Guard& guard,
                                             const Governor& governor) noexcept {
        const GuardWindow window = guardWindowForRaw(rawFeedback, guard, governor);
        const float level = std::isfinite(loopLevel) ? std::max(0.0f, loopLevel) : 0.0f;
        if (window.gate <= 0.0f || !(window.plateau > window.start + 1.0e-5f)
            || level <= window.start)
            return 0.0f;
        const float unit = std::clamp((level - window.start)
                                      / (window.plateau - window.start), 0.0f, 1.0f);
        return window.gate * std::pow(unit, guard.levelCurve);
    }
    static float guardedCoefficientUnchecked(float coefficient, float strength) noexcept {
        if (strength <= 0.0f) return coefficient;
        return coefficient / (1.0f + (coefficient - kUnityFeedback) * strength);
    }
    float guardDriveFromLut(float rawFeedback, float loopLevel) const noexcept {
        if (!guardOwnsRaw(rawFeedback)) return 0.0f;
        const float rawPosition = std::clamp(sanitiseRaw(rawFeedback), kGuardRawBypass,
                                             kMaximumRawFeedback) - kGuardRawBypass;
        const int rawIndex = std::clamp(static_cast<int>(rawPosition), 0, kGuardRawLutSize - 1);
        const int rawNext = std::min(rawIndex + 1, kGuardRawLutSize - 1);
        const float rawFraction = rawPosition - static_cast<float>(rawIndex);
        const float normal = std::clamp(std::isfinite(loopLevel) ? loopLevel : 0.0f,
                                        0.0f, kMaximumPlateauLevel) / kMaximumPlateauLevel;
        const float levelPosition = normal * static_cast<float>(kGuardDriveLutSize - 1);
        const int levelIndex = std::clamp(static_cast<int>(levelPosition), 0, kGuardDriveLutSize - 1);
        const int levelNext = std::min(levelIndex + 1, kGuardDriveLutSize - 1);
        const float levelFraction = levelPosition - static_cast<float>(levelIndex);
        const auto value=[this](int rawIndexValue, int levelIndexValue) noexcept {
            return guardDriveLut_[static_cast<std::size_t>(rawIndexValue * kGuardDriveLutSize
                                                            + levelIndexValue)];
        };
        const float lower = value(rawIndex, levelIndex)
                          + levelFraction * (value(rawIndex, levelNext) - value(rawIndex, levelIndex));
        const float upper = value(rawNext, levelIndex)
                          + levelFraction * (value(rawNext, levelNext) - value(rawNext, levelIndex));
        return lower + rawFraction * (upper - lower);
    }
    void rebuildGuardDriveLut() noexcept {
        for (int rawIndex = 0; rawIndex < kGuardRawLutSize; ++rawIndex) {
            const float raw = kGuardRawBypass + static_cast<float>(rawIndex);
            for (int levelIndex = 0; levelIndex < kGuardDriveLutSize; ++levelIndex) {
                const float level = kMaximumPlateauLevel * static_cast<float>(levelIndex)
                                  / static_cast<float>(kGuardDriveLutSize - 1);
                guardDriveLut_[static_cast<std::size_t>(rawIndex * kGuardDriveLutSize + levelIndex)] =
                    guardDriveForSanitisedState(raw, level, guard_, governor_);
            }
        }
    }
    float guardedFeedback(float rawFeedback, float coefficient) const noexcept {
        if (!loopGuard_ || !guardOwnsRaw(rawFeedback) || coefficient <= kUnityFeedback) return coefficient;
        return guardedCoefficientUnchecked(coefficient,
                                           guard_.amount * guardDriveFromLut(rawFeedback, loopLevel_));
    }
    static float followerAlpha(float sampleRate, float milliseconds) noexcept {
        const float seconds = std::max(0.001f, milliseconds * 0.001f);
        return 1.0f - std::exp(-1.0f / (sampleRate * seconds));
    }
    void refreshFollowerCoefficients() noexcept {
        attackAlpha_ = followerAlpha(sampleRate_, governor_.attackMilliseconds);
        releaseAlpha_ = followerAlpha(sampleRate_, governor_.releaseMilliseconds);
    }

    float sampleRate_ = 44100.0f;
    Base base_{};
    Guard guard_{};
    std::array<float, kBaseLutSize> baseLut_{};
    Governor governor_{};
    std::array<float, kGuardRawLutSize * kGuardDriveLutSize> guardDriveLut_{};
    float activeFeedback_ = kUnityFeedback;
    float effectiveFeedback_ = kUnityFeedback;
    float loopLevel_ = 0.0f;
    float lastRawFeedback_ = 0.0f;
    float attackAlpha_ = 0.0f, releaseAlpha_ = 0.0f;
    bool enabled_ = true;
    bool loopGuard_ = true;
};
} // namespace nova
