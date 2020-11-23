#include "audio/Audio.h"

namespace se4 {
	Audio::Audio() {
		FMOD_System_Create(&system);
		FMOD_System_Init(system, 32, FMOD_INIT_NORMAL, nullptr);
		FMOD_System_GetMasterChannelGroup(system, &masterGroup);
		FMOD_ChannelGroup_SetVolume(masterGroup, SOUND_DEFAULT);
	}
	
	Audio::~Audio() {
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
		FMOD_SOUND* newSound;
		FMOD_System_CreateSound(system, path, FMOD_DEFAULT, 0, &newSound);
		sounds[soundID] = newSound;

	}
	//앞은 soundID, 뒤는 channel넘버
	//void Audio::play(std::vector<std::pair<const char*, const char*>> soundIDs) {
	//	for(std::pair<const char*, const char*> soundID : soundIDs)
	//			FMOD_System_PlaySound(system, sounds[soundID.first], customChGroup[soundID.second], false, nullptr);
	//}

	void Audio::play(const char* soundID, const char* channelName) {
		FMOD_System_PlaySound(system, sounds[soundID], customChGroup[channelName], false, nullptr);
	}

	void Audio::stop(const char* channelGroupName) {
		FMOD_ChannelGroup_Stop(customChGroup[channelGroupName]);
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

	void Audio::setVolume(const char* channelGroupName, float volume) {
		if(SOUND_MIN <= volume && volume <= SOUND_MAX)
			FMOD_ChannelGroup_SetVolume(customChGroup[channelGroupName], volume);
	}

	float Audio::getVolume(const char* channelGroupName) {
		float volume;
		FMOD_ChannelGroup_GetVolume(customChGroup[channelGroupName], &volume);
		
		return volume;
	}

	void Audio::update() {
		FMOD_BOOL m_bool;
		for (std::pair<const char*, FMOD_CHANNELGROUP*> channelGroup : customChGroup)
		{
			FMOD_ChannelGroup_IsPlaying(customChGroup[channelGroup.first], &m_bool);
			if (!m_bool)
				return;
		}

		FMOD_System_Update(system);
	}
}