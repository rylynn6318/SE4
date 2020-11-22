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

		auto loadLevel(int key, std::unique_ptr<se4::Level> level)->void;
		auto getCurrentLevel()->se4::Level*;
		auto setCurrnetLevel(int key, std::unique_ptr<se4::Window> &window)->void;
		
		auto addLevel(int key, std::function<std::unique_ptr<se4::Level>(std::unique_ptr<se4::Window>&)> func)->void;
		auto getSharedData(std::string)->std::any;
		auto setSharedData(std::string key, std::any data)->void;
		auto getLevelNum()->int { return currentLevel; }
		auto setLevelNum(int key)->void;

	
		std::map<int, std::unique_ptr<se4::Level>>levelList;
		std::map<std::string, std::any> sharedData;
		std::map<int, std::function<std::unique_ptr<se4::Level>(std::unique_ptr<se4::Window>&)>> funcList;
		int currentLevel = 0;
	};
}	