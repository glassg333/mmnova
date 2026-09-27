# Monomachine Nova — Windows builder с внешними Builds и sccache

Это обновлённый Windows-builder для Projucer + MSBuild. Он сохраняет прежний способ работы — открыть `build_vst3.bat` или перетащить на него папку проекта — но делает повторные сборки безопаснее и быстрее:

- физические `Builds` хранятся **вне распакованного ZIP**: по умолчанию в `E:\mm\build`;
- `<проект>\Builds` остаётся привычным путём для Projucer, но становится NTFS junction на внешний каталог;
- исходники и значимые опции сборки получают SHA-256 fingerprint; при изменении входных данных запускается безопасный `Rebuild`, а не использование сомнительных старых `.obj`;
- `sccache` кэширует результаты MSVC. Поэтому новый распакованный ZIP может получить cache hit для действительно неизменившихся единиц компиляции;
- в логе после сборки показывается статистика sccache: запросы, hits, misses и размер кэша;
- выключенная галочка удаления Builds теперь действительно сохраняет физические Builds **и до, и после** сборки.

> **Важно:** `E:\mm\build` — намеренно постоянное хранилище. Не помещайте его в одноразовую папку распакованного релиза и не направляйте его на сетевой/FAT-носитель. Для junction нужен локальный NTFS-том. Если диска `E:` нет, сначала создайте/подключите его либо осознанно измените `BuildCacheRoot` и `SccacheDir` в `build_config.json` на другой локальный NTFS-путь.

---

## Что лежит в пакете

| Файл | Назначение |
|---|---|
| `build_vst3.bat` | Обычный запуск в один клик; можно перетащить на него папку проекта или корень релиза. |
| `build_vst3.ps1` | Основной GUI/CLI builder. Нужен Windows PowerShell 5.1+. |
| `build_config.json` | Стартовая конфигурация. GUI обновляет её после запуска. |
| `README_original.txt` | Сохранённая инструкция исходного builder-а. |
| `build_config.original.json` | Сохранённый пример старой локальной конфигурации, только для справки. |

---

## Требования

1. **Windows 10/11**, PowerShell 5.1 или новее и локальный NTFS-диск `E:`.
2. **Visual Studio / Build Tools** с workload **«Разработка классических приложений на C++»** и MSBuild. Builder сам находит `MSBuild.exe` через `vswhere`, либо попросит указать его.
3. **JUCE Projucer**. При первом запуске builder попросит путь к `Projucer.exe`, если не найдёт его сам.
4. **sccache 0.18+** для кэширования между разными распаковками ZIP. Установить в PowerShell / Terminal:

   ```powershell
   winget install Mozilla.sccache
   sccache --version
   ```

   Нужна актуальная версия **не ниже 0.18**: именно она полноценно нормализует пути `SCCACHE_BASEDIRS` в аргументах MSVC. С более старой или нераспознанной версией builder не ломает сборку, но предупреждает, что cache hits надёжны только при том же абсолютном пути исходников.

5. Права администратора нужны только если готовый VST3 копируется в `C:\Program Files\Common Files\VST3`. GUI при необходимости запрашивает UAC. Создание NTFS junction `/J` не требует Developer Mode.

---

## Быстрый первый запуск

1. Распакуйте этот builder в любое постоянное место (например, `E:\mm\builder`).
2. Проверьте, что в `build_config.json` остаются:

   ```json
   "ExternalBuilds": true,
   "BuildCacheRoot": "E:\\mm\\build",
   "UseSccache": true,
   "SccacheDir": "E:\\mm\\build\\sccache",
   "DeleteBuilds": false
   ```

3. Установите sccache командой `winget install Mozilla.sccache`.
4. Откройте `build_vst3.bat` либо перетащите на него папку релиза / папку Synth или FX.
5. В окне оставьте включёнными:
   - **Builds вне ZIP: `E:\mm\build`**;
   - **sccache: общий кэш .obj между ZIP**;
   - выключенной галочку **«Удалять Builds перед/после сборки (без инкремента)»**.
6. Выберите VST3 и нажмите **СОБРАТЬ**.

При первой сборке builder:

- создаёт внешний путь наподобие
  `E:\mm\build\Monomachine-Nova-1.9.10\Monomachine_Nova_Synth`;
- создаёт junction `<проект>\Builds` на этот путь;
- при наличии старой обычной локальной папки `Builds` переносит её наружу **только если внешний каталог пуст**; два непустых дерева он намеренно не смешивает;
- считает fingerprint источников и запускает безопасный `Rebuild`;
- заполняет холодный sccache. Поэтому первый запуск не обязан быть быстрым.

