# Comb Scanner Lab - сборка VST3 / Standalone / оффлайн-рендерера.
#
# Скрипт сам находит cmake.exe (PATH -> Visual Studio 18/2026 -> Visual Studio 2022 -> vswhere),
# сам подбирает поддерживаемый генератор CMake и, если рядом нет исходников JUCE,
# скачивает JUCE через CMake FetchContent.
#
# Примеры:
#   powershell -ExecutionPolicy Bypass -File tools\BuildVST3.ps1
#   powershell -ExecutionPolicy Bypass -File tools\BuildVST3.ps1 -Configuration Debug
#   powershell -ExecutionPolicy Bypass -File tools\BuildVST3.ps1 -JuceDir C:\JUCE -CopyToVST3Folder
#   powershell -ExecutionPolicy Bypass -File tools\BuildVST3.ps1 -BuildAll
#
# Параметры:
#   -Configuration    Release (по умолчанию) или Debug
#   -CMakePath        явный путь к cmake.exe (если авто-поиск не устроил)
#   -JuceDir          путь к клонированному JUCE. Если не задан, берётся C:\JUCE или скачивается JUCE 8.0.15
#   -JuceTag          версия JUCE для скачивания (по умолчанию 8.0.15)
#   -Generator        явный генератор CMake, напр. "Visual Studio 18 2026"
#   -SkipConfigure    не запускать configure, только build (когда проект уже настроен)
#   -BuildAll         собрать ещё и Standalone + combscannerlab_render
#   -CopyToVST3Folder копировать собранный .vst3 в системную папку VST3
#   -Vst3Folder       куда копировать (по умолчанию C:\Program Files\Common Files\VST3)

param(
    [ValidateSet('Debug', 'Release')]
    [string] $Configuration = 'Release',

    [string] $CMakePath = '',
    [string] $JuceDir = '',
    [string] $JuceTag = '',
    [string] $Generator = '',

    [switch] $SkipConfigure,
    [switch] $BuildAll,
    [switch] $CopyToVST3Folder,
    [string] $Vst3Folder = 'C:\Program Files\Common Files\VST3'
)

$ErrorActionPreference = 'Stop'
Set-StrictMode -Version Latest

$ProjectRoot = (Resolve-Path (Join-Path $PSScriptRoot '..')).Path
$BuildDir    = Join-Path $ProjectRoot 'build\vs2026'
$PluginTarget = 'CombScannerLab_VST3'

function Write-Step([string] $Text) { Write-Host ''; Write-Host "=== $Text" -ForegroundColor Cyan }
function Write-Ok([string] $Text)   { Write-Host "  [ok] $Text" -ForegroundColor Green }
function Write-Warn2([string] $Text) { Write-Host "  [!] $Text" -ForegroundColor Yellow }

# ---------------------------------------------------------------------------
# 1. Ищем cmake.exe
# ---------------------------------------------------------------------------
function Get-CMakeCandidates {
    $list = New-Object System.Collections.Generic.List[string]

    $inPath = Get-Command cmake.exe -ErrorAction SilentlyContinue
    if ($inPath) { $list.Add($inPath.Source) }

    $roots = @(
        (Join-Path $env:ProgramFiles 'Microsoft Visual Studio'),
        (Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio')
    )

    foreach ($root in $roots) {
        if (-not (Test-Path -LiteralPath $root)) { continue }

        # "18" (VS 2026) и "2022" (VS 2022) - VS 2026 идёт первым
        foreach ($versionDir in @('18', '2022', '17')) {
            $dir = Join-Path $root $versionDir
            if (-not (Test-Path -LiteralPath $dir)) { continue }

            foreach ($edition in @('Community', 'Professional', 'Enterprise', 'BuildTools', 'Preview')) {
                $candidate = Join-Path $dir "$edition\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe"
                if (Test-Path -LiteralPath $candidate) { $list.Add($candidate) }
            }
        }
    }

    $vswhere = Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio\Installer\vswhere.exe'
    if (Test-Path -LiteralPath $vswhere) {
        $found = & $vswhere -latest -products * -find 'Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe' 2>$null
        foreach ($item in @($found)) {
            if ($item -and (Test-Path -LiteralPath $item)) { $list.Add($item) }
        }
    }

    return $list
}

function Test-GeneratorSupport([string] $Exe, [string] $GeneratorName) {
    $help = (& $Exe --help 2>$null | Out-String)
    return $help -match [regex]::Escape($GeneratorName)
}

Write-Step 'Поиск CMake'

$cmake = ''

if ($CMakePath) {
    if (-not (Test-Path -LiteralPath $CMakePath)) { throw "CMakePath не найден: $CMakePath" }
    $cmake = $CMakePath
}
else {
    $candidates = @(Get-CMakeCandidates)

    if ($candidates.Count -eq 0) {
        throw @'
cmake.exe не найден ни в PATH, ни внутри Visual Studio.
Что делать (любой вариант):
  1) Запустить этот скрипт из 'Developer PowerShell for VS 2026' - там cmake уже в PATH.
  2) Установить CMake: winget install Kitware.CMake
  3) Указать путь вручную: -CMakePath "C:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe"
