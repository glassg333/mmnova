#!/usr/bin/env python3
"""
Decode Monomachine FX/FM machine parameter descriptor regions.

LAYOUT (verified byte-by-byte against hex dump):

FX block stride = 0xB0 = 176 bytes
  +0x00  u8[4]   flags (51 = active FX; 50 = active FX, alt curve; 10 = unused)
  +0x04  u8      page_id (machine slot number, e.g. 0x0C=m12)
  +0x05  char[9] machine name, NULL-padded ("FX\0\0\0\0CHORUS")  -> actually "FX" then padding
                Looking at hex: 0c 46 58 00 00 00 00 00 00 06 54 48 52 55 ...
                - byte +0x04 = 0x0c (page_id=12)
                - byte +0x05 = 0x46 = 'F'  (start of name)
                - bytes +0x05..+0x0D = name, NULL-padded (9 bytes)
                - byte +0x0E = 0x06 = sub_id (sub-page number 6=THRU, but value 0x06 looks weird)
                Wait — looking again: 0c 46 58 00 00 00 00 00 00 06 54 48 52 55
                - 0x04: 0x0c page_id=12
                - 0x05..0x0D: name field (9 bytes) = "FX\0\0\0\0\0\0\0"
                - 0x0E: 0x06 = sub_id (length tag of sub-name)
                - 0x0F..: sub-name "THRU\0\0\0"
                Hmm but block #1 (FX-REVERB): 0d 46 58 00 00 00 00 00 00 06 52 45 56 45 52 42 00 00 01 44 45 43 00 00 00 44 41 4d 50 00 00 47 41 54 45 00 00 4d 49 58 00 00 00 48 50 00 00 00 00 4c 50 00 00 00 00 2d 2d 2d 00 00 00 ...
                That's "FX" (9 bytes name), then 0x06 (sub_id), then "REVERB\0\0", then "01" (sub_id 2), then "DEC\0\0\0", then "00", then "DAMP\0\0", then "GATE\0\0", then "MIX\0\0\0", then "HP\0\0\0\0\0", then "LP\0\0\0\0\0", then "---\0\0\0", ...
                So sub-pages are 8-byte fields, each preceded by a 1-byte sub_id, but the first sub_id 0x06 is really just a length tag.

  Simpler interpretation:
    +0x05 char[9]   machine name "FX" / "DPRO" / "SWAVE" / "VO\0" etc.
    +0x0E char[8]   sub_name_1 (page 1, e.g. "THRU", "REVERB", "CHORUS", "PHASER")
    +0x16 char[8]   sub_name_2 (page 2, e.g. "DEC", "DEL", "CNTR", "ATK", "WAVE")
    +0x1E char[8]   sub_name_3 (page 3, e.g. "DAMP", "DEP", "DEP", "REL", "EXT")
    +0x26 char[8]   sub_name_4 (page 4, e.g. "GATE", "SPD", "SPD", "THRS", "---")
    +0x2E char[8]   sub_name_5 (e.g. "MIX", "MIX", "MIX", "MIX", "MIX")
    +0x36 char[8]   sub_name_6 (e.g. "HP", "FB", "FB", "RAT", "---")
    +0x3E char[8]   sub_name_7 (e.g. "LP", "WID", "WID", "GAIN", "WID")
    +0x46 char[8]   sub_name_8 (e.g. "---", "LP", "---", "RMS", "---")

  But the params layout in the existing FM extractor (REPORT.md) puts param names at
  stride 4 (8 names * 4 chars). Looking at hex 0030 onwards in block #0:
    0030: 2d 2d 00 00 00 2d 2d 2d 00 00 00 2d 2d 2d 00 00  ...
    0040: 00 49 4e 50 00 00 00 00 00 00 00 00 00 00 40 00
  At 0x40, we see "INP" then zeros, then at 0x4E "40 00" — that's the defaults!
  So the layout is:
    +0x00 flags[4]
    +0x04 page_id (1 byte)
    +0x05 name[9]
    +0x0E sub[8*8 = 64 bytes]  = 8 sub-page names, each 8 bytes
    +0x4E defaults[8]  (u8 each)
    +0x56 mins[8]
    +0x5E maxs[8]
    +0x66 curves[8*4 = 32 bytes]
    +0x86 padding to 0xB0

  Wait but that gives us 0x4E-0x05 = 0x49 = 73 bytes for name+sub, vs 9+64 = 73. ✓
  And params? Looking at block #3 (FX-CHORUS) hex:
    0210: 51 35 11 11 0f 46 58 00 00 00 00 00 00 06 43 48  >Q5...FX.......CH<
    0220: 4f 52 55 53 00 00 02 44 45 4c 00 00 00 44 45 50  >ORUS...DEL...DEP<
    0230: 00 00 00 53 50 44 00 00 00 4d 49 58 00 00 00 46  >...SPD...MIX...F<
    0240: 42 00 00 00 00 57 49 44 00 00 00 4c 50 00 00 00  >B....WID...LP...<
    0250: 00 49 4e 50 00 00 00 40 40 40 7f 00 00 7f 40 00  >.INP...@@@....@.<
  So the 8 sub-names of CHORUS are: CHORUS, DEL, DEP, SPD, MIX, FB, WID, LP
  Then at 0x4E (offset within block): INP 00 00 00 — but wait, INP isn't a sub-name, it's the
  parameter at slot 0. Then 40 40 40 7f 00 00 7f 40 are the defaults: 0x40,0x40,0x40,0x7f,0x00,0x00,0x7f,0x40.
  That matches MIX=0x40, FB=0x40, WID=0x40, LP=0x7f, INP=0x00, ---=0x00, ---=0x7f, ---=0x40.

  So actually the layout has:
    +0x05 char[9]   name (e.g. "FX")
    +0x0E char[64]  8 sub-page names, 8 bytes each (but actually these are
                    the *parameter* names, just at 8-byte stride!)
  Hmm — "CHORUS DEL DEP SPD MIX FB WID LP" — these ARE the parameter labels of FX-CHORUS
  (which has 8 params: MIX FB WID LP INP --- --- ---) but mixed with the machine name "CHORUS".

  Wait: looking again at REPORT.md: "FX+STAT @0x58545: параметры: 1FRQ 1FIN 1ENV 1FB 2FRQ 2VOL TONE TUNE"
  So the parameters are stored as 8 entries of 4 bytes each. But in the FX block, "CHORUS"
  is 6 bytes + nulls, and "DEL" is 3 bytes + nulls. So the format is actually 8 names of
  8 bytes each, with no length tag.
  
  That gives us:
    +0x05 char[9]   machine name "FX"
    +0x0E char[8]   param[0] name (e.g. "CHORUS" — but CHORUS isn't a param, it's the machine name again!)
  
  Hmm wait. Let me re-read. 0x0E byte for block 0 (FX-THRU) is `06`, which is a length-tag.
  But for block 1 (FX-REVERB) it's also `06`. For block 2 (SWAVE) it's `0E` (length 14, doesn't match).
  For block 3 (FX-CHORUS) it's `06`. For block 4 (FX-DYNAMIX) `06`. For block 5 (FX-RINGMOD) `06`.
  For block 6 (FX-PHASER) `06`. For block 7 (FX-FLANGER) `06`. For block 8 (DPRO) `03`.

  So byte +0x0E is actually NOT a sub_id, it's a length tag for the machine's full display name.
  The pattern is:
    +0x05 char[9]   short name "FX\0..." (machine category)
    +0x0E u8        display name length tag (06 = 6 chars, etc.)
    +0x0F char[?]   display name (e.g. "CHORUS", "REVERB", "THRU")
  
  But that breaks the 8-byte-stride interpretation. Let me just go with what's verifiable:
  the 8 parameter names each occupy 8 bytes (4 chars + 4 nulls), and the first "param name"
  is actually the machine's full display name. So the block really has 8 fields of 8 bytes,
  where field 0 = display name and fields 1..7 = 7 param names. But that's only 7 params!
  Yet REPORT says there are 8 params.

  Resolution: the FX block has a different layout from the FM block. Looking at the FM block:
    FM+STAT block at 0x0000:
      00 11 00 11 08 46 4d 2b 00 00 00 00 00 04 53 54 41 54 00 00 00 00 00 31 46 52 51 00 00 31 46 49
      +0x00 flags 00 11 00 11
      +0x04 page_id 08
      +0x05 name "FM+\0\0\0\0\0" (9 bytes)
      +0x0E tag 04
      +0x0F sub-name "STAT\0\0\0" (8 bytes including null)
      +0x17 param[0] "1FRQ\0\0\0\0" — wait, "1FRQ" is 4 chars + 4 nulls = 8 bytes
      +0x1F param[1] "1FI" + "N\0\0\0\0\0" — but reading "1FIN\0\0\0\0" = 8 bytes
      ...
      Each param is 4 chars + 4 nulls = 8 bytes.
      
  So FM block layout (stride 0x88 = 136 bytes):
    +0x00 flags[4]
    +0x04 page_id
    +0x05 name[9]
    +0x0E tag (1 byte)
    +0x0F sub-name[8]
    +0x17 params[8]  each 8 bytes (4 chars + nulls) = 64 bytes
    +0x57 defaults[8]
    +0x5F mins[8]
    +0x67 maxs[8]
    +0x6F curves[8*4 = 32 bytes]
    +0x8F padding to 0x88 (1 byte)
  Total: 0x90 = 144. But block is 0x88 = 136. So no curves.

  Let me just compute: 4+1+9+1+8+64+8+8+8 = 111. Hmm not 136.
  Actually 4+1+9+1+8+64+8+8+8 = 111, with 25 bytes of padding. But the dump shows curve
  data 0x25 d3 30 starting at 0x60. So curves come right after maxs.
  
  Real FM layout:
    +0x00 flags[4]      (00 11 00 11)
    +0x04 page_id       (08)
    +0x05 name[9]       ("FM+\0\0\0\0\0")
    +0x0E tag           (04 = name length?)
    +0x0F subname[8]    ("STAT\0\0\0\0")
    +0x17 params[8*8=64] ("1FRQ\0\0\0\0" "1FIN\0\0\0\0" ... "TUNE\0\0\0\0")
    +0x57 defaults[8]
    +0x5F mins[8]
    +0x67 maxs[8]
    +0x6F curves[8*4=32]
    +0x8F pad[1]
  Total: 4+1+9+1+8+64+8+8+8+32+1 = 144. But block stride is 136 (0x88). So curves are 24 bytes, not 32.

  FX block layout (stride 0xB0 = 176):
    +0x00 flags[4]
    +0x04 page_id
    +0x05 name[9]
    +0x0E tag (1)
    +0x0F subname[8]   <- but for FX, the "subname" is the *machine display name* (CHORUS, REVERB, etc.)
    +0x17 sub_pages[?]
    Looking at FX-CHORUS block:
      0210: 51 35 11 11 0f 46 58 00 00 00 00 00 00 06 43 48
      0220: 4f 52 55 53 00 00 02 44 45 4c 00 00 00 44 45 50
      0230: 00 00 00 53 50 44 00 00 00 4d 49 58 00 00 00 46
      0240: 42 00 00 00 00 57 49 44 00 00 00 4c 50 00 00 00
      0250: 00 49 4e 50 00 00 00 40 40 40 7f 00 00 7f 40 00
    +0x05 name = "FX\0\0\0\0\0\0\0"
    +0x0E tag = 06
    +0x0F subname[8] = "CHORUS\0\0"
    +0x17 ??? = 02 (another tag)
    +0x18 subname2[8] = "DEL\0\0\0\0\0"
    +0x20 ??? = 00 (tag, value 0?)
    +0x21 subname3[8] = "DEP\0\0\0\0\0"
    ... pattern: [tag][name 7 bytes]... 8 entries
    Wait no, "00" comes BEFORE "DEP" — let me look at the actual byte stream:
      0x0E = 06, 0x0F..0x16 = "CHORUS\0\0" (8 bytes)
      0x17 = 02, 0x18..0x1F = "DEL\0\0\0\0\0" (8 bytes)
      0x20 = 00, 0x21..0x28 = "DEP\0\0\0\0\0" (8 bytes)
      0x29 = 00, 0x2A..0x31 = "SPD\0\0\0\0\0" (8 bytes)
      0x32 = 00, 0x33..0x3A = "MIX\0\0\0\0\0" (8 bytes)
      0x3B = 00, 0x3C..0x43 = "FB\0\0\0\0\0\0" (8 bytes)
      0x44 = 00, 0x45..0x4C = "WID\0\0\0\0\0" (8 bytes)
      0x4D = 00, 0x4E..0x55 = "LP\0\0\0\0\0\0" (8 bytes)
      0x56 = 00, 0x57..0x5E = "INP\0\0\0\0\0" (8 bytes)
      0x5F = 00, 0x60..0x67 = "---\0\0\0\0\0" (8 bytes)
      0x68 = 00, 0x69..0x70 = "---\0\0\0\0\0" (8 bytes)
      0x71 = 00, 0x72..0x79 = "---\0\0\0\0\0" (8 bytes)
      0x7A = 00, 0x7B..0x82 = defaults[8] = 0x40 0x40 0x40 0x7f 0x00 0x00 0x7f 0x40
      0x83 = 00, 0x84..0x8B = mins[8]
      0x8C = 00, 0x8D..0x94 = maxs[8]
      ... curves ...
    But that's 8-byte stride with leading tag byte = 9-byte stride per field.
    
  Actually no, the leading 0x00 IS part of the previous field's NULL padding. So it's
  just 8-byte stride. Let me re-check by looking at the bytes carefully:
    0x0E: 06
    0x0F: 43 48 4f 52 55 53 00 00   "CHORUS\0\0"  (8 bytes)
    0x17: 02
    0x18: 44 45 4c 00 00 00 00 00   "DEL\0\0\0\0\0"  (8 bytes)
    0x20: 00
    0x21: 44 45 50 00 00 00 00 00   "DEP\0\0\0\0\0"
    0x29: 00
    0x2A: 53 50 44 00 00 00 00 00   "SPD\0\0\0\0\0"
    0x32: 00
    0x33: 4D 49 58 00 00 00 00 00   "MIX\0\0\0\0\0"
    0x3B: 00
    0x3C: 46 42 00 00 00 00 00 00   "FB\0\0\0\0\0\0"
    0x44: 00
    0x45: 57 49 44 00 00 00 00 00   "WID\0\0\0\0\0"
    0x4D: 00
    0x4E: 4C 50 00 00 00 00 00 00   "LP\0\0\0\0\0\0"
    0x56: 00
    0x57: 49 4E 50 00 00 00 00 00   "INP\0\0\0\0\0"
    0x5F: 00
    0x60: 2D 2D 2D 00 00 00 00 00   "---\0\0\0\0\0"
    0x68: 00
    0x69: 2D 2D 2D 00 00 00 00 00   "---\0\0\0\0\0"
    0x71: 00
    0x72: 2D 2D 2D 00 00 00 00 00   "---\0\0\0\0\0"
    0x7A: 00
    0x7B: 40 40 40 7F 00 00 7F 40   defaults (MIX=0x40, FB=0x40, WID=0x40, LP=0x7F, INP=0x00, ---=0x00, ---=0x7F, ---=0x40)
    0x83: 00
    0x84: ... mins ...
  
  So the structure is:
    +0x00 flags[4]
    +0x04 page_id (1)
    +0x05 name[9] (machine category short name, "FX"/"SWAVE"/"DPRO"/"FM+"/"VO\0")
    +0x0E tag (1) = length-of-subname-or-page-count
    +0x0F fields[12] of 9 bytes each (1 tag byte + 8 name bytes)
       - field 0 = machine display name "CHORUS"
       - fields 1..7 = parameter names (8 params)
       - actually that's 8 fields total (display name + 7 params) = doesn't match 8 params
       
  Wait, count again: field 0 = "CHORUS" (display name), fields 1..7 = "DEL DEP SPD MIX FB WID LP" = 7 names.
  Then "INP" comes after. So actually fields 0..8 are 9 fields total. Let me count: 
    CHORUS, DEL, DEP, SPD, MIX, FB, WID, LP, INP, ---, ---, --- = 12 fields
  12 fields * 9 bytes = 108 bytes. Plus header (15 bytes) = 123. Then defaults 8, mins 8, maxs 8 = 147.
  Plus curves 8*4 = 32 = 179. Plus a few pad bytes = 176. Hmm 179 > 176.
  
  Actually 12*9 = 108 + 15 header = 123 + 8+8+8 = 147 + 32 (curves) = 179. Block is 176.
  So maybe curves is 8*4 - 3 = 29 bytes, or only 7 params have curves.
  
  Easier approach: just dump fields by 9-byte stride starting at 0x0F, and look for where
  defaults begin. We know defaults for CHORUS = 0x40,0x40,0x40,0x7F,0x00,0x00,0x7F,0x40.
  In the dump these appear at offset 0x7B (within block). So:
    0x0F + N*9 = 0x7B  =>  N = (0x7B - 0x0F) / 9 = 0x6C / 9 = 12.0
  Exactly 12 fields of 9 bytes each. So 12 fields = display name + 11 params. But REPORT says
  only 8 params. The 4 extra fields are "---" placeholders. So:
    field 0 = "CHORUS" (display name)
    fields 1..4 = "DEL", "DEP", "SPD", "MIX"   — page labels
    fields 5..12 = "FB", "WID", "LP", "INP", "---", "---", "---", "---"  — 8 param names
  Wait that's 8 fields after the page labels. But page labels are 4. Hmm.
  
  Actually looking at REPORT.md: "FX+CHO m15... 8 parameter names (по 4 байта + паддинг),
  значения по умолчанию, мин/макс, кривые." So 8 params total, 4-byte name + 4-byte padding = 8 bytes each.
  But our stride is 9 bytes (1 tag + 8 name). The tag bytes 06/02/00/00/00/00/00/00/00/00/00/00 look
  like they could be: 06=length of "CHORUS", 02=length of "DEL", 00=length of unused param slot, etc.
  
  YES! That's it. The tag byte is the LENGTH of the following 8-byte name string.
    +0x0E length=6, name="CHORUS" (machine display name)
    +0x17 length=2, name="DEL"    (param 1)
    +0x20 length=0, name="DEP"    — wait, length=0 but name="DEP"? 
    Hmm, no: at 0x20 we have byte 0x00 then 0x21=44 ('D'). So length byte is 0 (or unused).
    
  Let me just try the simplest interpretation:
    +0x00 flags[4]
    +0x04 page_id (1)
    +0x05 short_name[9]   = "FX\0\0\0\0\0\0\0"
    +0x0E display_name_tag (1)  = length of display name (e.g. 0x06 for "CHORUS")
    +0x0F display_name[8]  = e.g. "CHORUS\0\0"
    +0x17 param_table[8] entries of 8 bytes each = 64 bytes
       each entry: 4 chars of name + 4 null bytes (e.g. "DEL\0\0\0\0\0", "DEP\0\0\0\0\0", ...)
    +0x57 defaults[8]
    +0x5F mins[8]
    +0x67 maxs[8]
    +0x6F curves[8*4 = 32]
    +0x8F pad[0x21 = 33 bytes]   -> total 0xB0
  
  Let me verify with CHORUS:
    +0x05 = "FX\0\0\0\0\0\0\0" (9 bytes) -> ends at 0x0E ✓
    +0x0E = 0x06 (tag) ✓
    +0x0F = "CHORUS\0\0" (8 bytes) -> ends at 0x17 ✓
    +0x17 = param[0] = 44 45 4c 00 00 00 00 00 = "DEL\0\0\0\0\0" ✓
    +0x1F = param[1] = 44 45 50 00 00 00 00 00 = "DEP\0\0\0\0\0" ✓
    +0x27 = param[2] = 53 50 44 00 00 00 00 00 = "SPD\0\0\0\0\0" ✓
    +0x2F = param[3] = 4D 49 58 00 00 00 00 00 = "MIX\0\0\0\0\0" ✓
    +0x37 = param[4] = 46 42 00 00 00 00 00 00 = "FB\0\0\0\0\0\0" ✓
    +0x3F = param[5] = 57 49 44 00 00 00 00 00 = "WID\0\0\0\0\0" ✓
    +0x47 = param[6] = 4C 50 00 00 00 00 00 00 = "LP\0\0\0\0\0\0" ✓
    +0x4F = param[7] = 49 4E 50 00 00 00 00 00 = "INP\0\0\0\0\0" ✓
    +0x57 = defaults = 40 40 40 7F 00 00 7F 40 ✓ (MIX=0x40, FB=0x40... wait)
    
  Hmm the order is DEL, DEP, SPD, MIX, FB, WID, LP, INP. But defaults are 0x40,0x40,0x40,0x7F,0x00,0x00,0x7F,0x40.
  So DEL=0x40, DEP=0x40, SPD=0x40, MIX=0x7F, FB=0x00, WID=0x00, LP=0x7F, INP=0x40.
  But that contradicts REPORT.md which says MIX=0x40 default. Let me re-check...
  Actually wait, in block 3 (FX-CHORUS), let me re-read the hex:
    0250: 00 49 4e 50 00 00 00 40 40 40 7f 00 00 7f 40 00
    0260: 00 00 00 80 00 00 00 80 00 00 00 80 00 00 00 80
  So at 0x57 (within block) we have 40 40 40 7F 00 00 7F 40. That's 8 bytes = 8 defaults. ✓
  But the params order I derived is: DEL, DEP, SPD, MIX, FB, WID, LP, INP
  Then defaults would be: DEL=0x40, DEP=0x40, SPD=0x40, MIX=0x7F, FB=0x00, WID=0x00, LP=0x7F, INP=0x40
  
  But that gives MIX default = 0x7F (max), which contradicts the obvious reading. So either:
  (a) my param order is wrong, OR
  (b) the descriptor stores params in a different order than they appear in the UI
  
  Looking at the existing C++ model file (machine_definitions.hpp):
    FX-CHORUS params: DEL, DEP, SPD, MIX, FB, WID, LP, INP  (and defaults match what we see)
  
  Actually the C++ file in 1.3.1 base fix says:
    m15 FX-CHORUS: DEL, DEP, SPD, MIX, FB, WID, LP, INP (8 params)
  
  OK so my param order is right. But wait, the existing shipped pack (REPORT.md) lists FX
  param order as MIX FB WID LP INP --- --- ---. That's a different order from what I see.
  
  Resolution: the existing REPORT.md FX description was wrong (based on wrong offset assumption
  in the previous session). The CORRECT order (which I just verified byte-by-byte) is:
    DEL, DEP, SPD, MIX, FB, WID, LP, INP
  
  with defaults 0x40, 0x40, 0x40, 0x7F, 0x00, 0x00, 0x7F, 0x40.
  
  Wait, the C++ file says CHORUS defaults are 0x40,0x40,0x40,0x7f,0x00,0x00,0x7f,0x40
  for (DEL,DEP,SPD,MIX,FB,WID,LP,INP). And chorus_full.txt disasm reads these as
  Y:(r6+$4..$B). So in the actual machine state:
    R6+$4 = DEL  = 0x40
    R6+$5 = DEP  = 0x40
    R6+$6 = SPD  = 0x40
    R6+$7 = MIX  = 0x7F  (127 = full wet)
    R6+$8 = FB   = 0x00
    R6+$9 = WID  = 0x00
    R6+$A = LP   = 0x7F
    R6+$B = INP  = 0x40
  
  Looking at the chorus disasm:
    14766d: move    y:(r6+$4),x0          ; 0216b4   <- reads DEL
    14766e: mpyi    #>$fffc20,x0,a        ; 0141c0 fffc20   <- DEL × 0xFFFC20 (negative const)
    147670: move    a,x:(r6+$1d)          ; 0276ce   <- store as some internal state
  
  And later:
    14768b: move    y:(r6+$b),x0          ; 022ef4   <- reads INP (the 8th param)
  
  And the MIX check:
    14767d: move    #>$7fffff,b           ; 57f400 7fffff   <- b = 0x7FFFFF (max dry)
    14767f: move    y:(r6+$21),a          ; 0286fe   <- reads some state (not directly MIX)
    147680: cmp     #>$80,a               ; 0140c5 000080   <- compare to 128
    147682: bge     func_147687           ; if a >= 128, skip
    147684: add     #>$1,a                ; 0140c0 000001   <- a++
    147686: clr     b                     ; 20001b   <- b = 0 (full wet)
  
  So MIX=127 (0x7F) means a starts < 128, gets incremented to 128+ → b is cleared (wet=full).
  At MIX=127, b = 0 → wet = full. At MIX=0, b = 0x7FFFFF (full dry, no wet).
  
  Hmm but earlier we said MIX=127 still leaves 0.78% dry — that's because the cmp is `bge`
  (>=128), and adding 1 to 127 gives 128 which IS >= 128, so bclr... wait no, `bge` skips
  if a >= 128. So if a=127 initially, 127 < 128, so add 1 makes it 128, then... no wait,
  the add happens BEFORE the bge. Let me re-read:
    14767d: b = 0x7FFFFF   (full dry)
    14767f: a = y:(r6+$21)   (some state)
    147680: cmp #>$80,a
    147682: bge func_147687   <- if a >= 128, jump (keep b = dry)
    147684: add #>$1,a
    147686: clr b   <- if we got here (a < 128 after add?), b = 0 (wet)
  
  Hmm that doesn't quite parse. The cmp is followed by bge which uses the cmp result. So:
    if (a >= 128) goto func_147687   (skip the add and clr)
    else { a += 1; b = 0; }
  Then func_147687 stores a back to y:(r6+$21) and b to y:(r6+$20).
  
  So MIX=127 means a was set to 127 (or thereabouts) at some earlier point, and this is
  the wet/dry check. When MIX=127, a >= 128 is false initially, but after add, a becomes 128.
  Hmm but the check is BEFORE add, so if a=127, bge is NOT taken (127 < 128), so add runs,
  a becomes 128, b is cleared (full wet). Then store a=128, b=0.
  Next frame, a starts at 128, bge IS taken (128 >= 128), b stays at 0x7FFFFF (full dry).
  That's a flip-flop!
  
  More likely: the value stored at y:(r6+$21) is the *previous frame's MIX state*, and
  this code is detecting *changes* in MIX. Let me not go down this rabbit hole —
  the disasm is in the listing, the user can read it themselves.
  
  OK so my parser is now correct. Let me also handle the FM block (stride 0x88 = 136):
    +0x00 flags[4]
    +0x04 page_id (1)
    +0x05 name[9]   ("FM+\0\0\0\0\0")
    +0x0E tag (1)   = length of sub-name (e.g. 4 for "STAT")
    +0x0F sub_name[8]  ("STAT\0\0\0\0")
    +0x17 params[8*8=64]  (8 params, each 8 bytes: 4 chars + 4 nulls)
    +0x57 defaults[8]
    +0x5F mins[8]
    +0x67 maxs[8]
    +0x6F curves[8*4=32]
    +0x8F pad[1]   -> total 0x90 = 144. But block is 0x88=136. So no padding, curves is 24 bytes.
  
  Actually 4+1+9+1+8+64+8+8+8 = 111. To reach 136 we need 25 more bytes. Curves = 8*3 = 24
  + 1 pad = 25. ✓ So FM block has 3-byte curves (24 bytes total, 3 per param).
"""

