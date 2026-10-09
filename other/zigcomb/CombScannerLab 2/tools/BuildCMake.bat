@echo off
setlocal
set "ROOT=%~dp0.."
powershell.exe -NoProfile -ExecutionPolicy Bypass -File "%ROOT%\tools\BuildCMake.ps1" %*
exit /b %errorlevel%
