# =============================================================================
#  Сборка VST3 (Release x64) одним кликом: Projucer -> MSBuild -> Common Files\VST3
#  Cache edition: внешние Builds в E:\mm\build + sccache для повторных ZIP.
#
#  Заменяет ручную цепочку: открыть .jucer в Projucer -> "Save and open in IDE" ->
#  в Visual Studio выбрать Release / x64 / *_VST3 -> собрать -> скопировать .vst3.
#
#  Что делает:
#    1. Берёт папку проекта (перетащенную на .bat, либо спрашивает через окно выбора).
#       Можно бросить как корень (E:\mm\Monomachine-Nova-1.6.3), так и одну папку плагина.
#    2. Создаёт project\Builds junction в E:\mm\build\<release>\<product>.
#       При включённом «не удалять Builds» оставляет его между запусками.
#    3. Считает fingerprint исходников; при новом ZIP безопасно просит Rebuild.
#    4. Projucer --resave, затем MSBuild Release|x64; sccache возвращает
#       неизменившиеся объектники вместо повторной компиляции.
#    5. Проверяет, не занят ли старый плагин (Ableton и т.п.) — предлагает закрыть.
#    6. Копирует .vst3 в C:\Program Files\Common Files\VST3.
#    7. Показывает итог и статистику hit/miss sccache; Builds удаляет только
#       если пользователь явно включил «Удалить Builds».
#
#  Файл обязан быть сохранён в UTF-8 С BOM (иначе PowerShell 5.1 ломает кириллицу).
# =============================================================================
#requires -Version 5.1
[CmdletBinding()]
param(
    [string[]]$Path = @(),         # папки проектов (можно несколько, можно корень)
    [switch]$NoPicker,             # не показывать окно выбора (сразу собирать что передано)
    [string]$Vst3Dir = 'C:\Program Files\Common Files\VST3',
    [switch]$KeepBuild,            # сохранять physical Builds до/после сборки
    [switch]$NoInstall,            # только собрать
    [switch]$CleanOnly,            # только очистить physical Builds
    [switch]$CloseHosts,           # закрывать Ableton без вопроса
    [switch]$UseCMake,             # принудительно собирать через CMake (если нет .jucer)
    [switch]$Flat,                 # копировать один файл .vst3 (без папки-бандла Contents\x86_64-win)
    [string[]]$Formats = @('VST3'),# что собирать: VST3, Standalone, LV2, VST (что есть в проекте)
    [string]$OtherDir = '',        # куда класть Standalone (.exe) / LV2 / VST2; пусто = рядом с VST3-папкой в подпапке !build
    [switch]$NoManifest,           # пропустить VST3ManifestHelper (moduleinfo.json) -- экономит время
    [switch]$NoErrorReport,        # не открывать файл с ошибками при неудачной сборке
    [switch]$Worker,               # служебный: фоновый процесс сборки, вывод для окна
    [string]$OptFile = '',         # служебный: JSON с настройками от окна (надёжнее, чем аргументы с пробелами)
    [switch]$CleanModules,         # снести JuceLibraryCode\modules (локальная копия модулей JUCE, Projucer создаёт заново)
    [switch]$GlobalModules,        # переключить .jucer на общие модули JUCE (без локальной копии в проект)
    [switch]$AsyncDelete,          # удалять локальные modules в фоне (physical Builds очищается синхронно)
    [switch]$NoPdb,                 # Release без .pdb/.ilk -- меньше мусора и быстрее линковка
    [switch]$ExternalBuilds,        # Builds -- junction в постоянный внешний cache-root
    [string]$BuildCacheRoot = 'E:\mm\build',
    [switch]$UseSccache,            # кэш объектников между ZIP/релизами
    [string]$SccacheExe = '',       # необязательно: явный путь к sccache.exe
    [string]$SccacheDir = '',       # пусто = <BuildCacheRoot>\sccache
    [string]$SccacheSize = '30G'    # предел local disk cache
)

$ErrorActionPreference = 'Stop'
try { [Console]::OutputEncoding = [System.Text.Encoding]::UTF8; if ($Worker) { $w = New-Object IO.StreamWriter([Console]::OpenStandardOutput(), (New-Object Text.UTF8Encoding($false))); $w.AutoFlush = $true; [Console]::SetOut($w) } } catch { }

$Script:Root    = $PSScriptRoot; if (-not $Script:Root) { $Script:Root = (Get-Location).Path }
$Script:CfgPath = Join-Path $Script:Root 'build_config.json'
$Script:Cfg     = @{}
$Script:Log     = New-Object System.Collections.Generic.List[string]
$Script:Freed   = [int64]0
$Script:Results = New-Object System.Collections.Generic.List[object]
$Script:LogFile = $null
$Script:ErrorReports = New-Object System.Collections.Generic.List[string]
$Script:MsbuildLogs = New-Object System.Collections.Generic.List[string]

# ----------------------------------------------------------------- служебное --
function Log { param([string]$T, [string]$L = 'info')
    $line = '[' + (Get-Date).ToString('HH:mm:ss') + '] ' + $T
    $Script:Log.Add($line)
    $c = switch ($L) { 'ok' { 'Green' } 'warn' { 'Yellow' } 'err' { 'Red' } 'head' { 'Cyan' } default { 'Gray' } }
    if ($Worker) { [Console]::Out.WriteLine("@L|$L|$line"); [Console]::Out.Flush() } else { Write-Host $line -ForegroundColor $c }
    if ($Script:LogFile) { try { Add-Content -LiteralPath $Script:LogFile -Value $line -Encoding UTF8 } catch { } }
    Gui-Append $line $L
}
function Gui-Append([string]$line, [string]$L = 'info') {
    if (-not $Script:Con -or $Script:Con.IsDisposed) { return }
    $col = switch ($L) { 'ok' { [System.Drawing.Color]::FromArgb(110, 220, 120) } 'warn' { [System.Drawing.Color]::FromArgb(240, 200, 80) } 'err' { [System.Drawing.Color]::FromArgb(255, 110, 110) } 'head' { [System.Drawing.Color]::FromArgb(120, 190, 255) } 'dim' { [System.Drawing.Color]::FromArgb(130, 130, 140) } default { [System.Drawing.Color]::FromArgb(220, 220, 225) } }
    $Script:Con.SelectionStart = $Script:Con.TextLength; $Script:Con.SelectionLength = 0
    $Script:Con.SelectionColor = $col
    $Script:Con.AppendText($line + "`r`n")
    $Script:Con.ScrollToCaret()
    Gui-Pump
}
function Gui-Status([string]$text) {
    if ($Worker) { [Console]::Out.WriteLine("@S|$text"); [Console]::Out.Flush(); return }
    if ($Script:StatusLbl -and -not $Script:StatusLbl.IsDisposed) { $Script:StatusLbl.Text = $text }
    Gui-Pump
}
function Gui-Pump { if ($Script:Gui) { try { [System.Windows.Forms.Application]::DoEvents() } catch { } } }
function Fmt([int64]$b) {
    if ($b -ge 1GB) { return ('{0:N2} ГБ' -f ($b / 1GB)) }
    if ($b -ge 1MB) { return ('{0:N0} МБ' -f ($b / 1MB)) }
    if ($b -ge 1KB) { return ('{0:N0} КБ' -f ($b / 1KB)) }
    return "$b Б"
}
function TreeSize([string]$p) {
    $s = [int64]0
    try { Get-ChildItem -LiteralPath $p -Recurse -File -Force -EA SilentlyContinue | ForEach-Object { $s += $_.Length } } catch { }
    return $s
}
function Remove-Forever-Async([string]$p) {
    # Мгновенно: переименовать в <имя>.~del_xxx (атомарно), а реальное удаление -- в отдельном скрытом процессе.
    if (-not (Test-Path -LiteralPath $p)) { return [int64]0 }
    $size = TreeSize $p
    $full = (Resolve-Path -LiteralPath $p).Path
    $tomb = $full + '.~del_' + [guid]::NewGuid().ToString('N').Substring(0, 8)
    try { Rename-Item -LiteralPath $full -NewName (Split-Path -Leaf $tomb) -EA Stop } catch { return (Remove-Forever $p) }  # занято -> обычный путь (с ошибкой, если не выйдет)
    Start-Process -FilePath 'cmd.exe' -ArgumentList @('/c', "rmdir /s /q `"$tomb`"") -WindowStyle Hidden | Out-Null
    $Script:Freed += $size
    return $size
}
function Remove-Tombstones([string]$dir) {
    # добить остатки *.~del_* от прошлых запусков
    try { foreach ($t in (Get-ChildItem -LiteralPath $dir -Directory -Filter '*.~del_*' -Force -EA SilentlyContinue)) { Start-Process -FilePath 'cmd.exe' -ArgumentList @('/c', "rmdir /s /q `"$($t.FullName)`"") -WindowStyle Hidden | Out-Null } } catch { }
}
function Remove-Forever([string]$p) {
    # Удаление МИМО корзины. rmdir из cmd — самый быстрый и не спотыкается о длинные пути JUCE.
    if (-not (Test-Path -LiteralPath $p)) { return [int64]0 }
    $size = TreeSize $p
    $full = (Resolve-Path -LiteralPath $p).Path
    cmd.exe /c "rmdir /s /q `"$full`"" 2>$null | Out-Null
    if (Test-Path -LiteralPath $p) { cmd.exe /c "rmdir /s /q `"\\?\$full`"" 2>$null | Out-Null }
    if (Test-Path -LiteralPath $p) { try { Remove-Item -LiteralPath $p -Recurse -Force -EA Stop } catch { } }
    if (Test-Path -LiteralPath $p) { throw "Не удалось удалить $p — папку держит другая программа (Visual Studio? Projucer?). Закрой её и повтори." }
    $Script:Freed += $size
    return $size
}
function Run([string]$exe, [string[]]$argv) {
    # Внешняя программа. Снимаем 'Stop', иначе любая строка MSBuild в stderr = «фатальная ошибка».
    $prev = $ErrorActionPreference; $ErrorActionPreference = 'Continue'
    try { $out = & $exe @argv 2>&1; $code = $LASTEXITCODE } catch { $out = @($_.Exception.Message); $code = 1 }
    finally { $ErrorActionPreference = $prev }
    if ($null -eq $code) { $code = 0 }
    return [pscustomobject]@{ Out = @($out | ForEach-Object { [string]$_ }); Code = [int]$code }
}
function Run-Live([string]$exe, [string[]]$argv, [string]$label) {
    # Запуск с живым выводом: в консоли видно, что компилируется, каждые 15 с -- сердцебиение.
    $tmpDir = if ($env:TEMP) { $env:TEMP } else { [IO.Path]::GetTempPath() }
    $of = Join-Path $tmpDir ('nova_live_' + [guid]::NewGuid().ToString('N') + '.txt')
    $ef = $of + '.err'
    $quoted = $argv | ForEach-Object { if ($_ -match '\s') { '"' + $_ + '"' } else { $_ } }
    $prc = Start-Process -FilePath $exe -ArgumentList $quoted -PassThru -NoNewWindow -RedirectStandardOutput $of -RedirectStandardError $ef
    $all = New-Object System.Collections.Generic.List[string]
    $sw = [Diagnostics.Stopwatch]::StartNew(); $lastBeat = 0; $files = 0
    $fs = $null
    try {
        Start-Sleep -Milliseconds 300
        $fs = New-Object IO.FileStream($of, [IO.FileMode]::Open, [IO.FileAccess]::Read, [IO.FileShare]::ReadWrite)
        $rd = New-Object IO.StreamReader($fs, [Text.Encoding]::Default)
        while ($true) {
            while (($line = $rd.ReadLine()) -ne $null) {
                $all.Add($line); $t = $line.Trim()
                if ($t -match '^\S+\.(cpp|c|cc|mm)$') { $files++; if (-not $Worker) { Write-Host ("`r  [{0}] {1,-70}" -f $files, $t.Substring(0, [Math]::Min(70, $t.Length))) -NoNewline -ForegroundColor DarkGray }; Gui-Status ("{0}: {1} мин {2:00} с  |  файлов: {3}  |  {4}" -f $label, [int]($sw.Elapsed.TotalSeconds / 60), ([int]$sw.Elapsed.TotalSeconds % 60), $files, $t); continue }
                if ($t -match 'error [A-Z]+\d+|error MSB|fatal error') { if (-not $Worker) { Write-Host '' }; Log "  $t" 'err' }
                elseif ($t -match '\.vst3$|\.lib$|\.dll$|->') { if (-not $Worker) { Write-Host '' }; Log "  $t" 'ok' }
            }
            if ($prc.HasExited) { Start-Sleep -Milliseconds 200; while (($line = $rd.ReadLine()) -ne $null) { $all.Add($line) }; break }
            $sec = [int]$sw.Elapsed.TotalSeconds
            if ($sec - $lastBeat -ge 15) { $lastBeat = $sec; if (-not $Worker) { Write-Host '' }; Log ("  ...$label идёт: {0} мин {1:00} с, файлов скомпилировано: {2}" -f [int]($sec / 60), ($sec % 60), $files) }
            Gui-Pump
            Start-Sleep -Milliseconds 300
        }
        if (-not $Worker) { Write-Host '' }
        Gui-Status ("{0}: завершено за {1} мин {2:00} с, файлов: {3}" -f $label, [int]($sw.Elapsed.TotalSeconds / 60), ([int]$sw.Elapsed.TotalSeconds % 60), $files)
    } finally { if ($fs) { $fs.Close() } }
    if (Test-Path $ef) { Get-Content $ef -EA SilentlyContinue | ForEach-Object { $all.Add($_) }; Remove-Item $ef -Force -EA SilentlyContinue }
    Remove-Item $of -Force -EA SilentlyContinue
    return [pscustomobject]@{ Out = @($all); Code = [int]$prc.ExitCode }
}
function Write-ErrorReport([string]$Name, [string]$Cmd, [string[]]$Out, [string[]]$Errs) {
    # Файл со всеми ошибками/предупреждениями компилятора + контекст -- удобно целиком скинуть в чат агенту.
    try {
        $dir = Join-Path $Script:Root 'logs'
        $file = Join-Path $dir ("errors_" + ($Name -replace '[^\w\-]', '_') + '_' + (Get-Date).ToString('yyyy-MM-dd_HH-mm-ss') + '.txt')
        $warns = @($Out | Where-Object { $_ -match 'warning [A-Z]+\d+' } | Select-Object -Unique)
        $uErrs = @($Errs | Select-Object -Unique)
        $L = New-Object System.Collections.Generic.List[string]
        $L.Add("ОШИБКА СБОРКИ: $Name"); $L.Add("Дата: $((Get-Date).ToString('yyyy-MM-dd HH:mm:ss'))")
        $L.Add("MSBuild: $Cmd"); $L.Add('')
        $L.Add("=== ОШИБКИ ($($uErrs.Count)) ==="); foreach ($e in $uErrs) { $L.Add($e.Trim()) }
        $L.Add(''); $L.Add("=== ПРЕДУПРЕЖДЕНИЯ ($($warns.Count)) ==="); foreach ($w in ($warns | Select-Object -First 100)) { $L.Add($w.Trim()) }
        $L.Add(''); $L.Add('=== ПОЛНЫЙ ВЫВОД (последние 300 строк) ==='); foreach ($o in ($Out | Select-Object -Last 300)) { $L.Add($o) }
        Set-Content -LiteralPath $file -Value ($L -join "`r`n") -Encoding UTF8
        $Script:ErrorReports.Add($file)
        Log "Отчёт об ошибках: $file" 'warn'
        if ($Worker) { [Console]::Out.WriteLine("@E|$file"); [Console]::Out.Flush() }
        if (-not $NoErrorReport -and -not $Worker) {
            try { if ($Script:Gui) { [System.Windows.Forms.Clipboard]::SetText(($uErrs -join "`r`n")) ; Log 'Ошибки скопированы в буфер обмена' 'ok' } } catch { }
            Start-Process notepad.exe $file
        }
    } catch { Log "Не смог записать отчёт об ошибках: $($_.Exception.Message)" 'warn' }
}
function Is-Admin {
    $id = [Security.Principal.WindowsIdentity]::GetCurrent()
    return (New-Object Security.Principal.WindowsPrincipal($id)).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)
}
function Load-Cfg { if (Test-Path -LiteralPath $Script:CfgPath) { try { (Get-Content -LiteralPath $Script:CfgPath -Raw | ConvertFrom-Json).PSObject.Properties | ForEach-Object { $Script:Cfg[$_.Name] = $_.Value } } catch { } } }
function Save-Cfg { try { ($Script:Cfg | ConvertTo-Json) | Set-Content -LiteralPath $Script:CfgPath -Encoding UTF8 } catch { } }

