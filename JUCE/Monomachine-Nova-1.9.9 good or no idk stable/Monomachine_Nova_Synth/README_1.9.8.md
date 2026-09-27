# Monomachine Nova Synth 1.9.8

This source release adds usable feedback-loop filtering to EFFX DLY and appends the opt-in `NEW` Track Delay mode while preserving `mnm` and `old` at their saved/automation IDs `0` and `1`.

- Select `MODE DLY` = `new` only when the new practical Track Delay candidate is desired.
- `DBAS` raises the feedback high-pass edge; reducing `DWID` lowers the feedback low-pass edge.
- Right-click `DBAS` or `DWID` on either P1/P2 EFFX DLY page for the matching optional `DBAS Q` / `DWID Q` settings.
- Existing states before schema 23 never silently turn a historic value `2` into `NEW`; they fall back to `mnm`.

See [`../RELEASE_1.9.8_TRACK_DELAY_NEW.md`](../RELEASE_1.9.8_TRACK_DELAY_NEW.md) for the exact scope, provenance, parameter IDs, compatibility rule, and validation boundary. See [`../VALIDATION_1.9.8_TRACK_DELAY_NEW.md`](../VALIDATION_1.9.8_TRACK_DELAY_NEW.md) for the completed checks and remaining JUCE/DAW validation.
