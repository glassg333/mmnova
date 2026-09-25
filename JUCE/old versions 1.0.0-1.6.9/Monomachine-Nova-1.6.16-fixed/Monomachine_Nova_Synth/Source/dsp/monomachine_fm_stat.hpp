// Nova 1.6.5: FM+ STATIC (machine 8) выделен в собственный файл.
//
// ПОЛЬЗОВАТЕЛЬСКОЕ РЕШЕНИЕ 1.6.5: «fm stat в old можешь удалить» и
// «раздели все машины… чтобы я мог удалять отдельно версию fm stat,
// а не вмешиваться сразу в 3 FM машины».
//
// Поэтому прежний класс MonomachineFmStatic (old-движок) УДАЛЁН из сборки
// (старый совмещённый файл оставлен рядом как monomachine_fm_stat_par.hpp.bak-v7).
// Машина FM+ STAT (id 8) при этом НЕ удалена: в режиме old она теперь молча
// использует основной mnm-кор (monomachine::mnm::FmCore, FmKind::Stat) — тот же,
// что и в режиме mnm. Резервного old-варианта у FM+ STAT больше нет, о чём
// честно написано в меню DSP MODE (раздел SYNT).
//
// FM+ PAR (id 9) лежит в monomachine_fm_par.hpp, FM+ DYN (id 10) — в
// monomachine_fm_dynamic.hpp: три FM-машины больше не делят один файл.
#pragma once

namespace monomachine {
// Намеренно пусто: old-движок FM+ STATIC удалён в 1.6.5 по просьбе пользователя.
// Машина 8 работает через mnm-ядро (см. NovaDSP.h, MachineEngine::render).
} // namespace monomachine
