# Comb Scanner Lab - сборка через Projucer (путь как в Monomachine-Nova).
#
# Что делает:
#   1) находит Projucer.exe (PATH -> C:\JUCE -> Program Files -> -ProjucerPath);
#   2) находит MSBuild из Visual Studio 18 (2026), иначе из 2022;
#   3) при необходимости правит путь к модулям JUCE внутри .jucer (-JuceModulesPath);
#   4) Projucer --resave генерирует Builds\VisualStudio2022\CombScannerLab.sln;
#   5) MSBuild собирает цель "CombScannerLab - VST3", x64, Release;
#   6) проверяет бандл: есть ли внутри DLL и moduleinfo.json (манифест VST3,
#      без него Ableton плагин не видит); если манифеста нет - собирает
#      "VST3 Manifest Helper" и генерирует его вручную;
#   7) печатает путь к .vst3 и (по желанию) копирует его в папку VST3.
#
# Примеры:
#   powershell -ExecutionPolicy Bypass -File tools\BuildProjucer.ps1
#   powershell -ExecutionPolicy Bypass -File tools\BuildProjucer.ps1 -Configuration Debug
#   powershell -ExecutionPolicy Bypass -File tools\BuildProjucer.ps1 -JuceModulesPath C:\JUCE\modules
#   powershell -ExecutionPolicy Bypass -File tools\BuildProjucer.ps1 -CopyToVST3Folder
#   powershell -ExecutionPolicy Bypass -File tools\BuildProjucer.ps1 -CopyToVST3Folder -Vst3Folder "C:\Program Files\Common Files\VST3\!build test"

param(
    [ValidateSet('Debug', 'Release')]
    [string] $Configuration = 'Release',

    [string] $ProjucerPath = '',
    [string] $MSBuildPath = '',
    [string] $JuceModulesPath = '',

    [switch] $SkipResave,
    [switch] $CopyToVST3Folder,
    [string] $Vst3Folder = 'C:\Program Files\Common Files\VST3'
)

$ErrorActionPreference = 'Stop'
Set-StrictMode -Version Latest

$ProjectRoot = (Resolve-Path (Join-Path $PSScriptRoot '..')).Path
$JucerFile   = Join-Path $ProjectRoot 'CombScannerLab.jucer'
$Solution    = Join-Path $ProjectRoot 'Builds\VisualStudio2022\CombScannerLab.sln'
$TargetName  = 'CombScannerLab - VST3'

function Write-Step([string] $Text) { Write-Host ''; Write-Host "=== $Text" -ForegroundColor Cyan }
function Write-Ok([string] $Text)   { Write-Host "  [ok] $Text" -ForegroundColor Green }
function Write-Note([string] $Text) { Write-Host "  [!] $Text" -ForegroundColor Yellow }

if (-not (Test-Path -LiteralPath $JucerFile)) { throw "Нет .jucer: $JucerFile" }

# ---------------------------------------------------------------------------
# Projucer
# ---------------------------------------------------------------------------
Write-Step 'Поиск Projucer'

function Test-Projucer([string] $Path) {
    if (-not (Test-Path -LiteralPath $Path)) { return $false }
    $root = Split-Path -Parent $Path
    return (Test-Path -LiteralPath (Join-Path $root 'modules\juce_core')) -or (Test-Path -LiteralPath (Join-Path $root '..\modules\juce_core'))
}

$projucer = ''

if ($ProjucerPath) {
    if (-not (Test-Path -LiteralPath $ProjucerPath)) { throw "ProjucerPath не найден: $ProjucerPath" }
    $projucer = $ProjucerPath
}
else {
    $inPath = Get-Command Projucer.exe -ErrorAction SilentlyContinue
    $candidates = @()
    if ($inPath) { $candidates += $inPath.Source }
    $candidates += @(
        'C:\JUCE\Projucer.exe',
        (Join-Path $env:ProgramFiles 'JUCE\Projucer.exe'),
        (Join-Path ${env:ProgramFiles(x86)} 'JUCE\Projucer.exe')
    )
    if ($env:JUCE_DIR) { $candidates += (Join-Path $env:JUCE_DIR 'Projucer.exe') }

    foreach ($candidate in $candidates) {
        if (Test-Projucer $candidate) { $projucer = $candidate; break }
    }
}

if (-not $projucer) {
    throw @'
Projucer.exe не найден.
Варианты:
  1) Соберите Projucer из вашего JUCE: C:\JUCE\extras\Projucer\Builds\VisualStudio2022\Projucer.sln
     (или откройте .jucer в Projucer GUI и нажмите "Save and open in IDE").
  2) Укажите путь вручную: -ProjucerPath "C:\JUCE\Projucer.exe"