'@
    }

    if ($Generator) {
        # Генератор задан вручную - берём первый cmake, который его знает.
        foreach ($candidate in $candidates) {
            if (Test-GeneratorSupport $candidate $Generator) { $cmake = $candidate; break }
        }

        if (-not $cmake) {
            throw "Ни один найденный cmake не поддерживает генератор '$Generator'. Обновите CMake (для VS 2026 нужен 4.2+) или уберите -Generator."
        }
    }
    else {
        # Сначала пробуем генератор VS 2026, затем VS 2022.
        foreach ($gen in @('Visual Studio 18 2026', 'Visual Studio 17 2022')) {
            foreach ($candidate in $candidates) {
                if (Test-GeneratorSupport $candidate $gen) {
                    $cmake = $candidate
                    $Generator = $gen
                    break
                }
            }

            if ($cmake) { break }
        }

        if (-not $cmake) {
            $cmake = $candidates[0]
            $Generator = 'Visual Studio 18 2026'
            Write-Warn2 'Ни один cmake не заявил поддержку VS 18/17 - попробую генератор по умолчанию.'
        }
    }
}

$cmakeVersion = (& $cmake --version | Select-Object -First 1)
Write-Ok "$cmakeVersion"
Write-Host "      $cmake"

if (-not $Generator) { $Generator = 'Visual Studio 18 2026' }
Write-Ok "Генератор: $Generator"
Write-Ok "Конфигурация: $Configuration | x64"
Write-Ok "Папка сборки: $BuildDir"

# ---------------------------------------------------------------------------
# 2. Определяем, откуда брать JUCE
# ---------------------------------------------------------------------------
Write-Step 'JUCE'

$juceArg = $null

if ($JuceDir) {
    if (-not (Test-Path -LiteralPath (Join-Path $JuceDir 'CMakeLists.txt'))) {
        throw "JuceDir=$JuceDir не похож на клон JUCE (нет CMakeLists.txt). Нужен именно git-клон, а не папка с Projucer."
    }
    $juceArg = $JuceDir
    Write-Ok "Локальный JUCE: $JuceDir"
}
elseif ($env:JUCE_DIR -and (Test-Path -LiteralPath (Join-Path $env:JUCE_DIR 'CMakeLists.txt'))) {
    $juceArg = $env:JUCE_DIR
    Write-Ok "JUCE из переменной окружения JUCE_DIR: $juceArg"
}
elseif (Test-Path -LiteralPath 'C:\JUCE\CMakeLists.txt') {
    $juceArg = 'C:\JUCE'
    Write-Ok "Найден локальный JUCE: C:\JUCE"
}
else {
    if (-not (Get-Command git.exe -ErrorAction SilentlyContinue)) {
        Write-Warn2 'git.exe не найден в PATH - если CMake не сможет скачать JUCE, задайте -JuceDir C:\путь\к\JUCE'
    }
    if ($JuceTag) {
        Write-Ok "JUCE будет скачан CMake-ом (FetchContent), версия $JuceTag"
    }
    else {
        Write-Ok 'JUCE будет скачан CMake-ом (FetchContent), версия по умолчанию из CMakeLists.txt (8.0.15)'
    }
}

