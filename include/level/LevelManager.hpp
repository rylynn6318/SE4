#pragma once

#include "Level.hpp"

#include <map>
#include <string>
#include <any>
#include <vector>
#include "EngineConfig.h"


namespace se4 {
	class LevelManager {
	public:
		LevelManager() = default;

		virtual ~LevelManager() = default;

		auto loadLevel(LevelID key)->bool;

		auto getLevel(LevelID key) const->se4::Level*;

		auto addLevel(LevelID key, std::function<std::unique_ptr<se4::Level>()>&& func) -> bool;

		auto getSharedData(SharedDataID key)const->std::any;
		auto setSharedData(SharedDataID key, std::any data)->void;

		[[nodiscard]]
		auto getCurrentLevelID() const -> int { return currentLevel; }

		auto setCurrentLevelID(LevelID key) -> void;


		std::map<LevelID, std::unique_ptr<se4::Level>> levelList;
		std::map<SharedDataID, std::any> sharedData;
		std::map<LevelID, std::function<std::unique_ptr<se4::Level>()>> funcList;
		LevelID currentLevel = 0;
	};
}