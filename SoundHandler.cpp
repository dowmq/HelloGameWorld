#include "SoundHandler.h"
#include "StringData.h"


SoundHandler::SoundHandler()
{
}


SoundHandler::~SoundHandler()
{
}

bool SoundHandler::Init()
{
	FMOD::System_Create(&fmod_sound_system);
	fmod_sound_system->init(32, FMOD_INIT_NORMAL, 0);
	return true;
}

bool SoundHandler::Frame()
{
	if (fmod_sound_system)
		fmod_sound_system->update();
	return true;
}

bool SoundHandler::Release()
{
	if (fmod_sound_system) {
		fmod_sound_system->close();
		fmod_sound_system->release();
	}
	return true;
}

bool SoundHandler::SoundInitialize(SoundData& sound_data)
{
	char file_path[256] = { 0, };
	sprintf_s(file_path, 256, DataProvider__StringData(48).c_str(), sound_data.sound_file_name.c_str());
	FMOD_RESULT ret = fmod_sound_system->createSound(
		file_path, sound_data.sound_fmod_mode, 0, &sound_data.sound_fmod_instance);
	if (ret != FMOD_OK) {
		return false;
	}
	sound_data.sound_init = true;
	return true;
}

bool SoundHandler::Play(int sound_id) {
	return Play(DataProvider__SoundData(sound_id));
}

bool SoundHandler::Play(SoundData& sound_data)
{
	if (sound_data.sound_init == false) {
		SoundInitialize(sound_data);
	}
	fmod_sound_system->playSound(sound_data.sound_fmod_instance, 0, false, &sound_data.sound_fmod_channel);
	sound_data.sound_fmod_channel->setVolume(sound_data.sound_default_volume);
	SetMode(sound_data, sound_data.sound_fmod_mode);
	return true;
}

bool SoundHandler::SetMode(int sound_id, FMOD_MODE mode) {
	return SetMode(DataProvider__SoundData(sound_id), mode);
	return true;
}

bool SoundHandler::SetMode(SoundData& sound_data, FMOD_MODE mode) {
	if (sound_data.sound_fmod_channel) {
		sound_data.sound_fmod_channel->setMode(mode);
	}
	return true;
}

bool SoundHandler::Stop(int sound_id) {
	return Stop(DataProvider__SoundData(sound_id));
}

bool SoundHandler::Stop(SoundData& sound_data)
{
	if (sound_data.sound_fmod_channel) {
		sound_data.sound_fmod_channel->stop();
	}
	return true;
}

bool SoundHandler::Paused(int sound_id) {
	return Paused(DataProvider__SoundData(sound_id));
}

bool SoundHandler::Paused(SoundData& sound_data)
{
	if (sound_data.sound_fmod_channel) {
		bool paused;
		sound_data.sound_fmod_channel->getPaused(&paused);
		sound_data.sound_fmod_channel->setPaused(&paused);
	}
	return true;
}