# ----------------------------------------------------------------- окна ------
$Script:Gui = $false
try { Add-Type -AssemblyName System.Windows.Forms; Add-Type -AssemblyName System.Drawing; [System.Windows.Forms.Application]::EnableVisualStyles(); $Script:Gui = $true } catch { }
if ($Worker) { $Script:Gui = $false }
$Script:Dark = @{ Bg = [System.Drawing.Color]::FromArgb(24, 24, 27); Panel = [System.Drawing.Color]::FromArgb(32, 32, 36); Fg = [System.Drawing.Color]::FromArgb(235, 235, 235); Btn = [System.Drawing.Color]::FromArgb(50, 50, 56); Accent = [System.Drawing.Color]::FromArgb(36, 120, 60); Err = [System.Drawing.Color]::FromArgb(160, 40, 40) }
function Apply-Dark($ctrl) {
    if ($ctrl -is [System.Windows.Forms.Form]) { $ctrl.BackColor = $Script:Dark.Bg; $ctrl.ForeColor = $Script:Dark.Fg }
    foreach ($c in $ctrl.Controls) {
        $c.ForeColor = $Script:Dark.Fg
        if ($c -is [System.Windows.Forms.Button]) { $c.FlatStyle = 'Flat'; $c.FlatAppearance.BorderColor = [System.Drawing.Color]::FromArgb(90, 90, 96); if ($c.BackColor -eq [System.Drawing.SystemColors]::Control -or $c.BackColor -eq [System.Drawing.Color]::Empty) { $c.BackColor = $Script:Dark.Btn } }
        elseif ($c -is [System.Windows.Forms.TextBox] -or $c -is [System.Windows.Forms.CheckedListBox] -or $c -is [System.Windows.Forms.ListBox]) { $c.BackColor = $Script:Dark.Panel; $c.BorderStyle = 'FixedSingle' }
        elseif ($c -is [System.Windows.Forms.CheckBox]) { $c.BackColor = $Script:Dark.Bg; $c.FlatStyle = 'Standard' }
        elseif ($c -is [System.Windows.Forms.Label] -and $c.Tag -ne 'keep') { $c.BackColor = $Script:Dark.Bg }
        elseif ($c -is [System.Windows.Forms.Panel] -or $c -is [System.Windows.Forms.FlowLayoutPanel]) { $c.BackColor = $Script:Dark.Bg }
        if ($c.Controls.Count) { Apply-Dark $c }
    }
}
function Ask-YesNo([string]$title, [string]$text) {
    if ($Worker) { return $false }
    if (-not $Script:Gui) { $a = Read-Host ($text + ' [y/N]'); return ($a -match '^[yYдД]') }
    return ([System.Windows.Forms.MessageBox]::Show($text, $title, 'YesNo', 'Question') -eq 'Yes')
}
function Pick-Folder([string]$descr) {
    if ($Worker) { return '' }
    if (-not $Script:Gui) { return (Read-Host $descr) }
    $d = New-Object System.Windows.Forms.FolderBrowserDialog
    $d.Description = $descr; $d.ShowNewFolderButton = $false
    if ($Script:Cfg['LastPath'] -and (Test-Path -LiteralPath $Script:Cfg['LastPath'])) { $d.SelectedPath = $Script:Cfg['LastPath'] }
    if ($d.ShowDialog() -eq 'OK') { return $d.SelectedPath }
    return ''
}
function Pick-File([string]$title, [string]$filter) {
    if ($Worker) { return '' }
    if (-not $Script:Gui) { return (Read-Host $title) }
    $d = New-Object System.Windows.Forms.OpenFileDialog
    $d.Title = $title; $d.Filter = $filter
    if ($d.ShowDialog() -eq 'OK') { return $d.FileName }
    return ''
}
function Show-Summary([string]$head, [bool]$ok, [string]$body) {
    if (-not $Script:Gui) { Write-Host ''; Write-Host $head; Write-Host $body; return }
    $f = New-Object System.Windows.Forms.Form
    $f.Text = 'Сборка VST3'; $f.Width = 900; $f.Height = 620; $f.StartPosition = 'CenterScreen'
    $lbl = New-Object System.Windows.Forms.Label
    $lbl.Text = $head; $lbl.Dock = 'Top'; $lbl.Height = 44; $lbl.TextAlign = 'MiddleLeft'; $lbl.Padding = '10,0,0,0'
    $lbl.Font = New-Object System.Drawing.Font('Segoe UI', 12, [System.Drawing.FontStyle]::Bold)
    $lbl.ForeColor = 'White'; $lbl.Tag = 'keep'; $lbl.BackColor = $(if ($ok) { $Script:Dark.Accent } else { $Script:Dark.Err })
    $t = New-Object System.Windows.Forms.TextBox
    $t.Multiline = $true; $t.ReadOnly = $true; $t.ScrollBars = 'Vertical'; $t.Dock = 'Fill'
    $t.Font = New-Object System.Drawing.Font('Consolas', 9.5); $t.Text = $body
    $p = New-Object System.Windows.Forms.FlowLayoutPanel
    $p.Dock = 'Bottom'; $p.Height = 46; $p.FlowDirection = 'RightToLeft'; $p.Padding = '6,6,6,6'
    $bClose = New-Object System.Windows.Forms.Button; $bClose.Text = 'Закрыть'; $bClose.Width = 110; $bClose.Height = 30
    $bClose.Add_Click({ $f.Close() })
    $bDir = New-Object System.Windows.Forms.Button; $bDir.Text = 'Открыть папку VST3'; $bDir.Width = 160; $bDir.Height = 30
    $bDir.Add_Click({ if (Test-Path -LiteralPath $Vst3Dir) { Start-Process explorer.exe $Vst3Dir } })
    $bLog = New-Object System.Windows.Forms.Button; $bLog.Text = 'Открыть журнал'; $bLog.Width = 130; $bLog.Height = 30
    $bLog.Add_Click({ if ($Script:LogFile) { Start-Process notepad.exe $Script:LogFile } })
    $bCopy = New-Object System.Windows.Forms.Button; $bCopy.Text = 'Скопировать весь лог'; $bCopy.Width = 160; $bCopy.Height = 30
    $bCopy.Add_Click({ try { [System.Windows.Forms.Clipboard]::SetText($t.Text); $bCopy.Text = 'Скопировано!' } catch { } })
    $bMs = New-Object System.Windows.Forms.Button; $bMs.Text = 'Лог MSBuild (как в VS)'; $bMs.Width = 170; $bMs.Height = 30
    $bMs.Add_Click({ foreach ($m in $Script:MsbuildLogs) { if (Test-Path -LiteralPath $m) { Start-Process notepad.exe $m } } })
    $bMs.Enabled = ($Script:MsbuildLogs.Count -gt 0)
    if ($Script:ErrorReports.Count) {
        $bErr = New-Object System.Windows.Forms.Button; $bErr.Text = 'Скопировать ошибки'; $bErr.Width = 150; $bErr.Height = 30
        $bErr.Add_Click({ try { [System.Windows.Forms.Clipboard]::SetText((($Script:ErrorReports | ForEach-Object { Get-Content -LiteralPath $_ -Raw }) -join "`r`n`r`n")); $bErr.Text = 'Скопировано!' } catch { } })
        $bErrOpen = New-Object System.Windows.Forms.Button; $bErrOpen.Text = 'Открыть отчёт'; $bErrOpen.Width = 130; $bErrOpen.Height = 30
        $bErrOpen.Add_Click({ foreach ($er in $Script:ErrorReports) { Start-Process notepad.exe $er } })
        $p.Controls.AddRange(@($bClose, $bCopy, $bErr, $bErrOpen, $bMs, $bDir, $bLog))
    } else { $p.Controls.AddRange(@($bClose, $bCopy, $bMs, $bDir, $bLog)) }
    $f.Controls.AddRange(@($t, $p, $lbl))
    $f.AcceptButton = $bClose
    $p.Height = 50; $f.Width = 1000
    Apply-Dark $f
    $f.Add_Shown({ $f.Activate(); $t.Select(0, 0) })
    [void]$f.ShowDialog()
}

