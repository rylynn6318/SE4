//
// Created by ssm on 20. 10. 31..
//

#include <thread>
#include "core/Game.h"

using namespace std::chrono_literals;
namespace sc = std::chrono;

auto se4::Game::run() -> void {
    for (auto [id, win] : windows) {
        for (auto&[id, level] : levelManager.levelList) {
            level->createRenderContext(win);
            level->init();
        }
    }
    loop();
}

auto se4::Game::loop() -> void {
    auto const MS_PER_UPDATE = 16ms;
    auto previous = sc::system_clock::now();
    while (isRunning()) {
        auto start = sc::system_clock::now();

        inputManager.pollKeyEvent();

        for (auto key : levelManager.activatedLevelId()){
            levelManager.levelList.at(key)->update(0);
        }

        for (auto& [level_id, level] : levelManager.levelList) {
            for (auto [window_id, window] : windows){
                if(window->renderingLevelId() == level_id){
                    level->render(window_id, 0);
                }
            }
        }

        // 일단은 남는 시간동안 sleep 때림
        std::this_thread::sleep_for(start + MS_PER_UPDATE - sc::system_clock::now());
    }
}

auto se4::Game::addWindow(Window *window) -> void {
    windows[window->id] = window;
}
