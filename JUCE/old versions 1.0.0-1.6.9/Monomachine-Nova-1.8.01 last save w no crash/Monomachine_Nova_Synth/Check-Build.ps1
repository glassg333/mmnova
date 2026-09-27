param([string]$Vst3Path = "")
$ErrorActionPreference = 'Stop'
$root = $PSScriptRoot
Write-Host 'Monomachine Nova source audit / expected BUILD 1.6.5'
$projects = @(Get-ChildItem -LiteralPath $root -Filter '*.jucer' -File)
if ($projects.Count -ne 1) { throw 'Expected exactly one .jucer in this folder. Do not mix old and new projects.' }
[xml]$project = Get-Content -LiteralPath $projects[0].FullName -Raw
Write-Host ('Project: ' + $project.JUCERPROJECT.pluginName + ' / ' + $project.JUCERPROJECT.version)
$manifest = Get-Content -LiteralPath (Join-Path $root 'SOURCE_BUILD.json') -Raw | ConvertFrom-Json
foreach ($entry in $manifest.files.PSObject.Properties) {
    $file = Join-Path $root $entry.Name
    $actual = (Get-FileHash -LiteralPath $file -Algorithm SHA256).Hash.ToLowerInvariant()
    if ($actual -ne $entry.Value) { throw ('SOURCE MISMATCH: ' + $file) }
    Write-Host ('Source OK: ' + $entry.Name)
}
if ($Vst3Path -eq '') {
    Write-Host 'Optional: run again with -Vst3Path and the full path to your built or installed VST3.'
    exit 0
}
$item = Get-Item -LiteralPath $Vst3Path
if ($item.PSIsContainer) {
    $binaries = @(Get-ChildItem -LiteralPath $item.FullName -Recurse -File | Where-Object { $_.Extension -in @('.vst3', '.dll', '.so') })
    $metadata = @(Get-ChildItem -LiteralPath $item.FullName -Recurse -File -Filter 'moduleinfo.json')
    foreach ($info in $metadata) {
        Write-Host ('Metadata: ' + $info.FullName)
        Get-Content -LiteralPath $info.FullName | Select-String '"Name"|"Version"' | ForEach-Object { Write-Host $_.Line }
    }
} else { $binaries = @($item) }
if ($binaries.Count -eq 0) { throw 'No VST3 binary found at the supplied path.' }
foreach ($binary in $binaries) {
    $bytes = [System.IO.File]::ReadAllBytes($binary.FullName)
    $text = [System.Text.Encoding]::ASCII.GetString($bytes)
    $found = $text.Contains('BUILD 1.6.5 / Synth / mnm modes')
    Write-Host ('Binary: ' + $binary.FullName)
    Write-Host ('Last write: ' + $binary.LastWriteTime.ToString('s'))
    Write-Host ('SHA256: ' + (Get-FileHash -LiteralPath $binary.FullName -Algorithm SHA256).Hash)
    Write-Host ('Contains current UI build marker: ' + $found)
    if (-not $found) { Write-Warning 'This binary does not contain the expected UI marker. Check the build/copy path before loading it in the DAW.' }
}
Write-Host 'Read-only check complete. This script does not copy, delete or modify plugins.'
