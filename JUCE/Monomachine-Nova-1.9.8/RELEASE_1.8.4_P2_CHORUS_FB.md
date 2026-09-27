# Monomachine Nova — 1.8.4 P2 / CHORUS / FB follow-up

## Scope

This source snapshot follows `1.8.4 Classic Routing`. It keeps the native
CHORUS core and its tables intact. `CHORUS SAFE [NOT ORIGINAL]` remains an
explicit DSP-menu comparison option and is OFF by default.

## Confirmed P2 tail-clear fault fixed

`PANIC / CLEAR TAILS` already reset P1/P2 machine engines, but
`TrackChain::clear()` did not reset the state held by the native classic
filter and native delay cores. A cleared P2 path could therefore replay an
old low-frequency, feedback-like residue on the next pass. The fault is now
fixed by resetting both native cores together with the existing legacy
buffers and EQ state.

The clear operation now also clears every active FX-slot engine while keeping
its selected machine and controls intact. Thus a tail held by an active slot
cannot survive `PANIC / CLEAR TAILS` either.

No CHORUS table, native CHORUS arithmetic, CHORUS parameter law, or normal
(non-clear) P2 signal path was changed. A fixed P2-CHORUS trace remains
`1402208c0e4603bf`, matching the pre-routing 1.8.3 and Classic Routing
reference trace.

## Existing follow-up changes retained

- P2 control smoothing covers all 32 modulated P2 controls. `P2 MIX` is a
  real matrix/P-LOCK target and has its own audio-rate dry/wet ramp.
- P1/P2 PAN and VOL have a short audio-rate de-zipper.
- Global FB remains exact one-sample feedback, with an explicit `CLIP`
  control and DC HP range of 0.1…100 Hz. No automatic limiter was added.
- General MENU has no hidden CHORUS-idle option. The only bypass comparison
  is the explicit non-original `CHORUS SAFE` DSP switch.

## Regression coverage

`tests/TailClearTests.cpp` was added to both targets and registered in CMake
when `NOVA_BUILD_TESTS=ON`. It verifies both:

1. A native TrackChain delay recurrence becomes exactly silent after `clear()`.
2. An active CHORUS FX-slot has signal before clear and zero signal after
   `clear()`.

Headless validation after this change passed for both Synth and FX targets:

- core headless smoke;
- PPHASH: Synth `ec101520ac42bb83`, FX `f75309bcac42bb83`;
- FX-slots, classic routing, native-FX match, UI smoke, and P2 declick;
- ChorusAudit bit-exact native/core comparison;
- exact one-sample FB/clip/DC/serialization checks;
- TailClearTests: native TrackChain residue `0`, CHORUS FX-slot residue `0`;
- exact-FB benchmark: Synth `4.28%`, FX `4.58%` on the headless benchmark.
