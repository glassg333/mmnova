# Патчи dsp56kEmu, необходимые для прогона ядра MNM

Ядро Monomachine использует три инструкции, которых нет в интерпретаторе
dsp56k-emulator (master 2026). Патч добавляет их (сделано в нашей копии):

1. **MACI (+/-)#xxxx,S,D** — `source/dsp56kEmu/dsp_ops_alu.inl`, `op_Maci_xxxx`:
   было `errNotImplemented`. Реализация:
   ```cpp
   const bool  ab     = getFieldValue<Maci_xxxx,Field_d>(op);
   const bool  negate = getFieldValue<Maci_xxxx,Field_k>(op);
   const TWord qq     = getFieldValue<Maci_xxxx,Field_qq>(op);
   const TReg24 s    = TReg24(immediateDataExt<Maci_xxxx>());
   const TReg24 reg  = decode_qq_read(qq);
   alu_mac( ab, s, reg, negate, false );
   ```
2. **MPYRI** — там же, `op_Mpyri`: то же + `alu_rnd(ab)` после `alu_mpy`.
3. **PFLUSH** — `dsp_ops.inl`, `op_Pflush`: для интерпретатора это no-op
   (кэш инструкций всегда когерентен): делегирует `cache.pflushun()`.
4. Опционально: `dspconfig.h` — принудительно `g_jitSupported = false`
   (интерпретатор; JIT не нужен и требует asmjit со старым layout).
5. Для сборки с новым asmjit: добавить шим `asmjit/x86/x86operand.h`
   (include `asmjit/x86/x86_operand.h` + `asmjit::Operand::isValid()` alias),
   и в `dsp.h/dsp.cpp` гвард `DSP56300_NO_JIT` вокруг члена `Jit m_jit`.

Без 1) ядро падает на P:$055A (`maci #>$800,x0,a` — ветка AMP/PAN),
без 3) — на P:$0B4B (`pflush` — конец голоса).
