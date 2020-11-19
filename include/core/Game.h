//
// Created by ssm on 20. 10. 31..
//

#ifndef SE4_GAME_H
#define SE4_GAME_H

#include <memory>
#include "window/Window.h"
#include "GameConfig.h"
#include "level/Level.hpp"

namespace se4 {
    class Game {
    public:
        Game() = default;
        explicit Game(GameConfig const && config);

        auto run() -> void;

        // TODO : 일단 Pulbic으로 둠
    public:
        // TODO : 멤버 변수, 혹은 서비스로 InputManager, FileIO, AudioSystem, GameConfig 등 게임 전체에 하나만 존재하는 객체들 : 싱글톤 아님!
        std::function<bool()> isRunning;
        se4::Window* window;
        Level* level;

    private:
        auto loop() -> void;
    };
}

#endif //SE4_GAME_H
