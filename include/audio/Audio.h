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
		/*void addChannel(const char* channelName);*/
		void addChannelGroup(const char* groupName);
		void loadSound(const char* path, const char* soundID);
		/*void play(std::vector<std::pair<const char*, const char*>> soundIDs);*/
		void play(const char* soundID, const char* channelGroupName);
		void stop(const char* channelGroupName);
		/*void volumeUp(const char* channelGroupName);
		void volumeDown(const char* channelGroupName);*/
		void setVolume(const char* channelGroupName, float volume);
		float getVolume(const char* channelGroupName);
		void update();
		
	private:
		FMOD_SYSTEM* system;
		/*std::map<int, FMOD_CHANNEL*> channels;*/
		std::map<const char*, FMOD_CHANNELGROUP*> customChGroup;
		FMOD_CHANNELGROUP* masterGroup;
		
		std::map<const char*, FMOD_SOUND*> sounds;
	};
}