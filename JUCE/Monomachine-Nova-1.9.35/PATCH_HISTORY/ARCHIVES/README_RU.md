# Исторические clean ZIP — реестр

Каждая подпапка содержит неизменённый delivery ZIP, его `SHA256SUMS.txt` и
краткое описание source baseline. Эти ZIP сохранены в рабочем каталоге, но
намеренно исключаются из нового clean delivery ZIP: иначе каждый следующий
пакет рекурсивно вкладывал бы все прежние пакеты и быстро рос бы без причины.

| Baseline | Архив | SHA-256 |
|---|---|---|
| До RAW=63 UNITY / GUARD entry | `PRE_DFB_RAW63_UNITY_GUARD_ENTRY_2026-10-04/Monomachine-Nova-1.9.35.zip` | `01a8be952db7510ddba62a3581fc182c029656d74f2344f65f8985f4209d66a9` |
| До ENV held-LMB release | `PRE_ENV_HELD_LMB_RELEASE_COMMIT_2026-10-04/Monomachine-Nova-1.9.35.zip` | `59f32cca48e25ee67ee7fd642432cb0c10f6fd2d4ba489f30879504594efa964` |
| До FloatingPanel white foreground ink | `PRE_FLOATING_FOREGROUND_INK_2026-10-04/Monomachine-Nova-1.9.35.zip` | `7c99818f5a121e30c676a68ba8510e91418be209c0f1f5419fefcbafa5a19f7b` |
| До DPTH UNI/INV stack alignment | `PRE_DPTH_OPTION_STACK_2026-10-04/Monomachine-Nova-1.9.35.zip` | `bd8aea2fef3ca29b33ea1335d60d51791f2480c6cc50137db5a386de6b64b1d2` |

Пакет содержит этот реестр, per-baseline README и `SHA256SUMS.txt`; двоичные
historical ZIP доступны в сохранённом worktree, а не рекурсивно внутри нового
архива. Это сохраняет provenance без включения старых delivery payload в новый
source delivery.
