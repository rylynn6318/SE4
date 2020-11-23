#include "audio/Audio.h"

namespace se4 {
	Audio::Audio() {
		signature.addComponent<Sound>();

		FMOD_System_Create(&system);
		FMOD_System_Init(system, 32, FMOD_INIT_NORMAL, nullptr);
		FMOD_System_GetMasterChannelGroup(system, &masterGroup);
		FMOD_ChannelGroup_SetVolume(masterGroup, SOUND_DEFAULT);
	}
	
	Audio::~Audio() {
		for (auto it = sounds.begin(); it != sounds.end(); it++)
		{
			FMOD_Sound_Release(it->second);
		}
		for (auto it = customChGroup.begin(); it != customChGroup.end(); it++)
		{
			FMOD_ChannelGroup_Release(it->second);
		}
		FMOD_System_Close(system);
		FMOD_System_Release(system);
	}

	//void Audio::addChannel(const char* channelName) {
	//	channels[channelName] = nullptr;
	//}

	void Audio::addChannelGroup(const char* groupName) {
		FMOD_System_CreateChannelGroup(system, groupName, &customChGroup[groupName]);
		FMOD_ChannelGroup_SetVolume(customChGroup[groupName], SOUND_DEFAULT);
	}

	void Audio::loadSound(const char* path, const char* soundID) {
		if (sounds.find(soundID) != sounds.end())
			return;
		FMOD_System_CreateSound(system, path, FMOD_DEFAULT, 0, &sounds[soundID]);

	}
	//앞은 soundID, 뒤는 channel넘버
	//void Audio::play(std::vector<std::pair<const char*, const char*>> soundIDs) {
	//	for(std::pair<const char*, const char*> soundID : soundIDs)
	//			FMOD_System_PlaySound(system, sounds[soundID.first], customChGroup[soundID.second], false, nullptr);
	//}

	void Audio::play(const char* soundID, const char* groupName, bool isLoop) {
		FMOD_CHANNEL* ignore;

		if (isLoop)
		{
			FMOD_Sound_SetMode(sounds[soundID], FMOD_LOOP_NORMAL);
		}
		FMOD_System_PlaySound(system, sounds[soundID], customChGroup[groupName], false, &ignore);
		FMOD_Sound_SetMode(sounds[soundID], FMOD_LOOP_OFF);
	}

	void Audio::stop(const char* groupName) {
		FMOD_ChannelGroup_Stop(customChGroup[groupName]);
	}

	/*void Audio::volumeUp(const char* channelGroupName) {
		float volume;
		FMOD_ChannelGroup_GetVolume(customChGroup[channelGroupName], &volume);

		if (volume < SOUND_MAX) {
			volume += SOUND_WEIGHT;
		}

		FMOD_ChannelGroup_SetVolume(customChGroup[channelGroupName], volume);
	}

	void Audio::volumeDown(const char* channelGroupName) {
		float volume;
		FMOD_ChannelGroup_GetVolume(customChGroup[channelGroupName], &volume);

		if (volume < SOUND_MAX) {
			volume -= SOUND_WEIGHT;
		}

		FMOD_ChannelGroup_SetVolume(customChGroup[channelGroupName], volume);
	}*/

	void Audio::setMasterVolume(float volume) {
		if (SOUND_MIN <= volume && volume <= SOUND_MAX)
			FMOD_ChannelGroup_SetVolume(masterGroup, volume);
	}

	float Audio::getMasterVolume() {
		float volume;
		FMOD_ChannelGroup_GetVolume(masterGroup, &volume);
		return volume;
	}

	void Audio::setVolume(const char* groupName, float volume) {
		if(SOUND_MIN <= volume && volume <= SOUND_MAX)
			FMOD_ChannelGroup_SetVolume(customChGroup[groupName], volume);
	}

	float Audio::getVolume(const char* groupName) {
		float volume;
		FMOD_ChannelGroup_GetVolume(customChGroup[groupName], &volume);
		return volume;
	}

	void Audio::update(int dt) {
		FMOD_System_Update(system);

		for (auto& entity : registeredEntities)
		{
			ComponentHandle<Sound> sound;
			parentWorld->unpack(entity, sound);
			
			while (!sound->ids.empty())
			{
				const auto& front = sound->ids.front(); sound->ids.pop();
				play(front.soundId, front.groupId, front.isLoop);
			}
		}
	}
}