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

        auto getCamViewprot() -> SDL_Rect;

    public:
        // 일단 pulbic으로 둠
        std::unique_ptr<Window> window;
        int winMinWidth = 800;
        //SDL_Rect viewport{ 1, 0, 600, 400 };
    };
}

#endif //SE4_RENDERER_H
