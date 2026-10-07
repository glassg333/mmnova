param(
    [ValidateSet('Debug', 'Release')]
    [string] $Configuration = 'Release',

    [string] $ProjucerPath = 'C:\JUCE\Projucer.exe',

    [string] $MSBuildPath = 'C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\amd64\MSBuild.exe',

    [switch] $ForceResave
)

$ErrorActionPreference = 'Stop'
$ProjectRoot = (Resolve-Path (Join-Path $PSScriptRoot '..')).Path
$JucerFile = Join-Path $ProjectRoot 'CombScannerPro.jucer'
$BuildDirectory = Join-Path $ProjectRoot 'Builds\VisualStudio2022'
$Solution = Join-Path $BuildDirectory 'CombScannerPro.sln'
$ResaveStamp = Join-Path $BuildDirectory '.projucer-resaved'

if (-not (Test-Path $ProjucerPath)) { throw "Projucer not found: $ProjucerPath" }
if (-not (Test-Path $MSBuildPath)) { throw "MSBuild not found: $MSBuildPath" }
if (-not (Test-Path $JucerFile)) { throw "JUCE project not found: $JucerFile" }

$projectHash = (Get-FileHash -LiteralPath $JucerFile -Algorithm SHA256).Hash
$needsResave = $ForceResave -or -not (Test-Path $Solution) -or -not (Test-Path $ResaveStamp)
if (-not $needsResave) {
    $lastResaveHash = (Get-Content -LiteralPath $ResaveStamp -Raw).Trim()
    $needsResave = $projectHash -ne $lastResaveHash
}

if ($needsResave) {
    Write-Host "Projucer: $ProjucerPath"
    Write-Host 'Generating/updating the Visual Studio solution (project is new or changed)...'
    & $ProjucerPath --resave $JucerFile --fix-missing-dependencies
    if ($LASTEXITCODE -ne 0) { throw "Projucer failed with exit code $LASTEXITCODE" }
    if (-not (Test-Path $Solution)) { throw "Projucer did not create the solution: $Solution" }
    $projectHash = (Get-FileHash -LiteralPath $JucerFile -Algorithm SHA256).Hash
    Set-Content -LiteralPath $ResaveStamp -Value $projectHash -Encoding ascii
} else {
    Write-Host 'Skipping Projucer Resave: the .jucer project is unchanged since the last generated solution.'
}

Write-Host "Building VST3: $Configuration | x64"
& $MSBuildPath $Solution '/t:CombScannerPro - VST3' "/p:Configuration=$Configuration" '/p:Platform=x64' '/m' '/nologo' '/v:m'
if ($LASTEXITCODE -ne 0) { throw "MSBuild failed with exit code $LASTEXITCODE" }

$bundles = @(Get-ChildItem -Path $BuildDirectory -Directory -Filter 'CombScannerPro.vst3' -Recurse -ErrorAction SilentlyContinue)
if ($bundles.Count -gt 0) {
    foreach ($bundle in $bundles) {
        Write-Host "Built VST3 bundle: $($bundle.FullName)"
        $manifest = Get-ChildItem -Path $bundle.FullName -File -Filter 'moduleinfo.json' -Recurse -ErrorAction SilentlyContinue | Select-Object -First 1
        if (-not $manifest) { Write-Warning "moduleinfo.json not found inside $($bundle.FullName); keep the complete bundle and verify the Projucer VST3 helper target." }
    }
} else {
    Write-Warning 'Build succeeded but the CombScannerPro.vst3 bundle directory was not found below Builds\VisualStudio2022.'
}
