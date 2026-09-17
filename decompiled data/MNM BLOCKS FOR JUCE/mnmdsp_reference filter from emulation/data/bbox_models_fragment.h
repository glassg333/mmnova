// DPRO-BBOX: 12 drum models, note-mapped (P:$147632 in dsp1_pmem)
static constexpr struct { uint32_t start; uint32_t length; } kModels[12] = {
    { 0, 918 },   // abs P:$103E7B..$104211
    { 1977, 4951 },   // abs P:$104634..$10598B
    { 10961, 3008 },   // abs P:$10694C..$10750C
    { 16018, 1025 },   // abs P:$107D0D..$10810E
    { 18478, 1025 },   // abs P:$1086A9..$108AAA
    { 24504, 5001 },   // abs P:$109E33..$10B1BC
    { 37680, 1600 },   // abs P:$10D1AB..$10D7EB
    { 41330, 4019 },   // abs P:$10DFED..$10EFA0
    { 47398, 6002 },   // abs P:$10F7A1..$110F13
    { 54388, 715 },   // abs P:$1112EF..$1115BA
    { 56896, 1927 },   // abs P:$111CBB..$112442
    { 60873, 1483 },   // abs P:$112C44..$11320F
};