# ---------------------------------------------------------------------------
# 3. Configure
# ---------------------------------------------------------------------------
if (-not $SkipConfigure) {
    Write-Step 'Configure'

    $cmakeArgs = @('-S', $ProjectRoot, '-B', $BuildDir, '-G', $Generator, '-A', 'x64')

    if ($juceArg) { $cmakeArgs += "-DJUCE_DIR=$juceArg"; $cmakeArgs += '-DCOMBLAB_FETCH_JUCE=OFF' }
    if ($JuceTag) { $cmakeArgs += "-DCOMBLAB_JUCE_TAG=$JuceTag" }
    if ($BuildAll) { $cmakeArgs += '-DCOMBLAB_BUILD_RENDERER=ON' }

    & $cmake @cmakeArgs
    if ($LASTEXITCODE -ne 0) {
        throw @"
Configure упал (код $LASTEXITCODE).
Частые причины:
  * CMake старше 4.2 не знает генератор "Visual Studio 18 2026" - запустите скрипт заново с -Generator "Visual Studio 17 2022"
    (solution всё равно соберётся MSBuild-ом из VS 2026), либо обновите CMake из комплекта VS 2026.
  * Нет интернета и не задан -JuceDir - укажите локальный клон JUCE 8.
"@
    }
    Write-Ok 'Configure прошёл'
}
else {
    Write-Step 'Configure пропущен (-SkipConfigure)'
}

# ---------------------------------------------------------------------------
# 4. Build
# ---------------------------------------------------------------------------
Write-Step "Сборка ($Configuration)"

& $cmake --build $BuildDir --config $Configuration --target $PluginTarget --parallel
if ($LASTEXITCODE -ne 0) {
    Write-Warn2 "Цель $PluginTarget не собралась - пробую собрать всё (CombScannerLab_All)."
    & $cmake --build $BuildDir --config $Configuration --parallel
    if ($LASTEXITCODE -ne 0) { throw "MSBuild/CMake завершился с кодом $LASTEXITCODE" }
}

if ($BuildAll) {
    & $cmake --build $BuildDir --config $Configuration --parallel
    if ($LASTEXITCODE -ne 0) { throw "Сборка дополнительных целей завершилась с кодом $LASTEXITCODE" }
}

# ---------------------------------------------------------------------------
# 5. Что получилось
# ---------------------------------------------------------------------------
Write-Step 'Результат'

$artefactsRoot = Join-Path $BuildDir 'CombScannerLab_artefacts'
$bundles = @()

if (Test-Path -LiteralPath $artefactsRoot) {
    $bundles = @(Get-ChildItem -Path $artefactsRoot -Recurse -Directory -Filter '*.vst3' -ErrorAction SilentlyContinue)
}

if ($bundles.Count -eq 0) {
    Write-Warn2 "Сборка прошла, но папка .vst3 не найдена ниже $artefactsRoot"
}
else {
    foreach ($bundle in $bundles) {
        Write-Ok "VST3: $($bundle.FullName)"

        $manifest = Get-ChildItem -Path $bundle.FullName -Recurse -File -Filter 'moduleinfo.json' -ErrorAction SilentlyContinue |
            Select-Object -First 1

        if ($manifest) { Write-Host "      moduleinfo.json: да" }
        else { Write-Warn2 'moduleinfo.json внутри бандла не найден - переносите папку .vst3 целиком' }
    }
}

$standalone = Get-ChildItem -Path $artefactsRoot -Recurse -File -Filter 'Comb Scanner Lab.exe' -ErrorAction SilentlyContinue |
    Select-Object -First 1
if ($standalone) { Write-Ok "Standalone: $($standalone.FullName)" }

$renderer = Get-ChildItem -Path $BuildDir -Recurse -File -Filter 'combscannerlab_render.exe' -ErrorAction SilentlyContinue |
    Select-Object -First 1
if ($renderer) { Write-Ok "Рендерер: $($renderer.FullName)" }

if ($CopyToVST3Folder -and $bundles.Count -gt 0) {
    Write-Step "Копирование в $Vst3Folder"

    if (-not (Test-Path -LiteralPath $Vst3Folder)) {
        New-Item -ItemType Directory -Path $Vst3Folder -Force | Out-Null
    }

    foreach ($bundle in $bundles) {
        $destination = Join-Path $Vst3Folder $bundle.Name
        $null = robocopy $bundle.FullName $destination /E /NFL /NDL /NJH /NJS /NP

        if ($LASTEXITCODE -lt 8) { Write-Ok "Скопировано: $destination" }
        else { Write-Warn2 "robocopy вернул $LASTEXITCODE для $destination" }
    }
}
elseif ($bundles.Count -gt 0) {
    Write-Host ''
    Write-Host '  Подсказка: чтобы сразу скопировать плагин, добавьте -CopyToVST3Folder (и при желании -Vst3Folder "путь")' -ForegroundColor DarkGray
}

Write-Host ''
Write-Host 'Готово.' -ForegroundColor Green
