#include <level/LevelManager.hpp>
#include <utility>

namespace se4 {
	auto se4::LevelManager::loadLevel(int key)->void {
	    levelList[key] = std::move(funcList.at(key)());
	}

	auto se4::LevelManager::getLevel(int key)->Level* {
		auto iter = levelList.find(key);
		if (iter != levelList.end()) {
			return levelList.at(key).get();
		}
		else {
			return nullptr;
		}
	}

	auto LevelManager::addLevel(int key, std::function<std::unique_ptr<se4::Level>()>&& func) -> void
	{
		//중복 검사 관련 코드 추가해야함
		if (funcList.find(key) == funcList.end()) {
			funcList[key] = func;
		}
	}

	//auto se4::LevelManager::getSharedData(std::string key)->std::any {
	//	if (sharedData.count(key) == 0)
	//		return sharedData.at(key);
	//	else
	//		return NULL;
	//}

	//auto se4::LevelManager::setSharedData(std::string key, std::any data)->void {
	//	sharedData[key] = data;
	//}

	auto se4::LevelManager::setCurrentLevelID(int level)->void {
		currentLevel = level;
	}

}