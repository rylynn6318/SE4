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

		auto getCurrentLevel()->std::unique_ptr<Level>;
		auto addLevel(int key, std::unique_ptr<Level> level)->void;
		auto getSharedData(std::string)->std::any;
		auto setSharedData(std::string key, std::any data)->void;
		auto getLevelNum()->int { return currentLevel; }
		auto changeLevel(int key)->void;

	private:
		std::map<int, std::unique_ptr<Level>> levelList;					
		std::map<std::string, std::any> sharedData;
		int currentLevel;
	};
}	