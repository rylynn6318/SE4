#include "Level.hpp"

#include <map>
#include <string>
#include <any>
#include <vector>

namespace se4 {	
	class LevelManager {
	public:
		LevelManager() = default;
		virtual ~LevelManager() = default;

		auto loadLevel(int key)->void;
		auto getLevel(int key)->se4::Level*;
		
		auto addLevel(int key, std::function<std::unique_ptr<se4::Level>()> func)->void;
		//auto getSharedData(std::string)->std::any;
		//auto setSharedData(std::string key, std::any data)->void;
		auto getCurrentLevelID()->int { return currentLevel; }
		auto setCUrrentLevelID(int key)->void;

	
		std::map<int, std::unique_ptr<se4::Level>>levelList;
		//std::map<std::string, std::any> sharedData;
		std::map<int, std::function<std::unique_ptr<se4::Level>()>> funcList;
		int currentLevel = 0;
	};
}	