# Worklog — mmnova reverse engineering

---
Task ID: 1
Agent: main (Super Z)
Task: Clone glassg333/mmnova, recheck CHORUS decompilation (black fixed folder), extract FM synth data from Monomachine OS 1.32b firmware.

Work Log:
- Cloned repo to /home/z/my-project/repo-mmnova (39 MB, 1 commit).
- Read README.md, monomachine_os132b_reverse_engineering_status.txt, decompiled data/REPORT.md.
- Read CHORUS black fixed: README_RU.md, TECHNICAL_RU.md, TestsResults/original-instruction-comparison.txt.
- Read juce_fx_pack/docs/fm_pending.md + juce pack session log txt.
- Verified firmware SHA-256 = 369849175602e20a9dd2b6e0ad8ac404b76f82718b14afbf1cbc01b7acabec7e (matches TECHNICAL_RU.md).

---
Task ID: 2
Agent: main (Super Z)
Task: Independent re-extraction of DSP firmware images from BIN + chorus verification + FM data extraction.

Work Log:
- Disassembled the firmware's own ColdFire APLIB decompressor (BIN 0x1C4-0x362) with capstone; ported 1:1 to Python.
- Discovered Monomachine aPLib variant differences vs mischa85 tool: NO in-stream EOF marker (raw==767 check never fires); streams run to section end; pour stops on embedded JMP record.
- Disassembled loader func_7ce (BIN 0x7CE): records [cmd LE24][addr LE24][count LE24][data LE24], cmd 0=P 1=X 2=Y, 3=JMP; sections start AND end with JMP P:$000066 record (bytes 03 00 00 66 00 00); byte order in sections = LITTLE-ENDIAN 24-bit words.
- Disassembled boot main (BIN 0x1F6E-0x2078): sec1->DSP1, sec2->DSP2, sec3->both; one depack call per section into RAM 0x200000.
- Extracted all memory images independently; compared against repo's dsp{1,2}_{p,x,y}mem.bin: 0 mismatches across all 6 banks (1,343,588 P-words each).
- Cross-verified against dsp{1,2}_words.bin pour streams (116,186/116,118 words): sec1 first 3,579 words byte-perfect; full value sequences match.
- Chorus region P:$147661-$1477DE byte-identical to repo chorus_region.bin (382 words).
- Verified ChorusCore.cpp claims against authentic listing (init $147661-$14767A, process $14767B-$1477DD, delay X:$114000, width tables $14A000/$14A800, bset #$14,sr, LP table $144AC7, 6 interpolated taps, MIX dry residue ~0.78%).
- Extracted dispatch entries for FM machines: m8 FM+STAT init $145D12/config $145D1D/process $145D21; m9 FM+PAR $145EC9/$145ED4/$145EDB; m10 FM+DYN $14619D/$1461A8/$1461C1.
- Re-extracted ColdFire main (sec0, 425,858 bytes) and located machine parameter descriptors (stride 0xB0): FM+STAT @0x58545 (1FRQ 1FIN 1ENV 1FB 2FRQ 2VOL TONE TUNE), FM+PAR @0x585F5 (1FRQ 1ENV 2FRQ 2ENV 3FRQ 3ENV TONE TUNE), FM+DYN @0x586A5 (1FRQ 1FEN 1VOL 1VEN 2FRQ 2ENV 2FB TUNE) + defaults.
- Analyzed FM machines from listings: pitch conv = engine_pitch x 0x0BE37C -> L:$5 phase; sine lookup with linear interpolation (mpysu/dmac/macsu/asr pattern), 13-bit phase mask $1fff; ratio table P:$141A80 = 24 musical ratios 1/32..8/1 (1.23 fixed point); FM+DYN dynamic wave-table scan P:$141880 by 2ENV (n1=|2ENV-0x400000|>>15); FM+PAR = 3 parallel modulators sharing ratio table; TONE = LP via $144AC7 (shared with chorus).
- Extracted tables to download/mmnova_re/tables/, listings, descriptors, verified memory images.
- Wrote final report download/mmnova_re/REPORT_verdict_and_fm_data.md.
- Scripts persisted: scripts/mm_aplib_mm.py, mm_final_extract.py, mm_full_extract.py, mm_extract.py, mm_verify_all.py, mm_debug_depack.py, mm_locate3.py, mm_trace_eof.py, mm_hypo_b.py, mm_substreams.py, mm_locate.py, mm_diff_words.py, cf_disasm.py, parse_listings.py, probe*.py, build_alu_table.py, solve_regmove.py, dis56300.py (partial DSP56300 disassembler skeleton).

Stage Summary:
- CHORUS in "black fixed" = AUTHENTIC (byte-exact region, structure confirmed). Repo images genuine (0 mismatches, independent chain).
- FM synth data recovered: descriptors (param names/defaults), dispatch addresses, ratio table (24 ratios), DYN wave table, sine table, algorithms per machine.
- Deliverables in /home/z/my-project/download/mmnova_re/.
