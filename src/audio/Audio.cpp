#include "audio/Audio.h"

namespace se4 {
	Audio::Audio() {
		FMOD_System_Create(&system);
		FMOD_System_Init(system, 32, FMOD_INIT_NORMAL, nullptr);
		FMOD_System_GetMasterChannelGroup(system, &masterGroup);
		
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
	}

	void Audio::loadSound(const char* path, int soundID) {
		if (sounds.find(soundID) != sounds.end())
			return;
		FMOD_SOUND* newSound;
		FMOD_System_CreateSound(system, path, FMOD_DEFAULT, 0, &newSound);
		sounds[soundID] = newSound;

	}
	//앞은 soundID, 뒤는 channel넘버
	void Audio::play(std::vector<std::pair<int, int>> soundIDs) {
		for(std::pair<int, int> soundID : soundIDs)
				FMOD_System_PlaySound(system, sounds[soundID.first], nullptr, false, &channels[soundID.second]);

		FMOD_Channel_SetVolume(channels[1], 0.5);
	}

	void Audio::stop(int channelNum) {
		FMOD_Channel_Stop(channels[channelNum]);
	}

	void Audio::volumeUp(int channelNumber) {
		if (volume < SOUND_MAX) {
			volume += SOUND_WEIGHT;
		}

		FMOD_Channel_SetVolume(channels[channelNumber], volume);
	}

	void Audio::volumeDown(int channelNumber) {
		if (volume < SOUND_MAX) {
			volume -= SOUND_WEIGHT;
		}

		FMOD_Channel_SetVolume(channels[channelNumber], volume);
	}

	void Audio::update() {
		FMOD_BOOL m_bool;
		for (std::pair<int, FMOD_CHANNEL*> channel : channels)
		{
			FMOD_Channel_IsPlaying(channels[channel.first], &m_bool);
			if (!m_bool)
				return;
		}

		FMOD_System_Update(system);
	}
}