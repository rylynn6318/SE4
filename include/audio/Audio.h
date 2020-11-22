#include <iostream>
#include <vector>
#include <map>
#include <utility>

#include "../../extern/FMOD/inc/fmod.h"


#define SOUND_MAX 1.0f
#define SOUND_MIN 0.0f
#define SOUND_DEFAULT 0.5f
#define SOUND_WEIGHT 0.1f

namespace se4 {
	class Audio
	{
	public:
		Audio();
		~Audio();
		void addChannel(int channelNumber);
		void loadSound(const char* path, int soundId);
		void play(std::vector<std::pair<int, int>> soundIDs);
		void stop(int channelNum);
		void volumeUp(int channelNumber);
		void volumeDown(int channelNumber);
		void update();

	private:
		FMOD_SYSTEM* system;
		FMOD_BOOL m_bool;
		std::map<int, FMOD_CHANNEL*> channels;
		std::map<int, FMOD_SOUND*> sounds;
		float volume = 0.0;
	};
}