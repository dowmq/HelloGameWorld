#include "SoundData.h"
#include "StringData.h"


SoundData::SoundData()
{
	sound_fmod_channel = nullptr;
	sound_init = false;
	sound_fmod_instance = nullptr;
}


SoundData::~SoundData()
{
	if (sound_fmod_instance) {
		sound_fmod_instance->release();
		sound_fmod_instance = nullptr;
	}
}

bool SoundData::Release()
{
	if (sound_fmod_instance) {
		sound_fmod_instance->release();
		sound_fmod_instance = nullptr;
	}
	return true;
}

int SoundData::GetId() { return sound_id; }

bool SoundData::SetData(const std::string& lhs_type_name, const std::string& rhs_type_value)
{
	if (lhs_type_name == DataProvider__StringData(49)) {
		sound_id = std::atoi(rhs_type_value.c_str());
	}
	else if (lhs_type_name == DataProvider__StringData(50)) {
		sound_file_name = rhs_type_value;
	}
	else if (lhs_type_name == DataProvider__StringData(51)) {
		sound_default_volume = std::atof(rhs_type_value.c_str());
	}
	else if (lhs_type_name == DataProvider__StringData(52)) {
		sound_fmod_mode = (FMOD_MODE)std::atoi(rhs_type_value.c_str());
	}
	return true;
}

std::string SoundData::GetInitializeFileName() {
	return DataProvider__StringData(5);
}