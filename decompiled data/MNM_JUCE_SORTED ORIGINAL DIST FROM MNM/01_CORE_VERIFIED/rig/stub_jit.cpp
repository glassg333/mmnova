// Link-time stubs for the interpreter-only harness build (JIT & audio disabled).
#include "dsp56kEmu/audio.h"

namespace dsp56k
{
	Audio::Audio(bool) {}
	void Audio::terminate() {}
	void Audio::readRXimpl(RxFrame&) {}
	void Audio::writeTXimpl(const TxFrame&) {}
}