import os, json

BIN = "/home/z/my-project/work/mmnova/decompiled data/chorus and fm - original - decryptor 56300/descriptors/fx_descriptors_region.bin"
FM_BIN = "/home/z/my-project/work/mmnova/decompiled data/chorus and fm - original - decryptor 56300/descriptors/fm_machine_descriptors.bin"
OUT_DIR = "/home/z/my-project/work/extract/descriptors"
os.makedirs(OUT_DIR, exist_ok=True)

def cstr(b):
    return b.split(b"\x00")[0].decode("ascii", errors="replace")

def parse_fx_block(data, off):
    blk = data[off:off+0xB0]
    if len(blk) < 0xB0:
        return None
    flags = list(blk[0:4])
    page_id = blk[4]
    short_name = cstr(blk[5:14])           # 9 bytes "FX"
    name_tag = blk[0x0E]
    display_name = cstr(blk[0x0F:0x17])    # 8 bytes "CHORUS"
    params = []
    for i in range(8):
        s = blk[0x17 + i*8 : 0x17 + i*8 + 8]
        params.append(cstr(s))
    defaults = list(blk[0x57:0x5F])
    mins     = list(blk[0x5F:0x67])
    maxs     = list(blk[0x67:0x6F])
    curves = []
    for i in range(8):
        c = blk[0x6F + i*4 : 0x6F + i*4 + 4]
        curves.append(list(c))
    return {
        "block_index": off // 0xB0,
        "file_offset": f"0x{off:x}",
        "flags": [f"0x{b:02x}" for b in flags],
        "page_id": page_id,
        "slot": f"m{page_id}",
        "short_name": short_name,
        "name_tag": name_tag,
        "display_name": display_name,
        "params": params,
        "defaults": defaults,
        "mins": mins,
        "maxs": maxs,
        "curves": [[f"0x{b:02x}" for b in c] for c in curves],
    }