Физические артефакты разделены по релизу и продукту (`Synth`/`FX`); общий только каталог объектного кэша:

```text
E:\mm\build\
├── Monomachine-Nova-1.9.10\
│   ├── Monomachine_Nova_Synth\
│   └── Monomachine_Nova_FX\
├── Monomachine-Nova-1.9.11\
│   ├── Monomachine_Nova_Synth\
│   └── Monomachine_Nova_FX\
├── sccache\                 ← общий локальный object cache
└── .nova-tools\             ← созданный builder-ом launcher для MSVC
```

Никогда не переиспользуется сырой `Builds` одного релиза как `Builds` другого: это защищает от старых промежуточных файлов. Повторное использование между ZIP происходит на уровне sccache и только когда ключ компиляции совпадает.

---

## Что означает «сохранять Builds» теперь

| Настройка GUI | Поведение |
|---|---|
| **«Удалять Builds перед/после сборки» выключена** (рекомендуется) | Внешний physical Builds сохраняется. Если fingerprint не изменился, MSBuild может выполнить обычную инкрементальную сборку. Если исходники/опции изменились, builder принудительно делает `Rebuild`; sccache всё равно способен быстро вернуть неизменившиеся объектники. |
| **«Удалять Builds перед/после сборки» включена** | Physical Builds очищается до сборки и после успешной обработки результатов. Это режим без инкрементальных артефактов; общий `E:\mm\build\sccache` при этом не удаляется. |
| Кнопка **«Очистить Builds cache»** | Очищает только physical Builds выбранного проекта, оставляет project-side junction и не удаляет общий sccache. |
| Ошибка Projucer/MSBuild | Builds остаётся для диагностики вне зависимости от галочки. |

Очистка действует на рассчитанный physical каталог релиза, а не на `<проект>\Builds` junction. Перед рекурсивной очисткой builder отклоняет physical root, если он сам оказался ссылкой, и отсоединяет вложенные reparse points, чтобы не пройти за границы cache root.

---

## Почему новый ZIP не берёт старые `.obj`

В ZIP-архивах timestamps иногда одинаковые или сохранены от старого дерева. Поэтому одной только проверки дат недостаточно. Перед Projucer/MSBuild builder строит SHA-256 из:

- файлов проекта, за исключением `Builds`, `.vs`, `.git`, логов и других явно не-входных папок;
- выбранных форматов и build-настроек;
- выбранного MSBuild и фактического `cl.exe`, включая SHA-256 самих tool binaries — обновление Visual Studio на том же пути тоже вызывает Rebuild.

Fingerprint записывается рядом с physical Builds только после успешной компиляции. При отличии fingerprint выбирается `Target:Rebuild` (либо `--clean-first` для CMake), а не доверяется оставшемуся object tree. Это обеспечивает корректность; ускорение от sccache — отдельный слой.

Ожидаемые причины cache miss: изменение `.cpp`/заголовка, настроек Projucer/MSBuild, компилятора, включений, определений, версии SDK или реального результата препроцессора. Cache hit не должен ожидаться для всего проекта после любого изменения.

---

## sccache: как он подключён и как смотреть статистику

Builder **не** подставляет `sccache.exe` вместо `cl.exe` напрямую. Он находит реальный абсолютный путь MSVC `cl.exe`, создаёт ASCII-safe launcher в `E:\mm\build\.nova-tools`, а MSBuild получает его через `CLToolPath` / `CLToolExe`. Launcher вызывает:

```text
sccache.exe <полный путь к настоящему cl.exe> <аргументы MSBuild>
```

Так sccache всегда знает настоящий компилятор. Пути launcher-а передаются через environment variables, поэтому профиль Windows с кириллицей в имени не портит `.cmd`-файл.

На каждый запуск builder задаёт:

```text
SCCACHE_DIR=E:\mm\build\sccache
SCCACHE_CACHE_SIZE=30G
SCCACHE_BASEDIRS=<текущие source и physical Builds>
SCCACHE_IGNORE_SERVER_IO_ERROR=1
```

Он перезапускает sccache server для применения текущих base directories и в конце выводит `sccache --show-stats`. Ищите в логе блок:

```text
sccache statistics:
  Compile requests ...
  Cache hits ...
  Cache misses ...
  Cache size ...
```

Builder намеренно перезапускает server для текущих `SCCACHE_BASEDIRS`, поэтому показанные числа относятся к текущей server-сессии / запуску builder-а, а не являются обещанным пожизненным счётчиком диска. Для ручной диагностики:

