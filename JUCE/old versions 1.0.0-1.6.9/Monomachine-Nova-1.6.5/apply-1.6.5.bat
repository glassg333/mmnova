@echo off
rem ASCII-only stub: all logic lives in apply-1.6.5.ps1 (UTF-8, PowerShell).
rem Drag the mmnova repo root (or Monomachine-Nova-1.6.4 folder) onto this file.
powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0apply-1.6.5.ps1" -Target "%~1"
pause
