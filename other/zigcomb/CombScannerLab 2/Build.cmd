@echo off
rem Главный ярлык: собрать VST3 через Projucer + MSBuild (как в Monomachine-Nova).
rem Параметры (Debug, -CopyToVST3Folder и т.д.) — см. tools\BuildProjucer.ps1
setlocal
powershell.exe -NoProfile -ExecutionPolicy Bypass -File "%~dp0tools\BuildProjucer.ps1" %*
exit /b %errorlevel%
