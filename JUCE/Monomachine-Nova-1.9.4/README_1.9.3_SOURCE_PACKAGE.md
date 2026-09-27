# Monomachine Nova 1.9.3 — source package

This package contains the paired source projects:

- `Monomachine_Nova_Synth/`
- `Monomachine_Nova_FX/`

Read `RELEASE_1.9.3_FILT_EXTRAS_ENV.md` first. It documents the new filter
controls, the exact one-stage post-FILT `SAT` topology, schema-20 migration,
and validation boundary.

Each project includes:

- its own CMake and Projucer metadata at version `1.9.3`;
- `SOURCE_BUILD.json` with SHA-256 hashes for its packaged files;
- `Check-Build.ps1`, which verifies those hashes and, optionally, a built VST3
  UI build marker.

This is source-only delivery. No VST3 binary is included.
