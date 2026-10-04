# Статическая проверка DFB: RAW=63 UNITY и вход GUARD с RAW=64 — 04.10.2026

## Причина корректировки

Пользовательский feedback зафиксировал неверную рабочую границу: при
`DYNAMICS=ON` прежняя BASE-law возвращала для `RAW=63` коэффициент
`63/64 = 0.984375`. Поэтому хвост затухал даже при формально выключенном
level-GUARD. Это не было действием `FB CLIP / GUARD`: его прежний level gate
на точном RAW=63 уже был нулевым.

Исправление не основано на audio render: это source-level contract по
запрошенной границе `63/64`.

## Новый активный контракт

Для P1 и P2, всех retained delay modes `mnm | old | new`:

| RAW DFB | `DYNAMICS=ON` | `FB CLIP / GUARD` |
|---:|---|---|
| `0..62` | `BASE CURVE` плавно подходит к unity | полный bypass |
| `63` | **ровно `1.000000`** | полный bypass: follower, governor и final write clip не работают |
| `64` | первый настраиваемый over-unity entry; fresh `HOLD @64=65/64` | первая active GUARD point |
| `65..127` | retained `RAW/64` upper path | active level window `START/PLT @64/@127`, OFFSET, CURVE, AMOUNT, ATTACK/RELEASE |

`DYNAMICS=OFF` сохраняет отдельный legacy A/B путь `raw/63`.

Вследствие этого RAW=63 становится чистой отправной точкой для бесконечного
хвоста: включение `DYNAMICS` само по себе больше не ослабляет его. RAW=64
становится первой регулируемой GUARD-точкой; `FB CLIP / GUARD` может удержать
её, а без него over-unity entry остаётся свободным.

## Реализация

- `DelayFeedbackDynamics::baseCoefficientForRaw()` нормализует низкую BASE
  curve к exact `kUnityFeedback` в RAW=63.
- `HOLD @64` сохраняет тот же APVTS ID `dfb_base_hold_64`, но его active range
  теперь `1.0000..65/64`, fresh default `65/64`. Это соединяет RAW=64 с
  retained RAW=65 без notch.
- Добавлен единый `guardOwnsRaw()`: hard boundary `RAW <64` → false,
  `RAW >=64` → true. LUT interpolation, follower и coefficient governor
  используют его одинаково.
- `TrackDelay.inl` вычисляет `guardWriteClip` из того же predicate и передаёт
  его в OLD/MNM/NEW final write paths. Поэтому `FB CLIP` не остаётся включённым
  после возврата с RAW>=64 на RAW=63.
- Parameter IDs, automation routes, DFB embedded overlay, white text, DLY
  modes и `FxSlotEngine` boundary не менялись. Schema остаётся `45`.

Перед изменением сохранён immutable source snapshot:
`PATCH_HISTORY/SNAPSHOTS/ACTIVE_1.9.35_PRE_DFB_RAW63_UNITY_GUARD_ENTRY_2026-10-04/`.

## Статическая защита от регрессии

`tests/DelayFeedbackDspTests.cpp` обновлён как будущая regression specification
(не исполнялся): он требует exact unity в 63, zero guard ownership в `63` и
`63.5`, first ownership в 64, и active restrain для over-unity RAW=64/65.

`DFB_GUARD_CORE_STATIC_CHECK.py` дополнительно проверяет:

1. `kUnityFeedback` reference в low BASE law;
2. default/range `HOLD @64 = 65/64`;
3. `guardOwnsRaw()` с hard RAW>=64 boundary;
4. единый `guardWriteClip` во всех OLD/MNM/NEW delay paths;
5. DFB panel/graph English strings `63 UNITY / 64 ENTRY / 65+ GROWTH`;
6. byte-identical Synth/FX sources и source manifests.

Выполненный статический результат: `DFB_GUARD_CORE_STATIC_CHECK.py` —
**161 checks PASS**; `FM_MODE_CLEANUP_STATIC_CHECK.py` — manifest/metadata
**OK**; Synth/FX source manifests — `167 + 167` hashes PASS. Новый DFB audit
не исполняет C++ test source.

В этой записи не заявляется compilation, CTest, executable DSP test, VST3/DAW
проверка или audio render. Выполнены только статические/source checks.
