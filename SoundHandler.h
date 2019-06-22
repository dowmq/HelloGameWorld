#pragma once
#pragma comment(lib, "fmod_vc.lib")
#include "TSingleton.h"
#include "inc/fmod.hpp"
#include "SoundData.h"
class SoundHandler : public TSingleton<SoundHandler>
{
	friend class TSingleton<SoundHandler>;
	SoundHandler();
public:
	~SoundHandler();

	bool	Init();
	bool	Frame();
	bool	Release();

	bool	SoundInitialize(SoundData& sound_data);

	bool	Play(int sound_id);
	bool	Play(SoundData& sound_data);

	bool	SetMode(int sound_id, FMOD_MODE mode);
	bool	SetMode(SoundData& sound_data, FMOD_MODE mode);

	bool	Stop(int sound_id);
	bool	Stop(SoundData& sound_data);

	bool	Paused(int sound_id);
	bool	Paused(SoundData& sound_data);
private:
	FMOD::System*		fmod_sound_system;
};