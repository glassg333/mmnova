@echo off
REM Перетащи на этот файл папку проекта (или несколько) -- или просто запусти. Всё остальное -- в окне.
setlocal EnableDelayedExpansion
set "P="
:loop
if "%~1"=="" goto run
set "P=!P!,'%~1'"
shift
goto loop
:run
if defined P set "P=!P:~1!"
if defined P (
  start "" powershell -NoProfile -STA -WindowStyle Hidden -ExecutionPolicy Bypass -Command "& '%~dp0build_vst3.ps1' -Path @(!P!)"
) else (
  start "" powershell -NoProfile -STA -WindowStyle Hidden -ExecutionPolicy Bypass -File "%~dp0build_vst3.ps1"
)
