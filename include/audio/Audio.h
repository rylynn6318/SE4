#include <iostream>
#include <vector>
#include <map>
#include <utility>
#include <fmod.h>

#define SOUND_MAX 1.0f
#define SOUND_MIN 0.0f
#define SOUND_DEFAULT 0.5f
#define SOUND_WEIGHT 0.1f

namespace se4 {
	class Audio
	{
	public:
		Audio(int channelCount);
		~Audio();
		void loadSound(const char* path, int soundId);
		void play(std::vector<std::pair<int, int>> soundIDs);
		void stop(int channelNum);

	private:
		FMOD_SYSTEM* system;
		std::vector<FMOD_CHANNEL*> channels;
		std::map<int, FMOD_SOUND*> sounds;
	};
}