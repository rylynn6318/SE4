//
// Created by ssm on 20. 10. 31..
//

#include "core/Game.h"



auto se4::Game::addWorlds(std::unique_ptr<World> world_ptr) -> void {
    worlds.push_back(std::move(world_ptr));
}

auto se4::Game::run() -> void {

}

auto se4::Game::stop() -> void {

}