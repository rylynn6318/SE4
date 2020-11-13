//
// Created by ssm on 20. 11. 12..
//

#ifndef SE4_RENDERER_H
#define SE4_RENDERER_H

#include "EngineConfig.h"
#include "updater/Updater.hpp"
#include "window/Window.h"
#include SDK_RENDERER_H

namespace se4 {
    class Renderer : public se4::Updater, SDK_RENDERER {
    public:
        Renderer();

        auto init() -> bool override;

        auto render(int time) -> void;

    public:
        // 일단 pulbic으로 둠
        std::unique_ptr<Window> window;
    };
}

#endif //SE4_RENDERER_H
