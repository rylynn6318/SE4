//
// Created by ssm on 20. 10. 31..
//

#include "core/Game.h"



auto se4::Game::addWorlds(std::shared_ptr<World>&& world_ptr) -> void {
    world = std::move(world_ptr);
}

auto se4::Game::run() -> void {

}

auto se4::Game::stop() -> void {

}

template<typename... Worlds>
se4::Game::Game(const se4::GameConfig &&config) {

}
