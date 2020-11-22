#include <level/LevelManager.hpp>

namespace se4 {
	auto se4::LevelManager::loadLevel(int key, std::unique_ptr<se4::Level> level)->void {
		auto iter = levelList.find(key);
		if (iter == levelList.end())
			levelList.insert(std::pair<int, std::unique_ptr<Level>>(key, std::move(level)));
	}

	auto se4::LevelManager::getCurrentLevel()->Level* {
		auto iter = levelList.find(currentLevel);		
		if (iter != levelList.end()) {
			return levelList.at(currentLevel).get();
		}
		else {
			return nullptr;
		}
	}

	auto LevelManager::addLevel(int key, std::function<std::unique_ptr<se4::Level>(std::unique_ptr<se4::Window>&)> func) -> void
	{
		//중복 검사 관련 코드 추가해야함
		if (funcList.find(key) == funcList.end()) {
			funcList.at(key) = func;
		}
	}

	auto se4::LevelManager::getSharedData(std::string key)->std::any {
		if (sharedData.count(key) == 0)
			return sharedData.at(key);
		else
			return NULL;
	}

	auto se4::LevelManager::setSharedData(std::string key, std::any data)->void {
		sharedData[key] = data;
	}

	auto se4::LevelManager::setLevelNum(int level)->void {
		currentLevel = level;
	}

}