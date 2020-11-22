#include "audio/Audio.h"

namespace se4 {
	Audio::Audio(int channelCount) {
		FMOD_System_Create(&system);
		FMOD_System_Init(system, 32, FMOD_INIT_NORMAL, nullptr);
		for (int i = 0; i < channelCount; i++)
			channels.push_back(nullptr);
	}
	
	Audio::~Audio() {
		FMOD_System_Close(system);
		FMOD_System_Release(system);
	}

	void Audio::loadSound(const char* path, int soundID) {
		if (sounds.find(soundID) != sounds.end())
			return;
		//FMOD_SOUND* newSound;
			FMOD_System_CreateSound(system, path, FMOD_DEFAULT, 0, &sounds[soundID]);
	}
	//앞은 soundID, 뒤는 channel넘버
	void Audio::play(std::vector<std::pair<int, int>> soundIDs) {
		for(std::pair<int, int> soundID : soundIDs)
				FMOD_System_PlaySound(system, sounds[soundID.first], nullptr, false, &channels.at(soundID.second));
	}

	void Audio::stop(int channelNum) {
		FMOD_Channel_Stop(channels.at(channelNum));
	}
}