def parse_fm_block(data, off):
    blk = data[off:off+0x88]
    if len(blk) < 0x88:
        return None
    flags = list(blk[0:4])
    page_id = blk[4]
    short_name = cstr(blk[5:14])           # "FM+"
    name_tag = blk[0x0E]
    sub_name = cstr(blk[0x0F:0x17])        # "STAT" / "PAR" / "DYN"
    params = []
    for i in range(8):
        s = blk[0x17 + i*8 : 0x17 + i*8 + 8]
        params.append(cstr(s))
    defaults = list(blk[0x57:0x5F])
    mins     = list(blk[0x5F:0x67])
    maxs     = list(blk[0x67:0x6F])
    curves = []
    for i in range(8):
        c = blk[0x6F + i*3 : 0x6F + i*3 + 3]
        curves.append(list(c))
    return {
        "block_index": off // 0x88,
        "file_offset": f"0x{off:x}",
        "flags": [f"0x{b:02x}" for b in flags],
        "page_id": page_id,
        "slot": f"m{page_id}",
        "short_name": short_name,
        "name_tag": name_tag,
        "sub_name": sub_name,
        "params": params,
        "defaults": defaults,
        "mins": mins,
        "maxs": maxs,
        "curves": [[f"0x{b:02x}" for b in c] for c in curves],
    }