# ----------------------------------------------------------- инструменты -----
function Find-VsWhere { $p = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"; if (Test-Path $p) { return $p }; return '' }
function Find-MSBuild {
    if ($Script:Cfg['MSBuild'] -and (Test-Path -LiteralPath $Script:Cfg['MSBuild'])) { return $Script:Cfg['MSBuild'] }
    $vw = Find-VsWhere
    if ($vw) {
        # -prerelease обязателен для VS 2026 Insiders; -products * ловит Build Tools
        foreach ($extra in @(@('-prerelease'), @())) {
            $r = Run $vw (@('-latest', '-products', '*') + $extra + @('-requires', 'Microsoft.Component.MSBuild', '-find', 'MSBuild\**\Bin\MSBuild.exe'))
            foreach ($l in $r.Out) { if ($l -and (Test-Path -LiteralPath $l.Trim())) { return $l.Trim() } }
        }
    }
    # прямой поиск по всем версиям (2026 = "18", 2022 = "2022")
    $found = @()
    foreach ($root in @('C:\Program Files\Microsoft Visual Studio', 'C:\Program Files (x86)\Microsoft Visual Studio')) {
        if (-not (Test-Path $root)) { continue }
        $found += Get-ChildItem -Path $root -Recurse -Filter 'MSBuild.exe' -Depth 5 -EA SilentlyContinue |
            Where-Object { $_.FullName -match '\\MSBuild\\Current\\Bin\\(amd64\\)?MSBuild\.exe$' }
    }
    if ($found.Count) { return ($found | Sort-Object { $_.VersionInfo.FileVersion } -Descending | Select-Object -First 1).FullName }
    $f = Pick-File 'Где лежит MSBuild.exe? (обычно ...\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe)' 'MSBuild.exe|MSBuild.exe'
    if ($f -and (Test-Path -LiteralPath $f)) { return $f }
    return ''
}
function Get-Toolsets([string]$msbuild) {
    # Какие наборы инструментов (v143/v145...) реально установлены рядом с этим MSBuild
    $vsRoot = $msbuild
    for ($i = 0; $i -lt 4; $i++) { $vsRoot = Split-Path -Parent $vsRoot }
    $ts = @()
    $d = Join-Path $vsRoot 'MSBuild\Microsoft\VC'
    if (Test-Path $d) {
        $ts = Get-ChildItem -Path $d -Recurse -Directory -Filter 'Platforms' -Depth 2 -EA SilentlyContinue |
            ForEach-Object { Get-ChildItem -LiteralPath (Join-Path $_.FullName 'x64\PlatformToolsets') -Directory -EA SilentlyContinue } |
            ForEach-Object { $_.Name } | Where-Object { $_ -match '^v\d{3}$' } | Sort-Object -Unique -Descending
    }
    return @($ts)
}
function Find-Projucer([string]$tree) {
    if ($Script:Cfg['Projucer'] -and (Test-Path -LiteralPath $Script:Cfg['Projucer'])) { return $Script:Cfg['Projucer'] }
    $cands = New-Object System.Collections.Generic.List[string]
    foreach ($base in @($tree, (Split-Path -Parent $tree), 'C:\JUCE', 'D:\JUCE', 'E:\JUCE', "$env:USERPROFILE\JUCE", "$env:USERPROFILE\Downloads\JUCE", "$env:USERPROFILE\Documents\JUCE", 'C:\Program Files\JUCE')) {
        if (-not $base) { continue }
        $cands.Add((Join-Path $base 'Projucer.exe'))
        $cands.Add((Join-Path $base 'JUCE\Projucer.exe'))
        $cands.Add((Join-Path $base 'JUCE\extras\Projucer\Builds\VisualStudio2022\x64\Release\App\Projucer.exe'))
        $cands.Add((Join-Path $base 'extras\Projucer\Builds\VisualStudio2022\x64\Release\App\Projucer.exe'))
    }
    foreach ($c in $cands) { if (Test-Path -LiteralPath $c) { return $c } }
    # Projucer запоминает себя в реестре/настройках? Нет. Спросим один раз и запомним.
    $f = Pick-File 'Где лежит Projucer.exe? (спрошу один раз, потом запомню)' 'Projucer.exe|Projucer.exe'
    if ($f -and (Test-Path -LiteralPath $f)) { return $f }
    return ''
}
function Find-CMake {
    $c = Get-Command cmake.exe -EA SilentlyContinue; if ($c) { return $c.Source }
    foreach ($p in @('C:\Program Files\CMake\bin\cmake.exe', 'C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe')) { if (Test-Path $p) { return $p } }
    return ''
}

# ------------------------------------------------------ cache / sccache ------
# The cache is deliberately outside an extracted source ZIP.  Builds stay
# version-scoped under E:\mm\build; only sccache objects are shared between
# releases.  That prevents stale VS artefacts from one release being mistaken
# for another while still making unchanged translation units cheap.
function Get-CfgString([string]$key, [string]$fallback = '') {
    if ($Script:Cfg.ContainsKey($key) -and $null -ne $Script:Cfg[$key] -and [string]$Script:Cfg[$key]) { return [string]$Script:Cfg[$key] }
    return $fallback
}
function Get-CfgBool([string]$key, [bool]$fallback = $false) {
    if (-not $Script:Cfg.ContainsKey($key) -or $null -eq $Script:Cfg[$key]) { return $fallback }
    $value = $Script:Cfg[$key]
    # Old versions serialised [switch] as { "IsPresent": true/false }.
    # Read that shape faithfully once, then the next Save-Cfg writes a boolean.
    $legacy = $value.PSObject.Properties['IsPresent']
    if ($legacy) { return [bool]$legacy.Value }
    return [bool]$value
}
function Get-SafeLeaf([string]$name) { return (([string]$name) -replace '[<>:"/\\|?*]', '_') }
function Is-ReparseDirectory([string]$path) {
    try { $i = Get-Item -LiteralPath $path -Force -EA Stop; return ($i.PSIsContainer -and (($i.Attributes -band [IO.FileAttributes]::ReparsePoint) -ne 0)) }
    catch { return $false }
}
function Get-ReleaseCacheKey([string]$projectDir) {
    # Normally a project is ...\Monomachine-Nova-1.9.10\Monomachine_Nova_Synth.
    # Walk upward to keep raw Builds separate per release even when a user drags
    # the shared JUCE directory or a project folder rather than the release root.
    $dir = Split-Path -Parent $projectDir
    for ($i = 0; $i -lt 6 -and $dir; $i++) {
        $leaf = Split-Path -Leaf $dir
        if ($leaf -match '(?i)(?<![0-9])v?\d+(?:\.\d+){1,3}(?![0-9])') { return (Get-SafeLeaf $leaf) }
        $next = Split-Path -Parent $dir
        if ($next -eq $dir) { break }
        $dir = $next
    }
    # A non-versioned project gets a stable, conservative fallback.  If its
    # immediate parent is simply JUCE, use its parent rather than putting every
    # release into E:\mm\build\JUCE.
    $fallback = Split-Path -Parent $projectDir
    if ((Split-Path -Leaf $fallback) -ieq 'JUCE') { $fallback = Split-Path -Parent $fallback }
    return (Get-SafeLeaf (Split-Path -Leaf $fallback))
}
function Get-ExternalBuildRoot($proj) {
    $releaseKey = Get-ReleaseCacheKey $proj.Dir
    $productKey = Get-SafeLeaf (Split-Path -Leaf $proj.Dir)
    $root = [string]$script:BuildCacheRoot
    if (-not $root) { $root = 'E:\mm\build' }
    return (Join-Path (Join-Path $root $releaseKey) $productKey)
}
function Ensure-ExternalBuildLink([string]$projectDir, [string]$physicalRoot) {
    # Keep the .jucer targetFolder as Builds\VisualStudio2022.  A junction lets
    # Projucer generate the same relative VS project, while the actual 600+ MB
    # live in E:\mm\build rather than inside the disposable ZIP extraction.
    $link = Join-Path $projectDir 'Builds'
    $parent = Split-Path -Parent $physicalRoot
    if (-not (Test-Path -LiteralPath $parent)) { New-Item -ItemType Directory -Path $parent -Force | Out-Null }
    $item = Get-Item -LiteralPath $link -Force -EA SilentlyContinue
    if ($item) {
        if (Is-ReparseDirectory $link) {
            $target = ''
            try { $target = [string](@($item.Target)[0]) } catch { }
            if (-not $target) { throw "Builds уже является junction, но PowerShell не смог определить его target: $link. Удали junction вручную и запусти снова." }
            try {
                $actual = [IO.Path]::GetFullPath($target).TrimEnd('\\')
                $expected = [IO.Path]::GetFullPath($physicalRoot).TrimEnd('\\')
                if (-not ($actual -ieq $expected)) { throw "Builds junction уже указывает в другое место: $target. Ожидался $physicalRoot. Скрипт ничего не удалил." }
            } catch {
                if ($_.Exception.Message -like 'Builds junction*') { throw }
                throw "Не удалось проверить target junction $link : $($_.Exception.Message)"
            }
            if (-not (Test-Path -LiteralPath $physicalRoot)) { New-Item -ItemType Directory -Path $physicalRoot -Force | Out-Null }
            return $link
        }
        if (-not $item.PSIsContainer) { throw "Путь $link существует, но это не папка Builds." }
        # First migration from a previous local build is safe only when the
        # external destination is absent/empty. Never silently merge trees.
        $hasExternal = (Test-Path -LiteralPath $physicalRoot) -and (@(Get-ChildItem -LiteralPath $physicalRoot -Force -EA SilentlyContinue).Count -gt 0)
        if ($hasExternal) { throw "Есть локальная папка $link и непустой внешний cache $physicalRoot. Скрипт не смешивает их автоматически: перенеси/удали один из них и запусти снова." }
        if (Test-Path -LiteralPath $physicalRoot) { Remove-Item -LiteralPath $physicalRoot -Force -EA Stop }
        Move-Item -LiteralPath $link -Destination $physicalRoot -EA Stop
        Log "Локальный Builds перенесён во внешний cache: $physicalRoot" 'ok'
    }
    if (-not (Test-Path -LiteralPath $physicalRoot)) { New-Item -ItemType Directory -Path $physicalRoot -Force | Out-Null }
    try { New-Item -ItemType Junction -Path $link -Target $physicalRoot -EA Stop | Out-Null }
    catch {
        # Windows directory junctions need no Developer Mode/admin privilege;
        # mklink is a fallback for older Windows PowerShell installations.
        cmd.exe /c "mklink /J `"$link`" `"$physicalRoot`"" | Out-Null
        if ($LASTEXITCODE -ne 0 -or -not (Test-Path -LiteralPath $link)) { throw "Не удалось создать junction $link -> ${physicalRoot}: $($_.Exception.Message)" }
    }
    Log "Builds вынесен: $link  ->  $physicalRoot" 'ok'
    return $link
}
function Remove-ChildReparsePoints([string]$root) {
    # rmdir /s has varied junction behaviour across Windows versions.  Detach
    # any child links first, without /s, so cache cleanup can never walk out of
    # the designated physical Builds root through a nested reparse point.
    $stack = New-Object 'System.Collections.Generic.Stack[string]'
    $stack.Push($root)
    while ($stack.Count -gt 0) {
        $dir = $stack.Pop()
        foreach ($child in @(Get-ChildItem -LiteralPath $dir -Force -EA Stop)) {
            $reparse = (($child.Attributes -band [IO.FileAttributes]::ReparsePoint) -ne 0)
            if ($reparse) {
                try {
                    if ($child.PSIsContainer) {
                        cmd.exe /c "rmdir `"$($child.FullName)`"" 2>$null | Out-Null
                        if (Test-Path -LiteralPath $child.FullName) { throw 'rmdir did not remove the reparse point' }
                    } else { Remove-Item -LiteralPath $child.FullName -Force -EA Stop }
                } catch { throw "Не удалось безопасно убрать reparse point $($child.FullName): $($_.Exception.Message)" }
                continue
            }
            if ($child.PSIsContainer) { $stack.Push($child.FullName) }
        }
    }
}
function Clear-PhysicalBuildRoot([string]$physicalRoot) {
    $freed = [int64]0
    if (Test-Path -LiteralPath $physicalRoot) {
        if (Is-ReparseDirectory $physicalRoot) { throw "Отказ от очистки: physical Builds root сам является junction/symlink: $physicalRoot" }
        Remove-ChildReparsePoints $physicalRoot
        $freed = Remove-Forever $physicalRoot
    }
    New-Item -ItemType Directory -Path $physicalRoot -Force | Out-Null
    return $freed
}
function Get-ProjectSourceFingerprint([string]$projectDir, [string]$signature) {
    # Hash project inputs, not the ZIP timestamp. This prevents an extracted
    # archive with backdated files from letting MSBuild reuse a stale .obj.
    $root = (Resolve-Path -LiteralPath $projectDir).Path.TrimEnd('\\')
    $skip = @('Builds', '.vs', '.git', 'logs', 'node_modules', '__pycache__', '.cache')
    $stack = New-Object 'System.Collections.Generic.Stack[string]'
    $files = New-Object 'System.Collections.Generic.List[object]'
    $stack.Push($root)
    while ($stack.Count -gt 0) {
        $dir = $stack.Pop()
        foreach ($child in @(Get-ChildItem -LiteralPath $dir -Force -EA Stop)) {
            if ($child.PSIsContainer) {
                if (($skip -contains $child.Name) -or (($GlobalModules -and $child.Name -eq 'modules' -and (Split-Path -Leaf $dir) -eq 'JuceLibraryCode')) -or (($child.Attributes -band [IO.FileAttributes]::ReparsePoint) -ne 0)) { continue }
                $stack.Push($child.FullName)
            } elseif (-not (($child.Attributes -band [IO.FileAttributes]::ReparsePoint) -ne 0)) {
                $files.Add($child)
            }
        }
    }
    $entries = New-Object 'System.Collections.Generic.List[string]'
    foreach ($file in @($files | Sort-Object FullName)) {
        $rel = $file.FullName.Substring($root.Length) -replace '^[\\/]+', ''
        $rel = $rel.Replace('\', '/')
        $hash = (Get-FileHash -LiteralPath $file.FullName -Algorithm SHA256 -EA Stop).Hash.ToLowerInvariant()
        $entries.Add($rel + "`0" + $hash)
    }
    $entries.Add('BUILD_OPTIONS' + "`0" + $signature)
    $payload = [Text.Encoding]::UTF8.GetBytes(($entries -join "`n"))
    $sha = [Security.Cryptography.SHA256]::Create()
    try { return ([BitConverter]::ToString($sha.ComputeHash($payload))).Replace('-', '').ToLowerInvariant() }
    finally { $sha.Dispose() }
}
function Get-ToolFingerprint([string]$toolPath) {
    if (-not $toolPath) { return 'missing' }
    try {
        if (Test-Path -LiteralPath $toolPath) {
            return ((Resolve-Path -LiteralPath $toolPath).Path + '=' + (Get-FileHash -LiteralPath $toolPath -Algorithm SHA256 -EA Stop).Hash.ToLowerInvariant())
        }
    } catch { }
    return ($toolPath + '=unhashable')
}
function Get-BuildInputState([string]$projectDir, [string]$physicalRoot, [string]$signature) {
    $stateFile = Join-Path $physicalRoot '.nova_source_fingerprint.sha256'
    $now = Get-ProjectSourceFingerprint $projectDir $signature
    $old = ''
    if (Test-Path -LiteralPath $stateFile) { try { $old = (Get-Content -LiteralPath $stateFile -Raw -EA Stop).Trim() } catch { } }
    return [pscustomobject]@{ File = $stateFile; Value = $now; Previous = $old; Changed = ($now -ne $old) }
}
function Save-BuildInputState($state) {
    Set-Content -LiteralPath $state.File -Value $state.Value -Encoding ASCII
}
function Find-Sccache {
    if ($script:SccacheExe -and (Test-Path -LiteralPath $script:SccacheExe)) { return $script:SccacheExe }
    $cmd = Get-Command 'sccache.exe' -EA SilentlyContinue | Select-Object -First 1
    if ($cmd -and $cmd.Source -and (Test-Path -LiteralPath $cmd.Source)) { return $cmd.Source }
    $candidates = @(
        (Join-Path $env:USERPROFILE 'scoop\apps\sccache\current\sccache.exe'),
        (Join-Path $env:LOCALAPPDATA 'Microsoft\WinGet\Packages\Mozilla.sccache_8wekyb3d8bbwe\sccache.exe'),
        'C:\ProgramData\chocolatey\bin\sccache.exe'
    )
    foreach ($candidate in $candidates) { if ($candidate -and (Test-Path -LiteralPath $candidate)) { return $candidate } }
    return ''
}
function Find-MsvcCl([string]$msbuild) {
    $roots = New-Object 'System.Collections.Generic.List[string]'
    if ($env:VCToolsInstallDir -and (Test-Path -LiteralPath $env:VCToolsInstallDir)) { $roots.Add($env:VCToolsInstallDir) }
    $d = Split-Path -Parent $msbuild
    while ($d -and (Split-Path -Leaf $d) -ne 'MSBuild') { $next = Split-Path -Parent $d; if ($next -eq $d) { break }; $d = $next }
    if ($d) {
        $vsRoot = Split-Path -Parent $d
        $tools = Join-Path $vsRoot 'VC\Tools\MSVC'
        if (Test-Path -LiteralPath $tools) {
            foreach ($v in @(Get-ChildItem -LiteralPath $tools -Directory -EA SilentlyContinue | Sort-Object Name -Descending)) { $roots.Add($v.FullName) }
        }
    }
    foreach ($root in @($roots | Select-Object -Unique)) {
        foreach ($rel in @('bin\Hostx64\x64\cl.exe', 'bin\Hostx86\x64\cl.exe')) {
            $candidate = Join-Path $root $rel
            if (Test-Path -LiteralPath $candidate) { return $candidate }
        }
    }
    return ''
}
function Write-SccacheWrapper([string]$cacheRoot) {
    $tools = Join-Path $cacheRoot '.nova-tools'
    New-Item -ItemType Directory -Path $tools -Force | Out-Null
    $wrapper = Join-Path $tools 'nova_sccache_cl.cmd'
    # Keep this CMD file ASCII-only.  Actual paths are inherited in Unicode
    # environment variables, so a Windows user profile with Cyrillic or other
    # non-ASCII characters cannot corrupt a path baked into the wrapper.
    $body = @"
@echo off
setlocal DisableDelayedExpansion
"%NOVA_SCCACHE_EXE%" "%NOVA_REAL_CL%" %*
set "RC=%ERRORLEVEL%"
endlocal & exit /b %RC%
"@
    [IO.File]::WriteAllText($wrapper, $body, (New-Object Text.ASCIIEncoding))
    return [pscustomobject]@{ Directory = $tools; File = $wrapper }
}
function Initialize-Sccache($projects, [string]$msbuild) {
    if (-not $script:UseSccache) { return $null }
    $sccache = Find-Sccache
    if (-not $sccache) { Log 'sccache.exe не найден: продолжаю обычным MSVC. Установи: winget install Mozilla.sccache' 'warn'; return $null }
    $realCl = Find-MsvcCl $msbuild
    if (-not $realCl) { Log 'sccache найден, но не найден настоящий MSVC cl.exe рядом с MSBuild; кэш отключён, сборка продолжится обычным MSVC.' 'warn'; return $null }
    $version = Run $sccache @('--version')
    if ($version.Code -ne 0) { Log "sccache.exe найден, но --version завершился с ошибкой: $($version.Out -join ' '). Продолжаю обычным MSVC." 'warn'; return $null }
    $versionText = ($version.Out -join ' ')
    $supportsBaseDirs = $false
    $m = [regex]::Match($versionText, '(\d+)\.(\d+)\.(\d+)')
    # 0.18 also strips basedirs from compiler arguments (/I, source file,
    # /Fo); older releases still tie a cache key to the extracted ZIP path.
    if ($m.Success) { $supportsBaseDirs = (([int]$m.Groups[1].Value -gt 0) -or (([int]$m.Groups[1].Value -eq 0) -and ([int]$m.Groups[2].Value -ge 18))) }
    $cacheRoot = [string]$script:BuildCacheRoot
    if (-not $cacheRoot) { $cacheRoot = 'E:\mm\build' }
    $disk = [string]$script:SccacheDir
    if (-not $disk) { $disk = Join-Path $cacheRoot 'sccache' }
    New-Item -ItemType Directory -Path $disk -Force | Out-Null
    $env:SCCACHE_DIR = $disk
    $env:SCCACHE_CACHE_SIZE = $(if ($script:SccacheSize) { $script:SccacheSize } else { '30G' })
    $env:SCCACHE_IGNORE_SERVER_IO_ERROR = '1' # a cache outage must never stop a release build
    if ($supportsBaseDirs) {
        $bases = New-Object 'System.Collections.Generic.List[string]'
        foreach ($project in $projects) {
            $bases.Add((Resolve-Path -LiteralPath $project.Dir).Path)
            if ($script:ExternalBuilds) { $bases.Add((Get-ExternalBuildRoot $project)) }
            else { $bases.Add((Join-Path $project.Dir 'Builds')) }
        }
        $env:SCCACHE_BASEDIRS = (@($bases | Select-Object -Unique) -join ';')
    } else {
        Remove-Item Env:SCCACHE_BASEDIRS -EA SilentlyContinue
        Log "sccache $versionText не поддерживает полный SCCACHE_BASEDIRS (нужен >= 0.18): кэш будет работать только при том же абсолютном пути исходников." 'warn'
    }
    # Older sccache daemons keep SCCACHE_BASEDIRS at server startup. A fresh
    # server for this invocation makes a new extracted release share safely.
    [void](Run $sccache @('--stop-server'))
    $started = Run $sccache @('--start-server')
    if ($started.Code -ne 0) { Log "sccache server не стартовал заранее: $($started.Out -join ' '). Попробую lazy-start; при проблеме MSVC будет запущен напрямую." 'warn' }
    # MSBuild and its CL wrapper inherit these Unicode environment variables.
    $env:NOVA_SCCACHE_EXE = $sccache
    $env:NOVA_REAL_CL = $realCl
    $wrapper = Write-SccacheWrapper $cacheRoot
    Log "sccache: $versionText" 'ok'
    Log "sccache disk cache: $disk (лимит $env:SCCACHE_CACHE_SIZE)" 'ok'
    if ($supportsBaseDirs) { Log "sccache path normalization: SCCACHE_BASEDIRS для текущих Synth/FX исходников и внешних Builds" 'dim' }
    Log "MSVC wrapper: $($wrapper.File) -> $realCl" 'dim'
    return [pscustomobject]@{ Enabled = $true; Exe = $sccache; RealCl = $realCl; Wrapper = $wrapper; Disk = $disk }
}
function Log-SccacheStats($cache) {
    if (-not $cache -or -not $cache.Enabled) { return }
    $r = Run $cache.Exe @('--show-stats')
    if ($r.Code -ne 0) { Log "sccache --show-stats: $($r.Out -join ' ')" 'warn'; return }
    Log 'sccache statistics:' 'head'
    foreach ($line in $r.Out) { if ($line -match 'Compile requests|Cache hits|Cache misses|Cache size|Non-cacheable|Errors') { Log ('  ' + $line.Trim()) 'dim' } }
}

# ------------------------------------------------------ занятость файла -----
$Script:HostNames = @('Ableton Live 12 Suite', 'Ableton Live 12', 'Ableton Live 11 Suite', 'Ableton Live 11', 'Ableton Live 10 Suite', 'Ableton Live', 'Live',
    'reaper', 'FL64', 'Bitwig Studio', 'Cubase13', 'Cubase12', 'Studio One', 'AudioPluginHost', 'Projucer', 'devenv', 'MSBuild', 'VBCSCompiler', 'pluginval')
$Script:FormatTable = @(
    [pscustomobject]@{ Key = 'VST3';       Suffix = '_VST3.vcxproj';             OutDir = 'VST3';               Pattern = '*.vst3'; Label = 'VST3' },
    [pscustomobject]@{ Key = 'Standalone'; Suffix = '_StandalonePlugin.vcxproj'; OutDir = 'Standalone Plugin';  Pattern = '*.exe';  Label = 'Standalone (.exe)' },
    [pscustomobject]@{ Key = 'LV2';        Suffix = '_LV2.vcxproj';              OutDir = 'LV2 Plugin';         Pattern = '*.lv2';  Label = 'LV2' },
    [pscustomobject]@{ Key = 'VST';        Suffix = '_VST.vcxproj';              OutDir = 'VST';                Pattern = '*.dll';  Label = 'VST2 (legacy)' }
)
function Vst3-Binary([string]$bundle) {
    if (-not (Test-Path -LiteralPath $bundle)) { return '' }
    if (-not (Get-Item -LiteralPath $bundle).PSIsContainer) { return $bundle }
    $b = Get-ChildItem -LiteralPath $bundle -Recurse -File -Filter '*.vst3' -EA SilentlyContinue | Select-Object -First 1
    if ($b) { return $b.FullName }; return ''
}
function Is-Locked([string]$file) {
    if (-not $file -or -not (Test-Path -LiteralPath $file)) { return $false }
    try { $fs = [IO.File]::Open($file, 'Open', 'ReadWrite', 'None'); $fs.Close(); return $false } catch { return $true }
}
function Who-Holds([string]$file) {
    $res = @()
    $leaf = [IO.Path]::GetFileName($file)
    foreach ($p in (Get-Process -EA SilentlyContinue)) {
        $isHost = $false
        foreach ($n in $Script:HostNames) { if ($p.ProcessName -like ($n + '*')) { $isHost = $true; break } }
        if (-not $isHost) { continue }
        $exact = $false
        try { foreach ($m in $p.Modules) { if ($m.FileName -like ('*' + $leaf)) { $exact = $true; break } } } catch { }
        $res += [pscustomobject]@{ Name = $p.ProcessName; Id = $p.Id; Exact = $exact }
    }
    return $res
}
function Free-File([string]$file) {
    # Возвращает $true, если файл свободен (или его удалось освободить).
    if (-not (Is-Locked $file)) { return $true }
    $h = Who-Holds $file
    $exact = @($h | Where-Object Exact)
    $list = if ($exact.Count) { ($exact | ForEach-Object { "$($_.Name) (PID $($_.Id))" }) -join ', ' } elseif ($h.Count) { ($h | ForEach-Object { "$($_.Name)?" }) -join ', ' } else { 'неизвестный процесс' }
    Log "Файл плагина ЗАНЯТ: $list" 'warn'
    $toKill = if ($exact.Count) { $exact } else { $h }
    if ($toKill.Count -eq 0) { return $false }
    $do = $CloseHosts
    if (-not $do) { $do = Ask-YesNo 'Файл занят другим процессом' ("Старый плагин сейчас держит:`r`n  $list`r`n`r`nЗакрыть эту программу (БЕЗ сохранения) и заменить плагин?`r`n`r`nНет = плагин останется старым.") }
    if (-not $do) { return $false }
    foreach ($x in $toKill) { try { Stop-Process -Id $x.Id -Force -EA Stop; Log "Закрыт: $($x.Name) (PID $($x.Id))" 'ok' } catch { Log "Не смог закрыть $($x.Name): $($_.Exception.Message)" 'warn' } }
    Start-Sleep -Seconds 2
    return (-not (Is-Locked $file))
}

# ------------------------------------------------------------- проекты ------
function Find-Projects([string[]]$dirs) {
    # Рекурсивно ищет папки с .jucer (или CMakeLists.txt с juce_add_plugin). Папки JUCE/Builds/Source пропускает.
    $skip = @('JUCE', 'Builds', 'JuceLibraryCode', 'Source', '.git', 'modules', 'node_modules', 'Assets', 'Resources')
    $found = @{}
    foreach ($dir in $dirs) {
        $dir = ([string]$dir).Trim().Trim('"')
        if (-not $dir) { continue }
        if (-not (Test-Path -LiteralPath $dir)) { Log "Папка не найдена: $dir" 'warn'; continue }
        $item = Get-Item -LiteralPath $dir
        if (-not $item.PSIsContainer) { $dir = Split-Path -Parent $dir }
        $stack = New-Object System.Collections.Generic.Stack[object]
        $stack.Push(@{ D = $dir; L = 0 })
        while ($stack.Count) {
            $cur = $stack.Pop()
            $j = Get-ChildItem -LiteralPath $cur.D -File -Filter '*.jucer' -EA SilentlyContinue | Select-Object -First 1
            $cm = Join-Path $cur.D 'CMakeLists.txt'
            if ($j) { $found[$cur.D.ToLower()] = [pscustomobject]@{ Dir = $cur.D; Jucer = $j.FullName; CMake = $cm }; continue }
            if ((Test-Path $cm) -and (Select-String -Path $cm -Pattern 'juce_add_plugin' -Quiet)) { $found[$cur.D.ToLower()] = [pscustomobject]@{ Dir = $cur.D; Jucer = ''; CMake = $cm }; continue }
            if ($cur.L -ge 3) { continue }
            foreach ($sub in (Get-ChildItem -LiteralPath $cur.D -Directory -EA SilentlyContinue)) {
                if ($sub.Name -in $skip) { continue }
                $stack.Push(@{ D = $sub.FullName; L = $cur.L + 1 })
            }
        }
    }
    return @($found.Values | Sort-Object Dir)
}

function Show-Main([string[]]$initial, [scriptblock]$OnBuild) {
    # Единое окно: настройки сверху, встроенная консоль снизу. Сборка идёт прямо в этом окне.
    $f = New-Object System.Windows.Forms.Form
    $f.Text = 'Сборка VST3'; $f.AllowDrop = $true; $f.MinimumSize = '620,520'
    # размер/позиция: по умолчанию минимальные и справа внизу экрана; дальше -- как оставил пользователь
    $wa = [System.Windows.Forms.Screen]::PrimaryScreen.WorkingArea
    $f.StartPosition = 'Manual'; $f.Size = $f.MinimumSize
    $f.Location = New-Object System.Drawing.Point(($wa.Right - $f.Width - 10), ($wa.Bottom - $f.Height - 10))
    if ($Script:Cfg['WinW'] -and $Script:Cfg['WinH']) {
        try {
            $rect = New-Object System.Drawing.Rectangle([int]$Script:Cfg['WinX'], [int]$Script:Cfg['WinY'], [int]$Script:Cfg['WinW'], [int]$Script:Cfg['WinH'])
            $onScreen = $false; foreach ($scr in [System.Windows.Forms.Screen]::AllScreens) { if ($scr.WorkingArea.IntersectsWith($rect)) { $onScreen = $true } }
            if ($onScreen) { $f.Size = $rect.Size; $f.Location = $rect.Location }
        } catch { }
    }
    $f.Add_FormClosed({
        $r = if ($f.WindowState -eq 'Normal') { $f.Bounds } else { $f.RestoreBounds }
        $Script:Cfg['WinX'] = $r.X; $Script:Cfg['WinY'] = $r.Y; $Script:Cfg['WinW'] = $r.Width; $Script:Cfg['WinH'] = $r.Height
        Save-Cfg
    })
    $f.Font = New-Object System.Drawing.Font('Segoe UI', 9)

    # --- верх: подсказка + список проектов
    $hint = New-Object System.Windows.Forms.Label
    $hint.Dock = 'Top'; $hint.Height = 40; $hint.TextAlign = 'MiddleCenter'; $hint.BorderStyle = 'FixedSingle'
    $hint.Text = "ПЕРЕТАЩИ СЮДА папку плагина или корень проекта"
    $hint.BackColor = [System.Drawing.Color]::FromArgb(40, 44, 60); $hint.Tag = 'keep'; $hint.AllowDrop = $true
    $list = New-Object System.Windows.Forms.CheckedListBox
    $list.Dock = 'Top'; $list.Height = 64; $list.CheckOnClick = $true; $list.AllowDrop = $true; $list.Font = New-Object System.Drawing.Font('Consolas', 10)

    # --- настройки
    $opts = New-Object System.Windows.Forms.TableLayoutPanel
    $opts.Dock = 'Top'; $opts.AutoSize = $true; $opts.ColumnCount = 1; $opts.Padding = '4,4,4,0'
    $rowFmt = New-Object System.Windows.Forms.FlowLayoutPanel; $rowFmt.AutoSize = $true; $rowFmt.Dock = 'Top'; $rowFmt.WrapContents = $true
    $lblFmt = New-Object System.Windows.Forms.Label; $lblFmt.Text = 'Собирать:'; $lblFmt.AutoSize = $true; $lblFmt.Padding = '0,5,6,0'
    $rowFmt.Controls.Add($lblFmt)
    $script:fmtBoxes = @{}
    $savedF = @($Script:Cfg['Formats']); if (-not $savedF -or $savedF.Count -eq 0) { $savedF = @('VST3') }
    foreach ($ft in $Script:FormatTable) {
        $cb = New-Object System.Windows.Forms.CheckBox; $cb.Text = $ft.Label; $cb.AutoSize = $true; $cb.Tag = $ft.Key
        $cb.Checked = ($savedF -contains $ft.Key); $script:fmtBoxes[$ft.Key] = $cb; $rowFmt.Controls.Add($cb)
    }
    $lblMac = New-Object System.Windows.Forms.Label; $lblMac.Text = '(AU / macOS — только на Mac с Xcode)'; $lblMac.AutoSize = $true; $lblMac.Tag = 'keep'; $lblMac.Padding = '8,5,0,0'
    $rowFmt.Controls.Add($lblMac)

    $rowDir = New-Object System.Windows.Forms.Panel; $rowDir.Height = 32; $rowDir.Dock = 'Top'
    $lblDir = New-Object System.Windows.Forms.Label; $lblDir.Text = 'VST3 в:'; $lblDir.AutoSize = $true; $lblDir.Location = '4,7'
    $tbDir = New-Object System.Windows.Forms.TextBox; $tbDir.Location = '120,4'; $tbDir.Anchor = 'Top,Left,Right'; $tbDir.Width = $f.ClientSize.Width - 200
    $tbDir.Text = $(if ($Script:Cfg['Vst3Dir']) { [string]$Script:Cfg['Vst3Dir'] } else { $Vst3Dir })
    $bDir = New-Object System.Windows.Forms.Button; $bDir.Text = '…'; $bDir.Width = 34; $bDir.Height = 26; $bDir.Anchor = 'Top,Right'; $bDir.Location = ($f.ClientSize.Width - 52).ToString() + ',3'
    $bDir.Add_Click({ $d = Pick-Folder 'Папка, куда класть готовый .vst3'; if ($d) { $tbDir.Text = $d } })
    $rowDir.Controls.AddRange(@($lblDir, $tbDir, $bDir))

    $rowOther = New-Object System.Windows.Forms.Panel; $rowOther.Height = 32; $rowOther.Dock = 'Top'
    $lblO = New-Object System.Windows.Forms.Label; $lblO.Text = 'Standalone в:'; $lblO.AutoSize = $true; $lblO.Location = '4,7'
    $tbO = New-Object System.Windows.Forms.TextBox; $tbO.Location = '120,4'; $tbO.Anchor = 'Top,Left,Right'; $tbO.Width = $f.ClientSize.Width - 200
    $tbO.Text = $(if ($Script:Cfg['OtherDir']) { [string]$Script:Cfg['OtherDir'] } else { $OtherDir })
    $bO = New-Object System.Windows.Forms.Button; $bO.Text = '…'; $bO.Width = 34; $bO.Height = 26; $bO.Anchor = 'Top,Right'; $bO.Location = ($f.ClientSize.Width - 52).ToString() + ',3'
    $bO.Add_Click({ $d = Pick-Folder 'Папка для Standalone (.exe) / LV2'; if ($d) { $tbO.Text = $d } })
    $rowOther.Controls.AddRange(@($lblO, $tbO, $bO))

    $flags = New-Object System.Windows.Forms.FlowLayoutPanel; $flags.Dock = 'Top'; $flags.AutoSize = $true; $flags.FlowDirection = 'LeftToRight'
    function New-Flag([string]$text, [string]$cfgKey, [bool]$def) {
        $c = New-Object System.Windows.Forms.CheckBox; $c.Text = $text; $c.AutoSize = $true; $c.Margin = '3,0,10,0'
        $c.Checked = Get-CfgBool $cfgKey $def
        return $c
    }
    $cbFlat  = New-Flag 'VST3 одним файлом (без папки-бандла)' 'Flat' $true
    $cbDel   = New-Flag 'Удалять Builds перед/после сборки (без инкремента)' 'DeleteBuilds' $false
    $cbClose = New-Flag 'Закрывать Ableton без вопроса' 'CloseHosts' $true
    $cbMan   = New-Flag 'Пропускать ManifestHelper (быстрее)' 'NoManifest' $true
    $cbErr   = New-Flag 'При ошибке открыть отчёт об ошибках' 'ErrorReport' $true
    $cbMod   = New-Flag 'Сносить JuceLibraryCode\modules (после сборки)' 'CleanModules' $true
    $cbGlob  = New-Flag 'Общие модули JUCE (не копировать 3000 файлов в проект)' 'GlobalModules' $true
    $cbAsync = New-Flag 'Удалять Juce modules в фоне (Builds — синхронно)' 'AsyncDelete' $true
    $cbPdb   = New-Flag 'Без .pdb (меньше мусора, быстрее)' 'NoPdb' $true
    $cbExt   = New-Flag 'Builds вне ZIP: E:\mm\build' 'ExternalBuilds' $true
    $cbScc   = New-Flag 'sccache: общий кэш .obj между ZIP' 'UseSccache' $true
    $flags.Controls.AddRange(@($cbFlat, $cbDel, $cbClose, $cbMan, $cbErr, $cbMod, $cbGlob, $cbAsync, $cbPdb, $cbExt, $cbScc))

    $btns = New-Object System.Windows.Forms.FlowLayoutPanel; $btns.Dock = 'Top'; $btns.AutoSize = $true; $btns.WrapContents = $true; $btns.Padding = '2,4,2,2'
    $bBuild = New-Object System.Windows.Forms.Button; $bBuild.Text = '▶  СОБРАТЬ'; $bBuild.Width = 150; $bBuild.Height = 30
    $bBuild.BackColor = $Script:Dark.Accent; $bBuild.ForeColor = 'White'; $bBuild.FlatStyle = 'Flat'; $bBuild.Font = New-Object System.Drawing.Font('Segoe UI', 10, [System.Drawing.FontStyle]::Bold)
    $bClean = New-Object System.Windows.Forms.Button; $bClean.Text = 'Очистить Builds cache'; $bClean.Width = 120; $bClean.Height = 28
    $bAdd   = New-Object System.Windows.Forms.Button; $bAdd.Text = 'Добавить…'; $bAdd.Width = 100; $bAdd.Height = 28
    $bClr   = New-Object System.Windows.Forms.Button; $bClr.Text = 'Очистить'; $bClr.Width = 90; $bClr.Height = 28
    $bCopy  = New-Object System.Windows.Forms.Button; $bCopy.Text = 'Копировать лог'; $bCopy.Width = 130; $bCopy.Height = 28
    $bErrs  = New-Object System.Windows.Forms.Button; $bErrs.Text = 'Копировать ошибки'; $bErrs.Width = 150; $bErrs.Height = 28; $bErrs.Enabled = $false
    $bMs    = New-Object System.Windows.Forms.Button; $bMs.Text = 'Лог MSBuild'; $bMs.Width = 120; $bMs.Height = 28; $bMs.Enabled = $false
    $bVst   = New-Object System.Windows.Forms.Button; $bVst.Text = 'Папка VST3'; $bVst.Width = 110; $bVst.Height = 28
    $btns.Controls.AddRange(@($bBuild, $bClean, $bAdd, $bClr, $bCopy, $bErrs, $bMs, $bVst))
    $opts.Controls.AddRange(@($btns, $flags, $rowOther, $rowDir, $rowFmt))   # TableLayout: добавляем снизу вверх? нет -- порядок Dock=Top обратный
    foreach ($c in @($rowFmt, $rowDir, $rowOther, $flags, $btns)) { $c.BringToFront() }

    # --- консоль
    $con = New-Object System.Windows.Forms.RichTextBox
    $con.Dock = 'Fill'; $con.ReadOnly = $true; $con.Font = New-Object System.Drawing.Font('Consolas', 9)
    $con.BackColor = [System.Drawing.Color]::FromArgb(14, 14, 16); $con.ForeColor = [System.Drawing.Color]::FromArgb(220, 220, 225); $con.BorderStyle = 'None'
    $con.DetectUrls = $false; $con.WordWrap = $false; $con.ScrollBars = 'Both'; $con.Tag = 'keep'
    $status = New-Object System.Windows.Forms.Label; $status.Dock = 'Bottom'; $status.Height = 26; $status.TextAlign = 'MiddleLeft'; $status.Padding = '8,0,0,0'
    $status.BackColor = [System.Drawing.Color]::FromArgb(36, 36, 42); $status.Tag = 'keep'; $status.Text = 'Готов.'
    $head = New-Object System.Windows.Forms.Label; $head.Dock = 'Top'; $head.Height = 30; $head.TextAlign = 'MiddleLeft'; $head.Padding = '8,0,0,0'
    $head.Font = New-Object System.Drawing.Font('Segoe UI', 10, [System.Drawing.FontStyle]::Bold); $head.Tag = 'keep'; $head.Visible = $false; $head.ForeColor = 'White'
    $split = New-Object System.Windows.Forms.Panel; $split.Dock = 'Fill'
    $split.Controls.AddRange(@($con, $head))
    $f.Controls.AddRange(@($split, $status, $opts, $list, $hint))
    $Script:Con = $con; $Script:StatusLbl = $status; $Script:HeadLbl = $head
    foreach ($ln in $Script:Log) { Gui-Append $ln }

    # --- логика
    $script:mainItems = New-Object System.Collections.Generic.List[object]
    $addDirs = {
        param([string[]]$dirs)
        $projs = Find-Projects $dirs
        foreach ($p in $projs) {
            if ($script:mainItems | Where-Object { $_.Dir -ieq $p.Dir }) { continue }
            $script:mainItems.Add($p); [void]$list.Items.Add(('{0}   [{1}]' -f (Split-Path -Leaf $p.Dir), $p.Dir), $true)
            Gui-Append "Добавлен проект: $($p.Dir)" 'dim'
        }
        if ($projs.Count -eq 0) { Gui-Append 'В перетащенных папках не нашёл ни одного .jucer (искал на 3 уровня вглубь).' 'warn' }
    }
    $dragEnter = { param($s, $e) if ($e.Data.GetDataPresent([System.Windows.Forms.DataFormats]::FileDrop)) { $e.Effect = 'Copy' } }
    $dragDrop  = { param($s, $e) $paths = @($e.Data.GetData([System.Windows.Forms.DataFormats]::FileDrop)); & $addDirs $paths }
    foreach ($c in @($f, $hint, $list, $con)) { $c.AllowDrop = $true; $c.Add_DragEnter($dragEnter); $c.Add_DragDrop($dragDrop) }
    $bAdd.Add_Click({ $d = Pick-Folder 'Папка плагина или корень проекта'; if ($d) { & $addDirs @($d) } })
    $bClr.Add_Click({ $list.Items.Clear(); $script:mainItems.Clear() })
    $bCopy.Add_Click({ try { [System.Windows.Forms.Clipboard]::SetText($con.Text); Gui-Status 'Лог скопирован в буфер обмена' } catch { } })
    $bErrs.Add_Click({ try { [System.Windows.Forms.Clipboard]::SetText((($Script:ErrorReports | ForEach-Object { Get-Content -LiteralPath $_ -Raw }) -join "`r`n`r`n")); Gui-Status 'Ошибки скопированы в буфер обмена' } catch { } })
    $bMs.Add_Click({ foreach ($m in $Script:MsbuildLogs) { if (Test-Path -LiteralPath $m) { Start-Process notepad.exe $m } } })
    $bVst.Add_Click({ $d = $tbDir.Text.Trim(); if (Test-Path -LiteralPath $d) { Start-Process explorer.exe $d } })

    $script:busy = $false
    $script:wk = $null
    $timer = New-Object System.Windows.Forms.Timer; $timer.Interval = 150
    $finishRun = {
        $timer.Stop()
        $code = 1; try { $code = $script:wk.Proc.ExitCode } catch { }
        if ($script:wk.Reader) { try { while (($ln = $script:wk.Reader.ReadLine()) -ne $null) { & $handleLine $ln } } catch { }; $script:wk.Reader.Close() }
        Remove-Item $script:wk.Out -Force -EA SilentlyContinue
        if ($script:wk.Err -and (Test-Path -LiteralPath $script:wk.Err)) {
            try { $errTxt = Get-Content -LiteralPath $script:wk.Err -EA SilentlyContinue | Where-Object { $_.Trim() }; foreach ($el in $errTxt) { Gui-Append $el 'err' } } catch { }
            Remove-Item $script:wk.Err -Force -EA SilentlyContinue
        }
        if (-not $script:wk.Head) { $script:wk.Head = $(if ($code -eq 0) { 'ГОТОВО' } else { "ЗАВЕРШЕНО С ОШИБКОЙ (код $code)" }); $script:wk.Ok = ($code -eq 0) }
        $ok = [bool]$script:wk.Ok
        $head.Text = '  ' + $script:wk.Head; $head.BackColor = $(if ($ok) { $Script:Dark.Accent } else { $Script:Dark.Err }); $head.Visible = $true
        Gui-Status $script:wk.Head
        $bErrs.Enabled = ($Script:ErrorReports.Count -gt 0); $bMs.Enabled = ($Script:MsbuildLogs.Count -gt 0)
        if ($Script:ErrorReports.Count -and $cbErr.Checked) {
            try { [System.Windows.Forms.Clipboard]::SetText((($Script:ErrorReports | ForEach-Object { Get-Content -LiteralPath $_ -Raw }) -join "`r`n`r`n")); Gui-Append 'Ошибки скопированы в буфер обмена' 'ok' } catch { }
            foreach ($er in $Script:ErrorReports) { Start-Process notepad.exe $er }
        }
        Load-Cfg
        foreach ($c in @($bBuild, $bClean, $bAdd, $bClr, $list, $bStop)) { $c.Enabled = $true }; $bStop.Enabled = $false
        $script:busy = $false
    }
    $handleLine = {
        param([string]$ln)
        if ($ln.StartsWith('@L|')) { $i = $ln.IndexOf('|', 3); Gui-Append $ln.Substring($i + 1) $ln.Substring(3, $i - 3) }
        elseif ($ln.StartsWith('@S|')) { $status.Text = $ln.Substring(3) }
        elseif ($ln.StartsWith('@E|')) { $Script:ErrorReports.Add($ln.Substring(3)) }
        elseif ($ln.StartsWith('@M|')) { $Script:MsbuildLogs.Add($ln.Substring(3)) }
        elseif ($ln.StartsWith('@D|')) { $i = $ln.IndexOf('|', 3); $script:wk.Ok = ($ln.Substring(3, $i - 3) -eq 'OK'); $script:wk.Head = $ln.Substring($i + 1) }
        elseif ($ln.Trim()) { Gui-Append $ln 'dim' }
    }
    $timer.Add_Tick({
        if (-not $script:wk) { return }
        try {
            if (-not $script:wk.Reader -and (Test-Path -LiteralPath $script:wk.Out)) {
                $fs = New-Object IO.FileStream($script:wk.Out, [IO.FileMode]::Open, [IO.FileAccess]::Read, [IO.FileShare]::ReadWrite)
                $script:wk.Reader = New-Object IO.StreamReader($fs, [Text.Encoding]::UTF8)
            }
            if ($script:wk.Reader) { $n = 0; while ($n -lt 200 -and ($ln = $script:wk.Reader.ReadLine()) -ne $null) { & $handleLine $ln; $n++ } }
        } catch { }
        if ($script:wk.Proc.HasExited) { & $finishRun }
    })
    $start = {
        param([bool]$clean)
        if ($script:busy) { return }
        $sel = @(); for ($i = 0; $i -lt $list.Items.Count; $i++) { if ($list.GetItemChecked($i)) { $sel += $script:mainItems[$i].Dir } }
        if ($sel.Count -eq 0) { Gui-Append 'Ничего не выбрано — перетащи папку и поставь галочки.' 'warn'; return }
        $fmts = @($script:fmtBoxes.Values | Where-Object Checked | ForEach-Object { $_.Tag })
        if (-not $clean -and $fmts.Count -eq 0) { Gui-Append 'Выбери хотя бы один формат (VST3 / Standalone / ...).' 'warn'; return }
        # сохранить настройки (воркер тоже их запишет, но LastDirs нужны сразу)
        $Script:Cfg['Formats'] = $fmts; $Script:Cfg['OtherDir'] = $tbO.Text.Trim(); $Script:Cfg['NoManifest'] = $cbMan.Checked; $Script:Cfg['ErrorReport'] = $cbErr.Checked
        $Script:Cfg['CloseHosts'] = $cbClose.Checked; $Script:Cfg['DeleteBuilds'] = $cbDel.Checked; $Script:Cfg['CleanModules'] = $cbMod.Checked; $Script:Cfg['GlobalModules'] = $cbGlob.Checked; $Script:Cfg['AsyncDelete'] = $cbAsync.Checked; $Script:Cfg['NoPdb'] = $cbPdb.Checked; $Script:Cfg['ExternalBuilds'] = $cbExt.Checked; $Script:Cfg['UseSccache'] = $cbScc.Checked; $Script:Cfg['BuildCacheRoot'] = (Get-CfgString 'BuildCacheRoot' 'E:\mm\build'); $Script:Cfg['SccacheDir'] = (Get-CfgString 'SccacheDir' 'E:\mm\build\sccache'); $Script:Cfg['SccacheSize'] = (Get-CfgString 'SccacheSize' '30G'); $Script:Cfg['LastDirs'] = @($sel); $Script:Cfg['Flat'] = $cbFlat.Checked; $Script:Cfg['Vst3Dir'] = $tbDir.Text.Trim()
        Save-Cfg
        $optFile = Join-Path $env:TEMP ('nova_opt_' + [guid]::NewGuid().ToString('N') + '.json')
        $optObj = [ordered]@{ Dirs = @($sel); CloseHosts = [bool]$cbClose.Checked; KeepBuild = (-not $cbDel.Checked); CleanOnly = [bool]$clean; Flat = [bool]$cbFlat.Checked
                              Dir = $tbDir.Text.Trim().Trim('"'); OtherDir = $tbO.Text.Trim().Trim('"'); Formats = @($fmts); NoManifest = [bool]$cbMan.Checked; ErrorReport = $false; CleanModules = [bool]$cbMod.Checked; GlobalModules = [bool]$cbGlob.Checked; AsyncDelete = [bool]$cbAsync.Checked; NoPdb = [bool]$cbPdb.Checked; ExternalBuilds = [bool]$cbExt.Checked; BuildCacheRoot = (Get-CfgString 'BuildCacheRoot' 'E:\mm\build'); UseSccache = [bool]$cbScc.Checked; SccacheExe = (Get-CfgString 'SccacheExe' ''); SccacheDir = (Get-CfgString 'SccacheDir' 'E:\mm\build\sccache'); SccacheSize = (Get-CfgString 'SccacheSize' '30G') }
        ($optObj | ConvertTo-Json -Depth 3) | Set-Content -LiteralPath $optFile -Encoding UTF8
        $args = @('-NoProfile', '-ExecutionPolicy', 'Bypass', '-File', "`"$PSCommandPath`"", '-Worker', '-OptFile', "`"$optFile`"")
        $out = Join-Path $env:TEMP ('nova_gui_' + [guid]::NewGuid().ToString('N') + '.txt')
        $script:busy = $true
        foreach ($c in @($bBuild, $bClean, $bAdd, $bClr, $list)) { $c.Enabled = $false }; $bStop.Enabled = $true
        $head.Visible = $false; $con.Clear(); $Script:ErrorReports.Clear(); $Script:MsbuildLogs.Clear(); $bErrs.Enabled = $false; $bMs.Enabled = $false
        $status.Text = 'Запуск...'; Gui-Append 'Запускаю фоновую сборку...' 'dim'
        # Без событий .NET-процесса: их обработчики в PowerShell не выполняются, пока открыт диалог окна.
        # Воркер пишет stdout прямо в файл, окно читает файл таймером.
        $errf = $out + '.err'
        $proc = Start-Process -FilePath 'powershell.exe' -ArgumentList $args -PassThru -WindowStyle Hidden -RedirectStandardOutput $out -RedirectStandardError $errf
        $script:wk = @{ Proc = $proc; Out = $out; Err = $errf; Reader = $null; Head = ''; Ok = $null }
        $timer.Start()
    }
    $bStop = New-Object System.Windows.Forms.Button; $bStop.Text = '■ Стоп'; $bStop.Width = 80; $bStop.Height = 28; $bStop.Enabled = $false
    $bStop.Add_Click({ if ($script:wk -and -not $script:wk.Proc.HasExited) { try { & taskkill.exe /PID $script:wk.Proc.Id /T /F 2>$null | Out-Null } catch { }; Gui-Append 'Сборка остановлена пользователем.' 'warn' } })
    $btns.Controls.Add($bStop); $bStop.BringToFront(); $btns.Controls.SetChildIndex($bStop, 1)
    $bBuild.Add_Click({ & $start $false })
    $bClean.Add_Click({ & $start $true })
    $f.Add_FormClosing({ param($s, $e) if ($script:busy) { $e.Cancel = $true; Gui-Status 'Идёт сборка — дождись окончания или нажми Стоп.' } })
    if (-not ($initial -and $initial.Count) -and $Script:Cfg['LastDirs']) { $initial = @($Script:Cfg['LastDirs'] | Where-Object { $_ -and (Test-Path -LiteralPath $_) }) }
    if ($initial -and $initial.Count) { $f.Add_Shown({ & $addDirs $initial }) }
    Apply-Dark $f; $lblMac.ForeColor = 'Gray'; $con.BackColor = [System.Drawing.Color]::FromArgb(14, 14, 16)
    [void]$f.ShowDialog()
}

function Set-GlobalModules([string]$jucer, [string]$projucer) {
    # Переключает все модули в .jucer на useGlobalPath=1 / useLocalCopy=0 и прописывает
    # глобальный путь к модулям в настройках Projucer, если он пуст. Идемпотентно.
    try {
        $raw = Get-Content -LiteralPath $jucer -Raw
        $new = $raw
        $new = [regex]::Replace($new, '(<MODULE\b[^>]*?)\suseLocalCopy="1"', '$1 useLocalCopy="0"')
        $new = [regex]::Replace($new, '(<MODULE\b[^>]*?)\suseGlobalPath="0"', '$1 useGlobalPath="1"')
        # модули без атрибута useGlobalPath -- добавить
        $new = [regex]::Replace($new, '<MODULE\b(?![^>]*useGlobalPath=)([^>]*?)(/?)>', '<MODULE$1 useGlobalPath="1"$2>')
        # пути к модулям внутри экспортёров (MODULEPATH) при useGlobalPath игнорируются -- не трогаем
        if ($new -ne $raw) {
            $bak = $jucer + '.bak'; if (-not (Test-Path -LiteralPath $bak)) { Copy-Item -LiteralPath $jucer -Destination $bak }
            Set-Content -LiteralPath $jucer -Value $new -Encoding UTF8
            $n = ([regex]::Matches($new, 'useGlobalPath="1"')).Count
            Log "$(Split-Path -Leaf $jucer): модули переключены на общие (глобальный путь), модулей: $n; копия -> $(Split-Path -Leaf $bak)" 'ok'
        } else { Log "$(Split-Path -Leaf $jucer): модули уже общие" 'dim' }
    } catch { Log "Не смог поправить .jucer: $($_.Exception.Message)" 'warn'; return }
    # глобальный путь в настройках Projucer
    try {
        $settings = Join-Path $env:APPDATA 'Projucer\Projucer.settings'
        $modulesDir = ''
        $jroot = Split-Path -Parent $projucer
        foreach ($cand in @((Join-Path $jroot 'modules'), (Join-Path (Split-Path -Parent $jroot) 'modules'), 'C:\JUCE\modules')) { if (Test-Path -LiteralPath (Join-Path $cand 'juce_core')) { $modulesDir = $cand; break } }
        if (-not $modulesDir) { Log 'Не нашёл папку modules рядом с Projucer — проверь File -> Global Paths в Projucer.' 'warn'; return }
        if (Test-Path -LiteralPath $settings) {
            [xml]$x = Get-Content -LiteralPath $settings -Raw
            $node = $x.SelectSingleNode("//VALUE[@name='defaultJuceModulePath']")
            if (-not $node) {
                $node = $x.CreateElement('VALUE'); $node.SetAttribute('name', 'defaultJuceModulePath'); $node.SetAttribute('val', $modulesDir)
                [void]$x.DocumentElement.AppendChild($node); $x.Save($settings)
                Log "Projucer: глобальный путь к модулям задан: $modulesDir" 'ok'
            } elseif (-not $node.val -or -not (Test-Path -LiteralPath (Join-Path $node.val 'juce_core'))) {
                $node.SetAttribute('val', $modulesDir); $x.Save($settings)
                Log "Projucer: глобальный путь к модулям исправлен: $modulesDir" 'ok'
            } else { Log "Projucer: глобальный путь к модулям: $($node.val)" 'dim' }
        } else { Log "Projucer.settings не найден ($settings) — открой Projucer один раз, File -> Global Paths -> JUCE Modules = $modulesDir" 'warn' }
    } catch { Log "Не смог проверить Projucer.settings: $($_.Exception.Message)" 'warn' }
}
function Build-One($proj, [string]$msbuild, [string]$projucer, [string]$cmake) {
    $name = Split-Path -Leaf $proj.Dir
    $logicalBuildRoot = Join-Path $proj.Dir 'Builds'
    $physicalBuildRoot = $logicalBuildRoot
    if ($ExternalBuilds) {
        $physicalBuildRoot = Get-ExternalBuildRoot $proj
        try { [void](Ensure-ExternalBuildLink $proj.Dir $physicalBuildRoot) }
        catch { $r0 = [ordered]@{ Name = $name; Built = $false; Status = 'не собрано'; Target = ''; Detail = $_.Exception.Message; Time = '' }; Log $r0.Detail 'err'; $Script:Results.Add([pscustomobject]$r0); return }
    } elseif (-not (Test-Path -LiteralPath $physicalBuildRoot)) {
        New-Item -ItemType Directory -Path $physicalBuildRoot -Force | Out-Null
    }
    # Always use the logical junction path while reading generated .sln/.vcxproj:
    # Projucer writes relative paths from Builds back to Source.
    $buildRoot = $logicalBuildRoot
    $buildDir  = Join-Path $buildRoot 'VisualStudio2022'
    $r = [ordered]@{ Name = $name; Built = $false; Status = 'не собрано'; Target = ''; Detail = ''; Time = '' }
    Log "==== $name ====" 'head'
    $sw = [Diagnostics.Stopwatch]::StartNew()

    Remove-Tombstones $proj.Dir; Remove-Tombstones $physicalBuildRoot; Remove-Tombstones (Join-Path $proj.Dir 'JuceLibraryCode')
    # IMPORTANT: KeepBuild now applies before as well as after compilation. The
    # previous builder always deleted Builds here, making its UI checkbox inert.
    if ($CleanOnly) {
        try {
            $s = Clear-PhysicalBuildRoot $physicalBuildRoot
            Remove-Item -LiteralPath (Join-Path $physicalBuildRoot '.nova_source_fingerprint.sha256') -Force -EA SilentlyContinue
            $r.Status = 'очищено'; $r.Detail = "Build cache: $physicalBuildRoot"; Log "Builds очищен: $physicalBuildRoot; освобождено $(Fmt $s)" 'ok'; $Script:Results.Add([pscustomobject]$r); return
        } catch { $r.Detail = $_.Exception.Message; Log $r.Detail 'err'; $Script:Results.Add([pscustomobject]$r); return }
    }
    if (-not $KeepBuild) {
        try { $s = Clear-PhysicalBuildRoot $physicalBuildRoot; Log "Builds очищен перед сборкой (-KeepBuild не выбран): освобождено $(Fmt $s)" 'ok' }
        catch { $r.Detail = $_.Exception.Message; Log $r.Detail 'err'; $Script:Results.Add([pscustomobject]$r); return }
    } else {
        Log "Builds сохранён для инкрементальной сборки: $physicalBuildRoot" 'ok'
    }
    $modDir = Join-Path $proj.Dir 'JuceLibraryCode\modules'
    if ($CleanModules -and (Test-Path -LiteralPath $modDir)) {
        try { $s = Remove-Forever $modDir; Log "JuceLibraryCode\modules удалена навсегда: освобождено $(Fmt $s)" 'ok' } catch { Log $_.Exception.Message 'warn' }
    }

    # A content fingerprint gives correctness across fresh ZIP extractions. If
    # inputs changed we ask MSBuild for Rebuild; sccache then restores every
    # unchanged .obj and compiles only the genuinely changed units.
    if ($proj.Jucer -and -not $UseCMake -and $GlobalModules) { Set-GlobalModules $proj.Jucer $projucer }
    # Paths alone are not enough: an in-place VS update can replace cl.exe or
    # MSBuild without changing their path.  Their content hashes are part of
    # the build signature, so retained object files are rebuilt in that case.
    $realCompiler = if ($Script:Sccache) { $Script:Sccache.RealCl } else { Find-MsvcCl $msbuild }
    $sig = "formats=$($Formats -join ',');manifest=$NoManifest;global=$GlobalModules;nopdb=$NoPdb;cmake=$UseCMake;msbuild=$(Get-ToolFingerprint $msbuild);compiler=$(Get-ToolFingerprint $realCompiler)"
    try { $inputState = Get-BuildInputState $proj.Dir $physicalBuildRoot $sig }
    catch { $r.Detail = "Не смог посчитать source fingerprint: $($_.Exception.Message)"; Log $r.Detail 'err'; $Script:Results.Add([pscustomobject]$r); return }
    $forceRebuild = (-not $KeepBuild) -or $inputState.Changed
    if ($forceRebuild) {
        Log "Source fingerprint изменился$(if (-not $inputState.Previous) { ' / первый запуск' }): выполняю безопасный Rebuild; неизменившиеся .obj возьмёт sccache." 'warn'
    } else { Log 'Source fingerprint не изменился: оставляю обычную инкрементальную сборку.' 'dim' }

    $bundle = ''; $artefacts = @()
    if ($proj.Jucer -and -not $UseCMake) {
        # 2. Projucer --resave. Projucer -- GUI-приложение: обычный вызов не ждёт его завершения,
        #    поэтому запускаем через Start-Process -Wait и вывод пишем в файл.
        Log "Projucer --resave $(Split-Path -Leaf $proj.Jucer)"
        $po = Join-Path $env:TEMP 'nova_projucer_out.txt'; $pe = Join-Path $env:TEMP 'nova_projucer_err.txt'
        $prc = Start-Process -FilePath $projucer -ArgumentList @('--resave', ('"' + $proj.Jucer + '"')) -Wait -PassThru -NoNewWindow -RedirectStandardOutput $po -RedirectStandardError $pe
        $prOut = @(); foreach ($f in @($po, $pe)) { if (Test-Path $f) { $prOut += Get-Content $f -EA SilentlyContinue; Remove-Item $f -Force -EA SilentlyContinue } }
        $prOut | ForEach-Object { if ($_) { Log "  $_" } }
        # куда Projucer положил проект: читаем targetFolder экспортера VS2022 из .jucer
        $buildDir = ''
        try {
            [xml]$jx = Get-Content -LiteralPath $proj.Jucer -Raw
            $exp = $jx.SelectSingleNode('//EXPORTFORMATS/VS2022'); if (-not $exp) { $exp = $jx.SelectSingleNode('//EXPORTFORMATS/*[starts-with(name(),"VS")]') }
            if ($exp -and $exp.targetFolder) { $tf = $exp.targetFolder; if (-not [IO.Path]::IsPathRooted($tf)) { $tf = Join-Path $proj.Dir $tf }; if (Test-Path -LiteralPath $tf) { $buildDir = $tf } }
        } catch { }
        if (-not $buildDir) {
            $vcxAny = Get-ChildItem -LiteralPath $proj.Dir -Recurse -File -Filter '*.sln' -Depth 3 -EA SilentlyContinue | Select-Object -First 1
            if ($vcxAny) { $buildDir = $vcxAny.DirectoryName }
        }
        if ($prc.ExitCode -ne 0 -or -not $buildDir) {
            $r.Detail = "Projucer не смог пересохранить проект (код $($prc.ExitCode)); .sln так и не появился. " +
                        "Открой .jucer в Projucer, проверь Exporters -> Visual Studio 2022 и что в Modules нет красных путей. Вывод Projucer выше в журнале."
            Log $r.Detail 'err'; $Script:Results.Add([pscustomobject]$r); return
        }
        # Do not replace $buildRoot with Resolve-Path output here: through a
        # junction the logical Builds path is needed for Projucer's relative
        # Source/JuceLibraryCode paths. $physicalBuildRoot is used only for cleanup.
        Log "Проект: $buildDir"
        # 2b. Пропуск ManifestHelper: убираем из *_VST3.vcxproj ссылку на *_VST3ManifestHelper и его вызов
        #     в PostBuild. Проект генерируется Projucer'ом заново при каждой сборке, так что правка временная.
        if ($NoManifest -and ($Formats -contains 'VST3')) {
            $vcx3 = Get-ChildItem -LiteralPath $buildDir -File -Filter '*_VST3.vcxproj' | Select-Object -First 1
            if ($vcx3) {
                try {
                    $xml = Get-Content -LiteralPath $vcx3.FullName -Raw
                    $orig = $xml
                    # ссылка на проект-хелпер
                    $xml = [regex]::Replace($xml, '(?s)<ProjectReference Include="[^"]*ManifestHelper[^"]*">.*?</ProjectReference>', '')
                    # вызов juce_vst3_helper в PostBuildEvent (строка команды, содержащая helper.exe)
                    $xml = [regex]::Replace($xml, '(?m)^[^\r\n]*(?:ManifestHelper|juce_vst3_helper)[^\r\n]*\.exe[^\r\n]*\r?\n?', '')
                    if ($xml -ne $orig) { Set-Content -LiteralPath $vcx3.FullName -Value $xml -Encoding UTF8; Log 'ManifestHelper отключён для этой сборки (moduleinfo.json не создаётся)' 'ok' }
                    else { Log 'ManifestHelper в проекте не найден — нечего отключать' }
                } catch { Log "Не смог отключить ManifestHelper: $($_.Exception.Message)" 'warn' }
            }
        }
        # 3. MSBuild: выбранные форматы (цели берём из .sln, MSBuild сам подтянет SharedCode)
        $sln = Get-ChildItem -LiteralPath $buildDir -File -Filter '*.sln' | Select-Object -First 1
        if (-not $sln) { $r.Detail = "В $buildDir нет .sln — Projucer не сгенерировал решение."; Log $r.Detail 'err'; $Script:Results.Add([pscustomobject]$r); return }
        $slnProjects = @()
        foreach ($line in (Get-Content -LiteralPath $sln.FullName)) {
            if ($line -match '^Project\("\{[^}]+\}"\)\s*=\s*"([^"]+)"\s*,\s*"([^"]+)"') { $slnProjects += [pscustomobject]@{ Name = $Matches[1]; File = (Split-Path -Leaf $Matches[2]) } }
        }
        $wanted = @(); $missing = @()
        foreach ($ft in $Script:FormatTable) {
            if ($Formats -notcontains $ft.Key) { continue }
            $pj = $slnProjects | Where-Object { $_.File -like ('*' + $ft.Suffix) } | Select-Object -First 1
            if ($pj) { $wanted += [pscustomobject]@{ Fmt = $ft; Target = ($pj.Name -replace "[%\$@;\.\(\)']", '_') } } else { $missing += $ft.Key }
        }
        if ($missing.Count) { Log "В проекте $name нет форматов: $($missing -join ', ') (включи в Projucer -> Plugin Formats) — пропускаю их." 'warn' }
        if ($wanted.Count -eq 0) { $r.Detail = 'Ни один выбранный формат не включён в этом проекте.'; Log $r.Detail 'err'; $Script:Results.Add([pscustomobject]$r); return }
        $cpu = [Environment]::ProcessorCount
        $msLog = Join-Path (Join-Path $Script:Root 'logs') ("msbuild_" + ($name -replace '[^\w\-]', '_') + '_' + (Get-Date).ToString('yyyy-MM-dd_HH-mm-ss') + '.log')
        $Script:MsbuildLogs.Add($msLog)
        if ($Worker) { [Console]::Out.WriteLine("@M|$msLog"); [Console]::Out.Flush() }
        $common = @('/p:Configuration=Release', '/p:Platform=x64', "/m:$cpu", "/p:CL_MPCount=$cpu", '/nologo', '/v:m', '/nr:false', '/clp:NoSummary;ForceNoAlign', '/fl', "/flp:logfile=$msLog;verbosity=normal;encoding=utf-8")
        if ($Script:Sccache -and $Script:Sccache.Enabled) {
            # Use a forward trailing separator: it is valid for CreateProcess and
            # avoids a final backslash escaping the closing quote when a custom
            # external cache root contains spaces.
            $toolPath = $Script:Sccache.Wrapper.Directory.TrimEnd('\/') + '/'
            # CLToolExe is a .cmd wrapper that invokes `sccache <real cl.exe>`.
            # This avoids replacing Visual Studio's cl.exe or depending on PATH.
            $common += @("/p:CLToolPath=$toolPath", '/p:CLToolExe=nova_sccache_cl.cmd', '/p:UseMultiToolTask=true', '/p:TrackFileAccess=false')
            Log 'MSBuild -> sccache wrapper (MSVC response files supported by current sccache)' 'ok'
        }
        if ($NoPdb) { $common += @('/p:DebugInformationFormat=None', '/p:GenerateDebugInformation=false', '/p:LinkIncremental=false'); Log 'Без .pdb/.ilk (меньше мусора в Builds)' 'dim' }
        $tgts = if ($forceRebuild) { ($wanted | ForEach-Object { $_.Target + ':Rebuild' }) -join ';' } else { ($wanted | ForEach-Object { $_.Target }) -join ';' }
        Log "MSBuild $($sln.Name)  /t:`"$tgts`"  Release | x64  (потоков: $cpu)..."
        $msbArgs = @($sln.FullName, "/t:$tgts") + $common
        Log ("  > MSBuild " + ($msbArgs -join " "))
        $b = Run-Live $msbuild $msbArgs 'сборка'
        if ($b.Code -ne 0 -and (($b.Out -join ' ') -match 'MSB4057')) {
            Log 'Цель в .sln не подошла — собираю решение целиком (как Build Solution в VS).' 'warn'
            $msbArgs = if ($forceRebuild) { @($sln.FullName, '/t:Rebuild') + $common } else { @($sln.FullName) + $common }
            $b = Run-Live $msbuild $msbArgs 'сборка'
        }
        $errs = @($b.Out | Where-Object { $_ -match 'error [A-Z]+\d+|error MSB|fatal error' })
        # Проект от Projucer «под VS2022» хочет toolset v143. В VS 2026 его может не быть -> MSB8020.
        # Тогда пересобираем с тем toolset, который реально установлен (например v145).
        if ($b.Code -ne 0 -and ($errs -join ' ') -match 'MSB8020|MSB8036') {
            $ts = Get-Toolsets $msbuild
            if ($ts.Count) {
                Log "Toolset из проекта не установлен — пробую /p:PlatformToolset=$($ts[0]) (есть: $($ts -join ', '))" 'warn'
                $b = Run-Live $msbuild ($msbArgs + @("/p:PlatformToolset=$($ts[0])")) 'сборка'
                $errs = @($b.Out | Where-Object { $_ -match 'error [A-Z]+\d+|error MSB|fatal error' })
            }
        }
        if ($b.Code -ne 0 -and $errs.Count -eq 0) { @($b.Out | Select-Object -Last 15) | ForEach-Object { Log "  $_" 'err' } }
        if ($b.Code -ne 0) {
            $r.Detail = "Ошибка компиляции (код $($b.Code)), ошибок: $($errs.Count). Builds оставлен для разбора."
            Log $r.Detail 'err'
            Write-ErrorReport -Name $name -Cmd ($msbArgs -join ' ') -Out $b.Out -Errs $errs
            $Script:Results.Add([pscustomobject]$r); return
        }
        # итог MSBuild -- как в окне Output в VS
        $done = @($b.Out | Where-Object { $_ -match '\.vcxproj\s*->\s*|^\s*\S+\s*->\s*\S+\.(vst3|exe|dll|lib)\s*$' })
        foreach ($d in $done) { Log ("  " + $d.Trim()) 'ok' }
        Log "MSBuild: сборка успешна, целей: $($wanted.Count) ($(($wanted | ForEach-Object { $_.Fmt.Key }) -join ', ')); полный лог MSBuild: $msLog" 'ok'
        $artefacts = @()
        foreach ($w in $wanted) {
            $out = Join-Path $buildDir ('x64\Release\' + $w.Fmt.OutDir)
            $cand = Get-ChildItem -LiteralPath $out -Filter $w.Fmt.Pattern -Force -EA SilentlyContinue | Select-Object -First 1
            if (-not $cand) { $cand = Get-ChildItem -LiteralPath $buildDir -Recurse -Filter $w.Fmt.Pattern -Force -EA SilentlyContinue | Where-Object { $_.FullName -match '\\Release\\' -and $_.FullName -notmatch '\\SharedCode\\|ManifestHelper' } | Select-Object -First 1 }
            if ($cand) { $artefacts += [pscustomobject]@{ Fmt = $w.Fmt; Path = $cand.FullName } } else { Log "$($w.Fmt.Label): сборка прошла, но результат не найден в $out" 'err' }
        }
        $bundle = ($artefacts | Where-Object { $_.Fmt.Key -eq 'VST3' } | Select-Object -First 1).Path
    } else {
        # альтернатива: CMake (для проектов без .jucer)
        if (-not $cmake) { $r.Detail = 'Нет .jucer и не найден cmake.exe.'; Log $r.Detail 'err'; $Script:Results.Add([pscustomobject]$r); return }
        $bd = Join-Path $buildRoot 'cmake'
        $cfgArgs = @('-S', $proj.Dir, '-B', $bd, '-G', 'Visual Studio 17 2022', '-A', 'x64')
        Log "cmake $($cfgArgs -join ' ')"
        $c = Run $cmake $cfgArgs; if ($c.Code -ne 0) { ($c.Out | Select-Object -Last 30) | ForEach-Object { Log "  $_" 'err' }; $r.Detail = 'CMake configure не прошёл.'; $Script:Results.Add([pscustomobject]$r); return }
        Log 'cmake --build (Release, только *_VST3)...'
        $tg = Get-ChildItem -LiteralPath $bd -Filter '*_VST3.vcxproj' -Recurse | Select-Object -First 1
        $bArgs = @('--build', $bd, '--config', 'Release', '--parallel'); if ($forceRebuild) { $bArgs += '--clean-first' }; if ($tg) { $bArgs += @('--target', $tg.BaseName) }
        $b = Run $cmake $bArgs
        $errs = @($b.Out | Where-Object { $_ -match 'error [A-Z]+\d+|error MSB|fatal error' }); $errs | ForEach-Object { Log "  $_" 'err' }
        if ($b.Code -ne 0) { $r.Detail = "Ошибка компиляции (код $($b.Code))."; Log $r.Detail 'err'; $Script:Results.Add([pscustomobject]$r); return }
        $cand = Get-ChildItem -LiteralPath $bd -Recurse -Directory -Filter '*.vst3' -EA SilentlyContinue | Where-Object FullName -like '*Release*' | Select-Object -First 1
        if ($cand) { $bundle = $cand.FullName }
    }
    $sw.Stop(); $r.Time = "{0}:{1:00}" -f [int]$sw.Elapsed.TotalMinutes, $sw.Elapsed.Seconds
    if (-not $artefacts) { $artefacts = @(); if ($bundle) { $artefacts += [pscustomobject]@{ Fmt = $Script:FormatTable[0]; Path = $bundle } } }
    if ($artefacts.Count -eq 0) { $r.Detail = 'Сборка прошла, но ни одного результата не найдено в папке вывода.'; Log $r.Detail 'err'; $Script:Results.Add([pscustomobject]$r); return }
    $r.Built = $true
    try { Save-BuildInputState $inputState; Log "Source fingerprint записан: $($inputState.File)" 'dim' } catch { Log "Не смог записать source fingerprint: $($_.Exception.Message)" 'warn' }
    foreach ($a in $artefacts) { Log "Собрано за $($r.Time): [$($a.Fmt.Key)] $($a.Path) ($(Fmt (TreeSize $a.Path)))" 'ok' }

    # 4. установка каждого формата
    foreach ($a in $artefacts) {
        $rr = [ordered]@{ Name = "$name [$($a.Fmt.Key)]"; Built = $true; Status = ''; Target = ''; Detail = ''; Time = $r.Time }
        if ($NoInstall) { $rr.Status = 'собрано (без установки)'; $rr.Target = $a.Path; $Script:Results.Add([pscustomobject]$rr); continue }
        $isVst3 = ($a.Fmt.Key -eq 'VST3')
        $destDir = if ($isVst3) { $Vst3Dir } elseif ($OtherDir) { $OtherDir } else { Join-Path $Vst3Dir '!build' }
        $srcBin = if ($isVst3) { Vst3-Binary $a.Path } else { $a.Path }
        $srcIsDir = (Get-Item -LiteralPath $a.Path -Force).PSIsContainer
        $target = Join-Path $destDir (Split-Path -Leaf $a.Path)
        $rr.Target = $target
        $checkFile = if (Test-Path -LiteralPath $target) { if ((Get-Item -LiteralPath $target -Force).PSIsContainer) { Vst3-Binary $target } else { $target } } else { '' }
        if ($checkFile -and -not (Free-File $checkFile)) {
            $rr.Status = 'НЕ ЗАМЕНЕНО — файл занят'; $rr.Detail = 'Закрой Ableton (или другой хост) и запусти снова.'
            Log "$($rr.Name): $($rr.Status)" 'err'; $Script:Results.Add([pscustomobject]$rr); continue
        }
        try {
            if (-not (Test-Path -LiteralPath $destDir)) { New-Item -ItemType Directory -Path $destDir -Force | Out-Null }
            if (Test-Path -LiteralPath $target) {
                if ((Get-Item -LiteralPath $target -Force).PSIsContainer) { [void](Remove-Forever $target) } else { Remove-Item -LiteralPath $target -Force }
            }
            if ($isVst3 -and $Flat) {
                if (-not $srcBin) { throw 'внутри бандла не найден бинарник .vst3' }
                Copy-Item -LiteralPath $srcBin -Destination $target -Force; $nb = $target
            } elseif ($srcIsDir) {
                Copy-Item -LiteralPath $a.Path -Destination $target -Recurse -Force
                $nb = if ($isVst3) { Vst3-Binary $target } else { $target }
            } else {
                Copy-Item -LiteralPath $a.Path -Destination $target -Force; $nb = $target
            }
            if ($srcBin -and -not (Get-Item -LiteralPath $nb -Force).PSIsContainer) {
                if ((Get-FileHash -LiteralPath $nb).Hash -ne (Get-FileHash -LiteralPath $srcBin).Hash) { throw 'скопированный файл не совпадает с собранным' }
                $rr.Detail = "$(if ($isVst3 -and $Flat) { 'один файл, ' })$(Fmt (Get-Item -LiteralPath $nb).Length), $((Get-Item -LiteralPath $nb).LastWriteTime.ToString('HH:mm:ss'))"
            }
            $rr.Status = 'ЗАМЕНЕНО'
            Log "ЗАМЕНЕНО: $target" 'ok'
        } catch {
            $rr.Status = 'НЕ ЗАМЕНЕНО — ошибка копирования'; $rr.Detail = $_.Exception.Message
            Log "$($rr.Name): $($rr.Status): $($rr.Detail)" 'err'
        }
        $Script:Results.Add([pscustomobject]$rr)
    }
    $r = $null

    # 5. optionally clear the *physical* build cache. Never remove the
    # project-side junction: it is harmless and points to an empty cache root.
    if (-not $KeepBuild) {
        try { $s = Clear-PhysicalBuildRoot $physicalBuildRoot; Log "Физическая папка Builds очищена$(if ($AsyncDelete) { ' (синхронно для сохранности junction)' }): освобождено $(Fmt $s)" 'ok' }
        catch { Log "Builds очистить не удалось (занята): $physicalBuildRoot" 'warn' }
    } else { Log "Builds оставлена для следующей сборки: $physicalBuildRoot" 'ok' }
    if ($CleanModules -and (Test-Path -LiteralPath $modDir)) {
        try { $s = $(if ($AsyncDelete) { Remove-Forever-Async $modDir } else { Remove-Forever $modDir }); Log "JuceLibraryCode\modules удалена навсегда: освобождено $(Fmt $s)" 'ok' } catch { Log "modules удалить не удалось: $($_.Exception.Message)" 'warn' }
    }
    if ($r) { $Script:Results.Add([pscustomobject]$r) }
}

# =============================================================== главное =====
function Reset-Run {
    $Script:Freed = [int64]0
    $Script:Results.Clear(); $Script:ErrorReports.Clear(); $Script:MsbuildLogs.Clear()
    $logDir = Join-Path $Script:Root 'logs'; New-Item -ItemType Directory -Path $logDir -Force | Out-Null
    $Script:LogFile = Join-Path $logDir ('build_' + (Get-Date).ToString('yyyy-MM-dd_HH-mm-ss') + '.log')
    Get-ChildItem -LiteralPath $logDir -File | Sort-Object LastWriteTime -Descending | Select-Object -Skip 40 | Remove-Item -Force -EA SilentlyContinue
}

function Invoke-Build($opt) {
    # $opt: Dirs, CloseHosts, KeepBuild, CleanOnly, Flat, Dir, OtherDir, Formats, NoManifest, ErrorReport
    Reset-Run
    Log 'Сборка VST3: Release x64' 'head'
    $dirs = @($opt.Dirs)
    if ($opt.CloseHosts) { $script:CloseHosts = $true } else { $script:CloseHosts = $false }
    $script:KeepBuild = [bool]$opt.KeepBuild; $script:CleanOnly = [bool]$opt.CleanOnly; $script:Flat = [bool]$opt.Flat
    if ($opt.Dir) { $script:Vst3Dir = $opt.Dir }
    if ($opt.Formats -and $opt.Formats.Count) { $script:Formats = @($opt.Formats) }
    $script:OtherDir = [string]$opt.OtherDir
    $script:NoManifest = [bool]$opt.NoManifest; $script:NoErrorReport = -not [bool]$opt.ErrorReport; $script:CleanModules = [bool]$opt.CleanModules; $script:GlobalModules = [bool]$opt.GlobalModules; $script:AsyncDelete = [bool]$opt.AsyncDelete; $script:NoPdb = [bool]$opt.NoPdb
    $script:ExternalBuilds = [bool]$opt.ExternalBuilds; $script:BuildCacheRoot = [string]$opt.BuildCacheRoot; if (-not $script:BuildCacheRoot) { $script:BuildCacheRoot = 'E:\mm\build' }; $script:UseSccache = [bool]$opt.UseSccache; $script:SccacheExe = [string]$opt.SccacheExe; $script:SccacheDir = [string]$opt.SccacheDir; $script:SccacheSize = [string]$opt.SccacheSize; if (-not $script:SccacheSize) { $script:SccacheSize = '30G' }
    # Cast switches to plain JSON booleans.  Serialising SwitchParameter gives
    # { IsPresent: ... }, which makes a previously unchecked GUI flag look true.
    $Script:Cfg['CleanModules'] = [bool]$CleanModules; $Script:Cfg['GlobalModules'] = [bool]$GlobalModules; $Script:Cfg['AsyncDelete'] = [bool]$AsyncDelete; $Script:Cfg['NoPdb'] = [bool]$NoPdb; $Script:Cfg['ExternalBuilds'] = [bool]$ExternalBuilds; $Script:Cfg['BuildCacheRoot'] = $BuildCacheRoot; $Script:Cfg['UseSccache'] = [bool]$UseSccache; $Script:Cfg['SccacheExe'] = $SccacheExe; $Script:Cfg['SccacheDir'] = $SccacheDir; $Script:Cfg['SccacheSize'] = $SccacheSize
    $Script:Cfg['Formats'] = @($Formats); $Script:Cfg['OtherDir'] = $OtherDir; $Script:Cfg['NoManifest'] = [bool]$NoManifest; $Script:Cfg['ErrorReport'] = [bool]$opt.ErrorReport
    $Script:Cfg['CloseHosts'] = [bool]$CloseHosts; $Script:Cfg['DeleteBuilds'] = -not [bool]$KeepBuild; $Script:Cfg['LastDirs'] = @($dirs); $Script:Cfg['Flat'] = [bool]$Flat; $Script:Cfg['Vst3Dir'] = $Vst3Dir
    Save-Cfg

    $projects = @(Find-Projects $dirs)
    if ($projects.Count -eq 0) { throw 'Не найдено ни одного .jucer / CMakeLists.txt с плагином.' }
    $dir = $dirs[0]
    Log ("Проекты: " + (($projects | ForEach-Object { Split-Path -Leaf $_.Dir }) -join ', '))
    $projects | ForEach-Object { Log "  $($_.Dir)" }

    # права на папку назначения
    if (-not $CleanOnly -and -not $NoInstall -and -not (Is-Admin)) {
        $probe = Join-Path $Vst3Dir ('.w' + [guid]::NewGuid().ToString('N')); $canWrite = $false
        try { if (-not (Test-Path -LiteralPath $Vst3Dir)) { New-Item -ItemType Directory -Path $Vst3Dir -Force -EA Stop | Out-Null }; [IO.File]::WriteAllText($probe, 'x'); Remove-Item -LiteralPath $probe -Force; $canWrite = $true } catch { }
        if (-not $canWrite) { throw "Нет прав на запись в $Vst3Dir. Запусти build_vst3.bat от имени администратора (ПКМ -> Запуск от имени администратора) или выбери другую папку." }
    }

    $msbuild = ''; $projucer = ''; $cmake = ''
    if (-not $CleanOnly) {
        $needJucer = @($projects | Where-Object { $_.Jucer -and -not $UseCMake }).Count -gt 0
        $needCMake = @($projects | Where-Object { -not $_.Jucer -or $UseCMake }).Count -gt 0
        $msbuild = Find-MSBuild
        if (-not $msbuild) { throw 'Не найден MSBuild.exe (Visual Studio с компонентом «Разработка классических приложений на C++»).' }
        Log "MSBuild:  $msbuild" 'ok'; $Script:Cfg['MSBuild'] = $msbuild
        if ($needJucer) {
            $projucer = Find-Projucer $dir
            if (-not $projucer) { throw 'Не найден Projucer.exe. Запусти ещё раз и укажи его в окне выбора файла (запомню навсегда).' }
            Log "Projucer: $projucer" 'ok'; $Script:Cfg['Projucer'] = $projucer
        }
        if ($needCMake) { $cmake = Find-CMake; if ($cmake) { Log "CMake:    $cmake" 'ok' } }
        Log "Папка VST3: $Vst3Dir   (админ: $(if (Is-Admin) { 'да' } else { 'нет' }); режим: $(if ($Flat) { 'один файл' } else { 'бандл-папка' }))"
        Log "Форматы: $($Formats -join ', ')$(if ($OtherDir) { ";  Standalone/LV2 -> $OtherDir" })"
        if ($ExternalBuilds) { Log "Внешний Builds root: $BuildCacheRoot" 'ok' }
        $Script:Sccache = Initialize-Sccache $projects $msbuild
    } else { $Script:Sccache = $null }
    Save-Cfg

    foreach ($p in $projects) { Build-One $p $msbuild $projucer $cmake }
    Log-SccacheStats $Script:Sccache

    $allOk = ($Script:Results.Count -gt 0) -and (@($Script:Results | Where-Object { $_.Status -notin @('ЗАМЕНЕНО', 'очищено', 'собрано (без установки)') }).Count -eq 0)
    $head = if ($CleanOnly) { "Готово: Builds cache очищен, освобождено $(Fmt $Script:Freed)" }
            elseif ($allOk) { "ГОТОВО: плагины заменены, освобождено $(Fmt $Script:Freed)" }
            else { 'ЕСТЬ ПРОБЛЕМЫ — смотри лог' }
    Log '' ; Log '================ ИТОГ ================' 'head'
    foreach ($r in $Script:Results) {
        $lvl = if ($r.Status -in @('ЗАМЕНЕНО', 'очищено', 'собрано (без установки)')) { 'ok' } else { 'err' }
        Log (('{0,-40} {1}' -f $r.Name, $r.Status) + $(if ($r.Time) { "   (сборка $($r.Time))" } else { '' })) $lvl
        if ($r.Target) { Log "    -> $($r.Target)" }
        if ($r.Detail) { Log "       $($r.Detail)" 'dim' }
    }
    Log "Место освобождено (мимо корзины): $(Fmt $Script:Freed)"
    if ($Script:ErrorReports.Count) { Log 'ОТЧЁТЫ ОБ ОШИБКАХ (кнопка «Скопировать ошибки» или файл):' 'warn'; foreach ($er in $Script:ErrorReports) { Log "  $er" 'warn' } }
    if ($Script:MsbuildLogs.Count) { Log 'Полные логи MSBuild (как окно Output в VS):' 'dim'; foreach ($m in $Script:MsbuildLogs) { if (Test-Path -LiteralPath $m) { Log "  $m" 'dim' } } }
    if ($allOk -and -not $CleanOnly -and -not $NoInstall) { Log 'Ableton: если был открыт — перезапусти. Путь и имя плагина не менялись, полный рескан не нужен.' 'dim' }
    Log "Журнал: $($Script:LogFile)" 'dim'
    Log $head $(if ($allOk) { 'ok' } else { 'err' })
    return @{ Ok = $allOk; Head = $head }
}

$exitCode = 1
Load-Cfg
# Persistent cache defaults for GUI and unattended -NoPicker use. Explicit CLI
# switches/paths win; existing user config is retained.
if (-not $PSBoundParameters.ContainsKey('KeepBuild') -and $null -ne $Script:Cfg['DeleteBuilds']) { $KeepBuild = -not (Get-CfgBool 'DeleteBuilds' $false) }
if (-not $PSBoundParameters.ContainsKey('ExternalBuilds')) { $ExternalBuilds = Get-CfgBool 'ExternalBuilds' $true }
if (-not $PSBoundParameters.ContainsKey('UseSccache')) { $UseSccache = Get-CfgBool 'UseSccache' $true }
if (-not $PSBoundParameters.ContainsKey('BuildCacheRoot') -and $Script:Cfg['BuildCacheRoot']) { $BuildCacheRoot = [string]$Script:Cfg['BuildCacheRoot'] }
if (-not $PSBoundParameters.ContainsKey('SccacheExe') -and $Script:Cfg['SccacheExe']) { $SccacheExe = [string]$Script:Cfg['SccacheExe'] }
if (-not $PSBoundParameters.ContainsKey('SccacheDir') -and $Script:Cfg['SccacheDir']) { $SccacheDir = [string]$Script:Cfg['SccacheDir'] }
if (-not $PSBoundParameters.ContainsKey('SccacheSize') -and $Script:Cfg['SccacheSize']) { $SccacheSize = [string]$Script:Cfg['SccacheSize'] }
$initDirs = @($Path | ForEach-Object { $_.Trim('"') } | Where-Object { $_ })
if ($Worker -or $NoPicker -or -not $Script:Gui) {
    # консольный режим
    Reset-Run
    try {
        $opt = @{ Dirs = $initDirs; CloseHosts = [bool]$CloseHosts; KeepBuild = [bool]$KeepBuild; CleanOnly = [bool]$CleanOnly; Flat = [bool]$Flat; Dir = $Vst3Dir; OtherDir = $OtherDir; Formats = $Formats; NoManifest = [bool]$NoManifest; ErrorReport = (-not $NoErrorReport); CleanModules = [bool]$CleanModules; GlobalModules = [bool]$GlobalModules; AsyncDelete = [bool]$AsyncDelete; NoPdb = [bool]$NoPdb; ExternalBuilds = [bool]$ExternalBuilds; BuildCacheRoot = $BuildCacheRoot; UseSccache = [bool]$UseSccache; SccacheExe = $SccacheExe; SccacheDir = $SccacheDir; SccacheSize = $SccacheSize }
        if ($OptFile -and (Test-Path -LiteralPath $OptFile)) {
            $j = Get-Content -LiteralPath $OptFile -Raw | ConvertFrom-Json
            $opt = @{ Dirs = @($j.Dirs); CloseHosts = [bool]$j.CloseHosts; KeepBuild = [bool]$j.KeepBuild; CleanOnly = [bool]$j.CleanOnly; Flat = [bool]$j.Flat; Dir = [string]$j.Dir; OtherDir = [string]$j.OtherDir; Formats = @($j.Formats); NoManifest = [bool]$j.NoManifest; ErrorReport = [bool]$j.ErrorReport; CleanModules = [bool]$j.CleanModules; GlobalModules = [bool]$j.GlobalModules; AsyncDelete = [bool]$j.AsyncDelete; NoPdb = [bool]$j.NoPdb; ExternalBuilds = [bool]$j.ExternalBuilds; BuildCacheRoot = [string]$j.BuildCacheRoot; UseSccache = [bool]$j.UseSccache; SccacheExe = [string]$j.SccacheExe; SccacheDir = [string]$j.SccacheDir; SccacheSize = [string]$j.SccacheSize }
            Remove-Item -LiteralPath $OptFile -Force -EA SilentlyContinue
        }
        Log ("Папки: " + ($opt.Dirs -join ' | ')) 'dim'
        $res = Invoke-Build $opt
        $exitCode = if ($res.Ok) { 0 } else { 1 }
        if ($Worker) { [Console]::Out.WriteLine('@D|' + $(if ($res.Ok) { 'OK' } else { 'ERR' }) + '|' + $res.Head); [Console]::Out.Flush() }
    } catch { Log ("ОШИБКА: " + $_.Exception.Message) 'err'; Log ("  строка " + $_.InvocationInfo.ScriptLineNumber) 'err'; if ($Worker) { [Console]::Out.WriteLine('@D|ERR|ОШИБКА: ' + $_.Exception.Message) }; $exitCode = 1 }
} else {
    # если нет прав на Program Files -- сразу перезапускаемся с UAC, чтобы окно было одно
    $probeDir = $(if ($Script:Cfg['Vst3Dir']) { [string]$Script:Cfg['Vst3Dir'] } else { $Vst3Dir })
    if (-not (Is-Admin) -and $probeDir -like "$env:ProgramFiles*") {
        $probe = Join-Path $probeDir ('.w' + [guid]::NewGuid().ToString('N')); $canWrite = $false
        try { [IO.File]::WriteAllText($probe, 'x'); Remove-Item -LiteralPath $probe -Force; $canWrite = $true } catch { }
        if (-not $canWrite) {
            $argList = @('-NoProfile', '-STA', '-WindowStyle', 'Hidden', '-ExecutionPolicy', 'Bypass', '-File', "`"$PSCommandPath`"")
            if ($initDirs.Count) { $argList += @('-Path', (($initDirs | ForEach-Object { '"' + $_ + '"' }) -join ',')) }
            try { Start-Process powershell.exe -Verb RunAs -ArgumentList $argList; exit 0 } catch { }
        }
    }
    Show-Main $initDirs { param($opt) Invoke-Build $opt }
    $exitCode = 0
}
exit $exitCode
