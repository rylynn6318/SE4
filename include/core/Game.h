//
// Created by ssm on 20. 10. 31..
//

#ifndef SE4_GAME_H
#define SE4_GAME_H

#include <map>

#include "EngineConfig.h"

#include "window/Window.h"
#include "level/LevelManager.hpp"
#include "FileManager.hpp"
#include "SharedData.hpp"

namespace se4 {
    class Game {
    public:
        static Game &Instance() {
            static Game instance;
            return instance;
        }

        Game(const Game &) = delete;

        Game(Game &&) = delete;

        Game &operator=(const Game &) = delete;

        Game &operator=(Game &&) = delete;

        auto run() -> void;

        auto addWindow(Window*) -> void;

        // TODO : 일단 Pulbic으로 둠
    public:
        // TODO : 멤버 변수, 혹은 서비스로 InputManager, FileIO, AudioSystem, GameConfig 등 게임 전체에 하나만 존재하는 객체들 : 싱글톤 아님!
        std::function<bool()> isRunning;
        LevelManager levelManager;
        Input inputManager;
        std::map<WindowID, Window *> windows;
        std::vector<Window*> windowList;
        FileManager fileManager;
        SharedData sharedData;

    private:
        Game() = default;

        ~Game() = default;

        auto loop() -> void;
    };
}

#endif //SE4_GAME_H
