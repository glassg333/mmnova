# Архив снятых FM-кандидатов — 1.9.26

Дата архивации: 30.09.2026.

Здесь без изменения байтов сохранены пути из shipping Source обеих целей до
очистки 1.9.26. После копирования SHA-256 каждого файла был сопоставлен с
исходным путём, и только затем исходные candidate-only каталоги, тесты и
fixture были удалены из `Monomachine_Nova_Synth/` и `Monomachine_Nova_FX/`.

## Что снято с compilation/runtime graph

- `Source/dsp/fm_mnm_frq_env_fix/` — бывший raw ID 6;
- `Source/dsp/fm_try4_voice/` — бывший raw ID 7;
- `Source/dsp/fm_fix5/` — бывший raw ID 8;
- пустой `Source/dsp/monomachine_fm_stat.hpp` tombstone;
- `Source/models/FmExperimentProfiles.hpp`, связанный только с снятыми
  candidate display/default branches;
- `FmExperimentsTests.cpp`, `Fix5PluginLevelTests.cpp`, исходный
  `Fix5HostIntegrationTests.cpp` и fixture.

Их относительные пути внутри каждой цели сохранены буквально. Это история, а
не часть shipping Source и не источник текущего DSP dispatch.

## Что осталось активным

Режимы FM+ `0..5` (`mnm/old/new frq`, `mnm/new/old bpm`), их machine IDs,
параметры и routing не перемещались. `dsp/monomachine_chorus.hpp` и
`dsp/monomachine_voice_chain.hpp` в этот архив не входят и оставлены без
изменений по отдельному решению.

`SHA256SUMS.txt` содержит контрольные суммы всего архивного payload и этого
README; сам файл контрольных сумм в собственную сумму не включается.
