#pragma once
#pragma comment(lib, "fmod_vc.lib")
#include "Data.h"
#include <string>
#include "DataProvider.h"
#include "inc/fmod.hpp"
#define DataProvider__SoundData(sound_id) (*DataProvider<SoundData>::Instance().GetData(sound_id))
class SoundData : public Data
{
	friend class SoundHandler;
public:
	enum Type {
		MureungHill = 700001,
		CrimsonTower = 700002,
		MBtMouseClick = 700003,
		Attack = 700004,
		Jump = 700005,
		PickUpItem = 700006,
		QuestClear = 700007,
		BtMouseClick = 700008,
		Damaged_goblin = 700009,
		AboveTheTreetops = 700010,
	};
	SoundData();
	~SoundData();

	virtual bool SetData(const std::string& lhs_type_name, const std::string& rhs_type_value) override;
	virtual bool Release() override;
	int			GetId();

	static std::string GetInitializeFileName();
private:
	bool			sound_init;

	int				sound_id;
	std::string		sound_file_name;
	double			sound_default_volume;
	FMOD::Channel*	sound_fmod_channel;
	FMOD::Sound*	sound_fmod_instance;

	// #define FMOD_LOOP_OFF                               0x00000001
	// #define FMOD_LOOP_NORMAL                            0x00000002
	FMOD_MODE		sound_fmod_mode;

};

