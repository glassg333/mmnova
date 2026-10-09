@echo off
setlocal
set "ROOT=%~dp0.."
powershell.exe -NoProfile -ExecutionPolicy Bypass -File "%ROOT%\tools\BuildProjucer.ps1" %*
exit /b %errorlevel%
