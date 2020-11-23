#include <level/LevelManager.hpp>
#include <utility>
#include "core/Game.h"
#include <optional>
#include <cassert>

namespace se4 {
    auto se4::LevelManager::loadLevel(LevelID key) -> bool {
        if (levelList.find(key) == levelList.end()) {
            levelList[key] = std::move(funcList.at(key)());
            levelList[key]->init();
            return true;
        }
        else {
            return false;
        }
    }

    auto se4::LevelManager::getLevel(LevelID key) const -> Level* {
        auto iter = levelList.find(key);
        if (iter != levelList.end()) {
            return levelList.at(key).get();
        }
        else {
            return nullptr;
        }
    }

    auto LevelManager::addLevel(LevelID key, std::function<std::unique_ptr<se4::Level>()>&& func) -> bool {
        //중복 검사 관련 코드 추가해야함
        if (funcList.find(key) == funcList.end()) {
            funcList[key] = func;
            return true;
        }
        else {
            assert(!"aleady key in funcList");
            return false;
        }
    }

    auto se4::LevelManager::getSharedData(SharedDataID key) const ->std::any {
        std::optional<std::any>;
        if (sharedData.find(key) != sharedData.end())
            return sharedData.at(key);
        else
            return std::nullopt;
    }

    auto se4::LevelManager::setSharedData(SharedDataID key, std::any data)->void {
        sharedData[key] = data;
    }

    auto se4::LevelManager::setCurrentLevelID(LevelID key) -> void {
        currentLevel = key;
        for (auto& win : Game::Instance().windowList) {
            if (currentLevel == win->renderingLevelId()) {
                win->initLevelRender();
            }
        }
    }

}