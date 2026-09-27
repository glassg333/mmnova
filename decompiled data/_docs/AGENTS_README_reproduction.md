
## ВОСПРОИЗВЕДЕНИЕ БОЛЬШИХ ДАМПОВ (не включены в пакет — генерируются скриптами)

Нужен только BIN прошивки из репозитория glassg333/mmnova:

    python3 scripts/mm_final_extract.py

генерирует:
- my_dsp1_pmem.bin / my_dsp2_pmem.bin (P-образы 1,343,588 слов, 0 расхождений с repo decompiled data/)
- my_dsp{1,2}_{x,y}mem.bin (в пакете)
- sec{1,2,3}_full.bin — сырые потоки записей заливки (LE24)
- mm_work/... 

    python3 -c "import sys; sys.path.insert(0,'scripts'); from mm_final_extract import depack_full; from mm_aplib_mm import read_sections; d=open('elektron_sfx6-60_os1.32b.bin','rb').read(); s=read_sections(d); o,i=depack_full(d,s[0]['off']+8,s[0]['off']+8+s[0]['slen']); open('sec0_full.bin','wb').write(o)"

генерирует sec0_full.bin — ColdFire main (425,858 байт), дескрипторы машин @0x58540+.
