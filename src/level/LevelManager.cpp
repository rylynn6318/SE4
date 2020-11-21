#include <level/LevelManager.hpp>

namespace se4 {
	auto se4::LevelManager::getCurrentLevel()->std::unique_ptr<Level> {
		auto level = levelList.find(currentLevel);
		if (level != levelList.end()) {
			return std::move(levelList.at(currentLevel));
		}
	}

	auto se4::LevelManager::addLevel(int key, std::unique_ptr<Level> level)->void {
		if (levelList.count(key) == 0)
			levelList.insert(std::pair(key, std::move(level)));			
	}

	auto se4::LevelManager::getSharedData(std::string key)->std::any {
		if (sharedData.count(key) == 0)
			return sharedData.at(key);
	}

	auto se4::LevelManager::setSharedData(std::string key, std::any data)->void {
		sharedData[key] = data;
	}

	auto se4::LevelManager::changeLevel(int level)->void {
		currentLevel = level;
	}
}