
#pragma once
#include "ObxdVoice.h"
class VoiceQueue
{
private:
	ObxdVoice* voices;
	int idx,total;
public:
	VoiceQueue()
	{
		voices  = NULL;
		idx = 0;
		total = 0;
	}
	VoiceQueue(int voiceCount,ObxdVoice* voicesReference)
	{
		voices = voicesReference;
		idx = 0;total = voiceCount;
	}
	inline ObxdVoice* getNext()
	{
		idx = idx + 1;
		idx %=total;
		return &voices[idx];
	}
	inline void reInit(int voiceCount)
	{
		total = voiceCount;
		idx = idx%total;
	}
};