```powershell
sccache --show-stats
sccache --zero-stats
sccache --stop-server
```

Для полного сброса локального object cache остановите server, затем удалите **только** `E:\mm\build\sccache`. Не удаляйте `E:\mm\build` целиком, если хотите сохранить physical Builds других релизов.

---

## Консольный запуск

Пример VST3-сборки без GUI, с сохранением Builds:

```powershell
powershell -NoProfile -STA -ExecutionPolicy Bypass -File .\build_vst3.ps1 `
  -NoPicker `
  -Path "E:\mm\Monomachine-Nova-1.9.10" `
  -Formats VST3 `
  -KeepBuild `
  -ExternalBuilds `
  -UseSccache `
  -NoManifest
```

Если у релиза есть отдельные папки Synth и FX, можно передать обе:

```powershell
powershell -NoProfile -STA -ExecutionPolicy Bypass -File .\build_vst3.ps1 `
  -NoPicker `
  -Path "E:\mm\Monomachine-Nova-1.9.10\Monomachine_Nova_Synth", "E:\mm\Monomachine-Nova-1.9.10\Monomachine_Nova_FX" `
  -Formats VST3 -KeepBuild -ExternalBuilds -UseSccache -NoManifest
```

Указать другой размер кэша или явный путь к `sccache.exe` можно в `build_config.json`:

```json
{
  "BuildCacheRoot": "E:\\mm\\build",
  "SccacheDir": "E:\\mm\\build\\sccache",
  "SccacheSize": "50G",
  "SccacheExe": "C:\\tools\\sccache\\sccache.exe"
}
```

`ExternalBuilds`, `UseSccache` и `DeleteBuilds` также сохраняются GUI в этом файле. Не переносите старый `build_config.json` из прежнего builder-а поверх нового: в нём могут быть старые абсолютные пути и устаревшие switch-объекты. Используйте приложенный стартовый файл.

---

## Безопасное удаление / откат junction

`<проект>\Builds` — это junction, а не место, куда надо рекурсивно удалять данные. Чтобы удалить **только ссылку**, а не target:

```cmd
rmdir "E:\path\to\project\Builds"
```

**Не используйте `rmdir /s` для junction.** Затем, если это действительно нужно, отдельно удалите соответствующий physical каталог, например:

```cmd
rmdir /s /q "E:\mm\build\Monomachine-Nova-1.9.10\Monomachine_Nova_Synth"
```

Чтобы вернуться к старому builder-у:

1. Закройте Projucer, Visual Studio и DAW.
2. Удалите только project-side junction командой `rmdir "...\Builds"`.
3. При необходимости удалите physical folder конкретного продукта из `E:\mm\build`.
4. Запустите старый builder или Projucer; он снова создаст обычный `Builds` при генерации проекта.

Если новый builder сообщает, что существующий `Builds` junction указывает в другое место, он ничего не удалил. Проверьте target вручную (`cmd /c dir /al "<родительская-папка>"`) и либо сохраните нужные артефакты, либо удалите именно junction без `/s` перед повторным запуском.

---

## Рекомендуемая Windows-проверка

Этот пакет был статически проверен, включая разбор PowerShell-скрипта, но **не мог быть фактически собран в Linux-среде**. На первой Windows-машине сделайте короткую проверку:

1. Запустите одну чистую Synth/FX сборку и убедитесь, что в логе есть внешний путь Builds, версия sccache, `MSBuild -> sccache wrapper` и статистика.
2. Убедитесь, что `<проект>\Builds` отображается как junction на правильный `E:\mm\build\<release>\<product>`.
3. Запустите второй раз без изменения исходников: fingerprint должен сообщить, что он не изменился; статистика должна показывать cache hits или MSBuild должен быть инкрементальным.
4. Распакуйте идентичный исходный ZIP в другую папку и соберите его. При sccache 0.18+ ожидаются hits для неизменившихся translation units; physical raw Builds при этом остаётся изолированным по релизу.
5. Измените один исходный файл, соберите снова и убедитесь, что лог сообщает `безопасный Rebuild`, а готовый VST3 проходит обычную ручную проверку в DAW.

Если wrapper/MSBuild на конкретной установленной версии VS ведёт себя иначе, временно снимите галочку sccache: сборка продолжит работать обычным MSVC, а внешний Builds и fingerprint останутся активными. Сохраните `logs\msbuild_*.log` и `logs\errors_*.txt` для диагностики.