'@
}

Write-Ok "Projucer: $projucer"

# ---------------------------------------------------------------------------
# MSBuild из VS 2026 (18), иначе из VS 2022
# ---------------------------------------------------------------------------
Write-Step 'Поиск MSBuild'

function Get-MSBuildCandidates {
    $list = @()

    $inPath = Get-Command MSBuild.exe -ErrorAction SilentlyContinue
    if ($inPath) { $list += $inPath.Source }

    foreach ($root in @((Join-Path $env:ProgramFiles 'Microsoft Visual Studio'),
                        (Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio'))) {
        if (-not (Test-Path -LiteralPath $root)) { continue }

        foreach ($versionDir in @('18', '2022', '17')) {
            foreach ($edition in @('Community', 'Professional', 'Enterprise', 'BuildTools', 'Preview')) {
                $candidate = Join-Path $root "$versionDir\$edition\MSBuild\Current\Bin\amd64\MSBuild.exe"
                if (Test-Path -LiteralPath $candidate) { $list += $candidate }
            }
        }
    }

    return $list
}

$msbuild = ''

if ($MSBuildPath) {
    if (-not (Test-Path -LiteralPath $MSBuildPath)) { throw "MSBuildPath не найден: $MSBuildPath" }
    $msbuild = $MSBuildPath
}
else {
    $candidates = @(Get-MSBuildCandidates)
    if ($candidates.Count -gt 0) { $msbuild = $candidates[0] }
}

if (-not $msbuild) {
    throw 'MSBuild.exe не найден. Укажите -MSBuildPath "C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\amd64\MSBuild.exe"'
}

Write-Ok "MSBuild: $msbuild"

# ---------------------------------------------------------------------------
# Путь к модулям JUCE в .jucer (если просили поправить)
# ---------------------------------------------------------------------------
if ($JuceModulesPath) {
    Write-Step "Модули JUCE -> $JuceModulesPath"

    if (-not (Test-Path -LiteralPath (Join-Path $JuceModulesPath 'juce_core'))) {
        Write-Note "В $JuceModulesPath не видно папки juce_core - проверь путь к исходникам JUCE."
    }

    $normalized = $JuceModulesPath.TrimEnd('\', '/') -replace '\\', '/'
    $text = Get-Content -LiteralPath $JucerFile -Raw -Encoding UTF8
    $patched = [regex]::Replace($text, '(<MODULEPATH id="[^"]+" path=")[^"]*(")', ('${1}' + $normalized + '${2}'))
    Set-Content -LiteralPath $JucerFile -Value $patched -Encoding UTF8 -NoNewline
    Write-Ok 'Пути MODULEPATH в CombScannerLab.jucer обновлены'
}

# ---------------------------------------------------------------------------
# Resave
# ---------------------------------------------------------------------------
if (-not $SkipResave) {
    Write-Step 'Projucer --resave'

    & $projucer --resave $JucerFile --fix-missing-dependencies
    if ($LASTEXITCODE -ne 0) {
        throw @"
Projucer завершился с кодом $LASTEXITCODE.
Скорее всего не совпал путь к модулям JUCE. Откройте CombScannerLab.jucer в Projucer GUI,
проверьте секцию Modules (путь к juce_audio_processors_headless указан как C:/JUCE/modules)
или запустите скрипт с -JuceModulesPath "путь\к\JUCE\modules".
"@
    }

    if (-not (Test-Path -LiteralPath $Solution)) { throw "Projucer не создал solution: $Solution" }
    Write-Ok 'Solution создан/обновлён'
}
else {
    Write-Step 'Resave пропущен (-SkipResave)'
    if (-not (Test-Path -LiteralPath $Solution)) { throw "Solution ещё не создан, а -SkipResave указан: $Solution" }
}

# ---------------------------------------------------------------------------
# Сборка
# ---------------------------------------------------------------------------
Write-Step "Сборка: $TargetName | $Configuration | x64"

& $msbuild $Solution "/t:$TargetName" "/p:Configuration=$Configuration" '/p:Platform=x64' '/m' '/nologo' '/v:m'
if ($LASTEXITCODE -ne 0) { throw "MSBuild завершился с кодом $LASTEXITCODE" }

# ---------------------------------------------------------------------------
# Результат: проверка бандла + генерация манифеста при необходимости
# ---------------------------------------------------------------------------
Write-Step 'Результат'

$buildsRoot = Join-Path $ProjectRoot 'Builds'
$bundles = @(Get-ChildItem -Path $buildsRoot -Recurse -Directory -Filter '*.vst3' -ErrorAction SilentlyContinue)

# Версия плагина из .jucer (нужна для манифеста)
$pluginVersion = '1.0.0'
try {
    [xml] $jucerXml = Get-Content -LiteralPath $JucerFile -Raw -Encoding UTF8
    if ($jucerXml.JUCERPROJECT.version) { $pluginVersion = [string] $jucerXml.JUCERPROJECT.version }
}
catch { Write-Note "Не удалось прочитать версию из .jucer, беру $pluginVersion" }

function Get-HelperExe {
    $found = Get-ChildItem -Path $buildsRoot -Recurse -File -ErrorAction SilentlyContinue |
             Where-Object { $_.Name -like '*Helper*.exe' -and $_.FullName -notlike '*.vst3\*' } |
             Select-Object -First 1
    return $found
}

foreach ($bundle in $bundles) {
    Write-Ok "VST3: $($bundle.FullName)"

    $dll = Get-ChildItem -Path $bundle.FullName -Recurse -File -Filter '*.dll' -ErrorAction SilentlyContinue | Select-Object -First 1
    $manifestPath = Join-Path $bundle.FullName 'Contents\Resources\moduleinfo.json'

    if ($dll) { Write-Host "       DLL: $($dll.Name)" -ForegroundColor DarkGray }
    else { Write-Note "Внутри бандла нет DLL - цель Shared Code не собралась." }

    if (Test-Path -LiteralPath $manifestPath) {
        Write-Host '       moduleinfo.json: есть (Ableton увидит плагин)' -ForegroundColor DarkGray
        continue
    }

    Write-Note 'moduleinfo.json нет - Ableton такой бандл может не просканировать. Пробую сгенерировать.'

    Write-Step 'Генерация манифеста VST3 (VST3 Manifest Helper)'

    & $msbuild $Solution '/t:CombScannerLab - VST3 Manifest Helper' "/p:Configuration=$Configuration" '/p:Platform=x64' '/m' '/nologo' '/v:m'
    if ($LASTEXITCODE -ne 0) { Write-Note "Helper не собрался (код $LASTEXITCODE) - соберите цель 'CombScannerLab - VST3 Manifest Helper' в Visual Studio вручную." }

    $helper = Get-HelperExe

    if (-not $helper) {
        Write-Note 'Helper.exe не найден. Откройте решение в Visual Studio и соберите цель "CombScannerLab - VST3 Manifest Helper".'
        continue
    }

    $bundlePath = $bundle.FullName -replace '\\', '/'
    $resourceDir = Join-Path $bundle.FullName 'Contents\Resources'
    if (-not (Test-Path -LiteralPath $resourceDir)) { New-Item -ItemType Directory -Path $resourceDir -Force | Out-Null }

    & $helper.FullName '-create' '-version' $pluginVersion '-path' $bundlePath '-output' $manifestPath
    if ($LASTEXITCODE -ne 0) { Write-Note "Helper вернул код $LASTEXITCODE" }

    if (Test-Path -LiteralPath $manifestPath) {
        Write-Ok "moduleinfo.json создан: $manifestPath"
    }
    else {
        Write-Note 'Манифест создать не удалось. Плагин всё равно может работать, но Ableton может его не увидеть.'
    }
}

if ($CopyToVST3Folder -and $bundles.Count -gt 0) {
    Write-Step "Копирование в $Vst3Folder"

    if (-not (Test-Path -LiteralPath $Vst3Folder)) { New-Item -ItemType Directory -Path $Vst3Folder -Force | Out-Null }

    foreach ($bundle in $bundles) {
        $destination = Join-Path $Vst3Folder $bundle.Name
        $null = robocopy $bundle.FullName $destination /E /NFL /NDL /NJH /NJS /NP

        if ($LASTEXITCODE -lt 8) { Write-Ok "Скопировано: $destination" }
        else { Write-Note "robocopy вернул $LASTEXITCODE для $destination" }
    }
}
elseif ($bundles.Count -gt 0) {
    Write-Host '  Подсказка: добавляйте -CopyToVST3Folder, чтобы сразу скопировать плагин.' -ForegroundColor DarkGray
    Write-Host '  ВАЖНО: копировать нужно ВСЮ папку CombScannerLab.vst3 (внутри DLL + moduleinfo.json),' -ForegroundColor DarkGray
    Write-Host '         а не один файл DLL - иначе Ableton не просканирует.' -ForegroundColor DarkGray
}

Write-Host ''
Write-Host 'Готово.' -ForegroundColor Green
