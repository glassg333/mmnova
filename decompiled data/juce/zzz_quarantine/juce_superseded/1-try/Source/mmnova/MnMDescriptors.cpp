// ============================================================================
//  MnMDescriptors.cpp — таблица дескрипторов OS 1.32B
//  ВАЖНО (аудит): слот m14 = SWAVE-ENS, а не FX-EQ. Отдельной EQ-машины нет.
// ============================================================================
#include "MnMDescriptors.h"

namespace mmnova {

#define P8(a,b,c,d,e,f,g,h) { a,b,c,d,e,f,g,h }

static const MachineDescriptor kDesc[] =
{
 { MachineId::GND_GND,   "GND-GND",    "GND",   0x57FC5,
   P8("---","---","---","---","---","---","---","---"),
   { 0,0,0,0,0,0,0,0 }, false },

 { MachineId::GND_SIN,   "GND-SIN",    "SIN",   0x58075,
   P8("---","---","---","---","---","---","---","TUNE"),
   { 0,0,0,0,0,0,0,0x40 }, false },

 { MachineId::GND_NOIS,  "GND-NOIS",   "NOIS",  0x58125,
   P8("ST","RED","STON","---","---","---","---","TUNE"),
   { 0,0,0,0,0,0,0,0x40 }, false },

 { MachineId::SID_6581,  "SID-6581",   "6581",  0x581D5,
   P8("PW","PWAD","PWRS","WAVE","MOD","MSRC","MFRQ","TUNE"),
   { 0x00,0x00,0x60,0x00,0x00,0x00,0x40,0x40 }, false },

 { MachineId::SWAVE_SAW, "SWAVE-SAW",  "SAW",   0x58285,
   P8("UNIL","UNIW","UNIX","---","SUBX","SUB1","SUB2","TUNE"),
   { 0,0,0,0,0,0,0,0x40 }, false },

 { MachineId::SWAVE_PULS,"SWAVE-PULS", "PULS",  0x58335,
   P8("UNIL","UNIW","SUB1","SUB2","PW","PWAD","PWRS","TUNE"),
   { 0,0,0,0,0x40,0,0,0x40 }, false },

 { MachineId::DPRO_WAVE, "DPRO-WAVE",  "WAVE",  0x583E5,
   P8("WAVE","WP","WPM","WPRS","SYNC","SFRQ","---","TUNE"),
   { 0,0,0,0x60,0,0,0,0x40 }, false },

 { MachineId::DPRO_BBOX, "DPRO-BBOX",  "BBOX",  0x58495,
   P8("PTCH","STRT","---","---","RTRG","RTIM","---","---"),
   { 0x40,0,0,0,0,0,0,0 }, false },

 { MachineId::FM_STAT,   "FM-STAT",    "STAT",  0x58545,
   P8("1FRQ","1FIN","1ENV","1FB","2FRQ","2VOL","TONE","TUNE"),
   { 0x3C,0x40,0x50,0x1E,0x50,0x40,0x62,0x40 }, false },

 { MachineId::FM_PAR,    "FM-PAR",     "PAR",   0x585F5,
   P8("1FRQ","1ENV","2FRQ","2ENV","3FRQ","3ENV","TONE","TUNE"),
   { 0x3C,0x40,0x50,0x40,0x66,0x50,0x62,0x40 }, false },

 { MachineId::FM_DYN,    "FM-DYN",     "DYN",   0x586A5,
   P8("1FRQ","1FEN","1VOL","1VEN","2FRQ","2ENV","2FB","TUNE"),
   { 0x40,0x40,0x40,0x40,0x4A,0x50,0x1E,0x40 }, false },

 { MachineId::VO_6,      "VO-VO-6",    "VO-6",  0x58755,
   P8("VOC1","VOC2","V-SW","VOIC","CONS","CLEN","CVOL","TUNE"),
   { 0x40,0x40,0x60,0x00,0x00,0x40,0x40,0x40 }, false },

 { MachineId::FX_THRU,   "FX-THRU",    "THRU",  0x58805,
   P8("---","---","---","---","---","---","---","INP"),
   { 0,0,0,0,0,0,0,0x40 }, true },

 { MachineId::FX_REVERB, "FX-REVERB",  "REV",   0x588B5,
   P8("DEC","DAMP","GATE","MIX","HP","LP","---","INP"),
   { 0x40,0x02,0x7F,0x20,0x00,0x7F,0x00,0x40 }, true },

 // m14: SWAVE-ENS (НЕ FX-EQ!) — дескриптор ColdFire @0x58965
 { MachineId::SWAVE_ENS, "SWAVE-ENS",  "ENS",   0x58965,
   P8("PCH2","PCH3","PCH4","WAVE","PW","CHRL","CHRW","TUNE"),
   { 0x3F,0x3F,0x3F,0x00,0x40,0x00,0x7F,0x40 }, false },

 { MachineId::FX_CHORUS, "FX-CHORUS",  "CHO",   0x58A15,
   P8("DEL","DEP","SPD","MIX","FB","WID","LP","INP"),
   { 0x40,0x40,0x40,0x7F,0x00,0x00,0x7F,0x40 }, true },

 { MachineId::FX_DYNAMIX,"FX-DYNAMIX", "DYNMX", 0x58AC5,
   P8("ATK","REL","THRS","MIX","RAT","GAIN","RMS","INP"),
   { 0x40,0x40,0x40,0x7F,0x00,0x00,0x00,0x40 }, true },

 { MachineId::FX_RINGMOD,"FX-RINGMOD", "RING",  0x58B75,
   P8("WAVE","EXT","---","MIX","---","---","---","INP"),
   { 0x40,0x40,0x00,0x7F,0x00,0x00,0x00,0x40 }, true },

 { MachineId::FX_PHASER, "FX-PHASER",  "PHA",   0x58C25,
   P8("CNTR","DEP","SPD","MIX","FB","WID","---","INP"),
   { 0x40,0x40,0x40,0x7F,0x2D,0x00,0x7F,0x40 }, true },

 { MachineId::FX_FLANGER,"FX-FLANGER", "FLA",   0x58CD5,
   P8("DEL","DEP","SPD","MIX","FB","WID","---","INP"),
   { 0x40,0x40,0x40,0x7F,0x2D,0x00,0x7F,0x40 }, true },

 // страницы B/C — найдены аудитом, в «народной» карте машин отсутствуют
 { MachineId::DPRO_DDRW, "DPRO-DDRW",  "DDRW",  0x58D85,
   P8("WAV1","MIX","WAV2","TIME","BR1","WID","BR2","TUNE"),
   { 0x00,0x40,0x00,0x40,0x00,0x40,0x00,0x40 }, false },

 { MachineId::DPRO_DENS, "DPRO-DENS",  "DENS",  0x58E35,
   P8("PCH2","PCH3","PCH4","WAVE","---","CHRL","CHRW","TUNE"),
   { 0x3F,0x3F,0x3F,0x00,0x00,0x00,0x7F,0x40 }, false },

 { MachineId::FX_DLY,    "FX-DLY",     "DLY",   0x58EE5,
   P8("TIME","FB","---","MIX","LP","WID","---","INP"),
   { 0x40,0x40,0x00,0x7F,0x7F,0x00,0x00,0x40 }, true },

 { MachineId::FX_EXT,    "FX-EXT",     "EXT",   0x58F95,
   P8("SRR","BITS","---","MIX","DRIV","---","---","INP"),
   { 0x7F,0x00,0x00,0x7F,0x00,0x00,0x00,0x40 }, true },
};

#undef P8

const MachineDescriptor* descriptors()    { return kDesc; }
int  descriptorCount()                    { return (int) (sizeof (kDesc) / sizeof (kDesc[0])); }

const MachineDescriptor* descriptorFor (MachineId id)
{
    for (int i = 0; i < descriptorCount(); ++i)
        if (kDesc[i].id == id) return &kDesc[i];
    return &kDesc[0];
}

} // namespace mmnova
