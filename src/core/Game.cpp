//
// Created by ssm on 20. 10. 31..
//

#include <thread>
#include "core/Game.h"

using namespace std::chrono_literals;
namespace sc = std::chrono;

auto se4::Game::run() -> void {
    level->init();
    loop();
}

auto se4::Game::loop() const -> void {
    auto const MS_PER_UPDATE = 16ms;
    auto previous = sc::system_clock::now();
    while (isRunning()) {
        auto start = sc::system_clock::now();

        window->pollKeyEvent();

        level->update(0);

        level->render(0);

        // 일단은 남는 시간동안 sleep 때림
        std::this_thread::sleep_for(start + MS_PER_UPDATE - sc::system_clock::now());
    }
}
