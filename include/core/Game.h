//
// Created by ssm on 20. 10. 31..
//

#ifndef SE4_GAME_H
#define SE4_GAME_H

#include <memory>
#include "window/Window.h"
#include "GameConfig.h"
#include "world/World.hpp"

namespace se4 {
    class Game {
    public:
        template<typename... Worlds>
        explicit Game(GameConfig const && config, std::shared_ptr<World> world_ptr, std::shared_ptr<Worlds>... worlds_ptr);

        auto run() -> void;
        auto stop() -> void;

    private:
        // TODO : 멤버 변수, 혹은 서비스로 InputManager, FileIO, AudioSystem, GameConfig 등 게임 전체에 하나만 존재하는 객체들 : 싱글톤 아님!
        bool isRunning;
        Window window;
        std::vector<std::shared_ptr<World>> worlds;
        
        auto addWorlds(std::shared_ptr<World> world) -> void;

        template<typename... Worlds>
        auto addWorlds(std::shared_ptr<World> world, std::shared_ptr<Worlds>... worlds_ptr) -> void;

    };

    template<typename... Worlds>
    Game::Game(GameConfig const && config, std::shared_ptr<World> world_ptr, std::shared_ptr<Worlds>... worlds_ptr) {
        this->addWorlds(world_ptr, worlds_ptr...);
    }

    template<typename... Worlds>
    auto Game::addWorlds(std::shared_ptr<World> world_ptr, std::shared_ptr<Worlds>... worlds_ptr) -> void {
        worlds.push_back(std::move(world_ptr));
        this->addWorlds(worlds_ptr ...);
    }
}

#endif //SE4_GAME_H