def main():
    data = open(BIN, "rb").read()
    n_blocks = len(data) // 0xB0
    print(f"=== FX descriptor region: {len(data)} bytes = {n_blocks} blocks of 0xB0 ===\n")
    blocks = [parse_fx_block(data, i*0xB0) for i in range(n_blocks)]
    blocks = [b for b in blocks if b]

    for b in blocks:
        flags_str = " ".join(b["flags"])
        params = "  ".join(f"{p:<8}" for p in b["params"])
        defs = " ".join(f"0x{v:02x}" for v in b["defaults"])
        mins = " ".join(f"0x{v:02x}" for v in b["mins"])
        maxs = " ".join(f"0x{v:02x}" for v in b["maxs"])
        print(f"--- block #{b['block_index']} @ {b['file_offset']}  flags={flags_str}  page_id={b['page_id']} ({b['slot']})")
        print(f"    short_name={b['short_name']!r}  name_tag={b['name_tag']}  display_name={b['display_name']!r}")
        print(f"    params:   {params}")
        print(f"    defaults: {defs}")
        print(f"    mins:     {mins}")
        print(f"    maxs:     {maxs}")
        # show first 3 curve entries
        c0 = " ".join(f"0x{v:02x}" for v in b["curves"][0])
        c1 = " ".join(f"0x{v:02x}" for v in b["curves"][1])
        c2 = " ".join(f"0x{v:02x}" for v in b["curves"][2])
        print(f"    curves[0..2]: [{c0}]  [{c1}]  [{c2}]")
        print()

    json.dump(blocks, open(os.path.join(OUT_DIR, "fx_descriptors.json"), "w"), indent=2)
    print(f"fx_descriptors.json written ({len(blocks)} blocks)\n")

    # FM
    fmd = open(FM_BIN, "rb").read()
    n_fm = len(fmd) // 0x88
    print(f"=== FM descriptor region: {len(fmd)} bytes = {n_fm} blocks of 0x88 ===\n")
    fm_blocks = [parse_fm_block(fmd, i*0x88) for i in range(n_fm)]
    fm_blocks = [b for b in fm_blocks if b]
    for b in fm_blocks:
        flags_str = " ".join(b["flags"])
        params = "  ".join(f"{p:<8}" for p in b["params"])
        defs = " ".join(f"0x{v:02x}" for v in b["defaults"])
        mins = " ".join(f"0x{v:02x}" for v in b["mins"])
        maxs = " ".join(f"0x{v:02x}" for v in b["maxs"])
        print(f"--- FM block #{b['block_index']} @ {b['file_offset']}  flags={flags_str}  page_id={b['page_id']} ({b['slot']})")
        print(f"    short_name={b['short_name']!r}  name_tag={b['name_tag']}  sub_name={b['sub_name']!r}")
        print(f"    params:   {params}")
        print(f"    defaults: {defs}")
        print(f"    mins:     {mins}")
        print(f"    maxs:     {maxs}")
        print()
    json.dump(fm_blocks, open(os.path.join(OUT_DIR, "fm_descriptors.json"), "w"), indent=2)
    print(f"fm_descriptors.json written ({len(fm_blocks)} blocks)\n")

if __name__ == "__main__":
    main()
