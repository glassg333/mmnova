Регион $0002BE-$0002EB (подготовка A перед jsr (r1) = вызовом PROC)
находится в файле disasm_kernel_fx_runner_P00012B-000340.txt.
Ядро перед вызовом PROC передаёт в A питч несущей:
  A = $1D22A * tbl$140000[frac] * 2^int >> 13 * 2,
где pitch (2048 шагов/октава) -> int = pitch>>11, frac = pitch & $7FF.
