param(
    [string]$Target
)

$ErrorActionPreference = 'Continue'

function Fail($msg) {
    Write-Host ''
    Write-Host '============================================================'
    foreach ($line in $msg) { Write-Host $line }
    Write-Host '============================================================'
    Read-Host 'Press Enter to close / Enter'
    exit 1
}

Write-Host 'Monomachine Nova 1.6.5 -- install'
if (-not $Target) {
    Fail @(
        'OSIBKA: nado peretashhit papku NA fail apply-1.6.5.bat.',
        'Chto delat: peretashhi myshkoy papku mmnova (koren repozitoriya,',
        'gde lezhit papka JUCE) ili papku Monomachine-Nova-1.6.4.'
    )
}
$bs = [char]92
$Target = $Target.TrimEnd($bs, '/')

$root = $null
if (Test-Path (Join-Path $Target 'JUCE/Monomachine-Nova-1.6.4/Monomachine_Nova_Synth/Source/PluginEditor.cpp')) {
    $root = Join-Path $Target 'JUCE/Monomachine-Nova-1.6.4'
}
if (Test-Path (Join-Path $Target 'Monomachine_Nova_Synth/Source/PluginEditor.cpp')) {
    $root = $Target
}
if (-not $root) {
    Fail @(
        "NE TA PAPKA: $Target",
        'Chto delat: peretashhi koren repozitoriya mmnova (s papkoy JUCE vnutri)',
        'ili srazu JUCE/Monomachine-Nova-1.6.4.'
    )
}
Write-Host "Target: $root"

$editor = Join-Path $root 'Monomachine_Nova_Synth/Source/PluginEditor.cpp'
$state = 'base'
if (Select-String -LiteralPath $editor -SimpleMatch -Quiet -Pattern 'lfo1Button') { $state = 'final' }
elseif (-not (Select-String -LiteralPath $editor -SimpleMatch -Quiet -Pattern 'showLfo3')) { $state = 'foreign' }

if ($state -eq 'final') {
    Fail @(
        'Pokhozhe, 1.6.5 UZHE primenena (v PluginEditor.cpp est lfo1Button).',
        'Chto delat: nichego. Esli hochesh nakatit sverhu --',
        'vosstanovi chistiy main s GitHub.'
    )
}
if ($state -eq 'foreign') {
    Write-Host ''
    Write-Host 'VNIMANIE: fayli ne pohozhi ni na chistiy main, ni na 1.6.5 --'
    Write-Host 'u tebya lokalnie pravki PluginEditor.cpp.'
    Write-Host 'Esli prodolzhit: rezervnaya kopiya sozdastsya, no tvoi lokalnie'
    Write-Host 'pravki ETIH faylov perepishutsya versiey 1.6.5.'
    $ans = Read-Host 'Prodolzhit? Y/N'
    if ($ans -notmatch '^[Yy]') { Write-Host 'Otmeneno.'; Read-Host 'Enter'; exit 1 }
}

$backup = "$root-backup-1.6.4"
Write-Host "Rezervnaya kopiya: $backup"
try {
    Copy-Item -LiteralPath $root -Destination $backup -Recurse -Force -ErrorAction Stop
} catch {
    Fail @('NE SMOG sdelat rezervnuyu kopiyu.', 'Chto delat: osvobodi mesto / provery prava i zapusti eshche raz.')
}

Write-Host 'Kopiruyu fayli 1.6.5...'
try {
    Copy-Item -Path (Join-Path $PSScriptRoot 'payload/*') -Destination $root -Recurse -Force -ErrorAction Stop
} catch {
    Fail @(
        'NE SMOG skopirovat fayli iz payload.',
        'Chto delat: ubedis, chto ryadom s apply-1.6.5.bat lezhit papka payload,',
        'ili vosstanovi rezervnuyu kopiyu:',
        "  rmdir /S /Q `"$root`"",
        "  ren `"$backup`" `"Monomachine-Nova-1.6.4`""
    )
}

foreach ($p in @('Monomachine_Nova_Synth','Monomachine_Nova_FX')) {
    $old = Join-Path $root "$p/Source/dsp/monomachine_fm_stat_par.hpp"
    if (Test-Path -LiteralPath $old) {
        if (Test-Path -LiteralPath "$old.bak-v7") { Remove-Item -LiteralPath $old -Force }
        else { Rename-Item -LiteralPath $old -NewName 'monomachine_fm_stat_par.hpp.bak-v7' }
    }
}

$ok = $true
$problems = @()
foreach ($p in @('Monomachine_Nova_Synth','Monomachine_Nova_FX')) {
    $ed = Join-Path $root "$p/Source/PluginEditor.cpp"
    $nd = Join-Path $root "$p/Source/NovaData.h"
    $cm = Join-Path $root "$p/CMakeLists.txt"
    $fp = Join-Path $root "$p/Source/dsp/monomachine_fm_par.hpp"
    $fo = Join-Path $root "$p/Source/dsp/monomachine_fm_stat_par.hpp"
    if (-not (Select-String -LiteralPath $ed -SimpleMatch -Quiet -Pattern 'lfo1Button')) { $ok = $false; $problems += "$p : PluginEditor.cpp ne vstal" }
    if (-not (Select-String -LiteralPath $nd -SimpleMatch -Quiet -Pattern 'dly_repitch')) { $ok = $false; $problems += "$p : NovaData.h ne vstal" }
    if (-not (Select-String -LiteralPath $cm -SimpleMatch -Quiet -Pattern '/utf-8')) { $ok = $false; $problems += "$p : CMakeLists.txt bez /utf-8" }
    if (-not (Test-Path -LiteralPath $fp)) { $ok = $false; $problems += "$p : ne hvataet monomachine_fm_par.hpp" }
    if (Test-Path -LiteralPath $fo) { $ok = $false; $problems += "$p : stariy monomachine_fm_stat_par.hpp na meste" }
}

$py = Get-Command python -ErrorAction SilentlyContinue
if ($py) {
    Write-Host ''
    Write-Host 'Polniy verifikator (python):'
    $v = Join-Path $root 'verify_dsp_mode_patch.py'
    & python $v (Join-Path $root 'Monomachine_Nova_Synth/Source')
    if ($LASTEXITCODE -ne 0) { $ok = $false }
    & python $v (Join-Path $root 'Monomachine_Nova_FX/Source')
    if ($LASTEXITCODE -ne 0) { $ok = $false }
} else {
    Write-Host ''
    Write-Host 'Python ne nayden -- polniy verifikator propushen, proverki markerami vishe.'
}

if ($ok) {
    Write-Host ''
    Write-Host '============================================================'
    Write-Host ' GOTOVO: 1.6.5 primenena, proverki proydeny.'
    Write-Host ' VAZHNO: otkroy OBA .jucer v Projucer i nazhmi Save -- togda'
    Write-Host ' flag /utf-8 popadet v proekt Visual Studio i krakozyabry'
    Write-Host ' ne vernutsya. Potom sobiray kak obichno.'
    Write-Host " Rezervnaya kopiya: $backup"
    Write-Host '============================================================'
} else {
    Write-Host ''
    Write-Host '============================================================'
    Write-Host ' PLOHO: posle primeneniya proverki ne soshlis:'
    foreach ($pr in $problems) { Write-Host "   $pr" }
    Write-Host ' Chto delat: verni rezervnuyu kopiyu i napishi agentu, chto ne soshlos:'
    Write-Host "   rmdir /S /Q `"$root`""
    Write-Host "   ren `"$backup`" `"Monomachine-Nova-1.6.4`""
    Write-Host '============================================================'
}
Read-Host 'Press Enter to close / Enter'
