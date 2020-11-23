#include <iostream>
#include <vector>
#include <map>
#include <utility>

#include "updater/Updater.hpp"
#include "component/Sound.h"
#include "component/ComponentHandle.hpp"
#include "level/Level.hpp"

#include "../../extern/FMOD/inc/fmod.h"


#define SOUND_MAX 1.0f
#define SOUND_MIN 0.0f
#define SOUND_DEFAULT 0.5f
#define SOUND_WEIGHT 0.1f

namespace se4 {
	class Audio : public se4::Updater
	{
	public:
		Audio();
		~Audio();
		/*void addChannel(const char* channelName);*/
		void addChannelGroup(const char* groupName);
		void loadSound(const char* path, const char* soundID);
		/*void play(std::vector<std::pair<const char*, const char*>> soundIDs);*/
		void play(const char* soundID, const char* groupName, bool isLoop);
		void stop(const char* groupName);
		/*void volumeUp(const char* channelGroupName);
		void volumeDown(const char* channelGroupName);*/
		void setMasterVolume(float volume);
		float getMasterVolume();
		void setVolume(const char* groupName, float volume);
		float getVolume(const char* groupName);
		void update(int dt);
		
	private:
		FMOD_SYSTEM* system;
		/*std::map<int, FMOD_CHANNEL*> channels;*/
		std::map<const char*, FMOD_CHANNELGROUP*> customChGroup;
		FMOD_CHANNELGROUP* masterGroup;
		
		std::map<const char*, FMOD_SOUND*> sounds;
	};
}
