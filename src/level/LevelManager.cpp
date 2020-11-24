#include <level/LevelManager.hpp>
#include <utility>
#include <algorithm>
#include "core/Game.h"

namespace se4 {
    auto se4::LevelManager::loadLevel(LevelID key) -> void {
        deleteLevel(key);
        levelList[key] = std::move(funcList.at(key)());
        for ( auto [id, window] : se4::Game::Instance().windows) {
            levelList[key]->createRenderContext(window);
        }
        levelList[key]->init();
    }

    auto LevelManager::addLevel(LevelID key, std::function<std::unique_ptr<se4::Level>()> &&func) -> void {
        //중복 검사 관련 코드 추가해야함
        if (funcList.find(key) == funcList.end()) {
            funcList[key] = func;
        }
    }

    auto LevelManager::activateLevel(LevelID key) -> void {
        if (std::ranges::find(activatedId, key) == activatedId.end())
            activatedId.push_back(key);
    }

    auto LevelManager::deactivateLevel(LevelID key) -> void {        
        auto iter = std::ranges::find(activatedId, key);
        if (iter != activatedId.end()) {
            activatedId.erase(iter);
        }
    }

    auto LevelManager::activatedLevelId() const -> std::vector<LevelID> const & {
        return activatedId;
    }

    auto LevelManager::deleteLevel(LevelID key) -> void {
        if (levelList.find(key) != levelList.end()) {
            levelList.erase(key);
        }        
    }
}