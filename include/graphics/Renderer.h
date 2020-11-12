//
// Created by ssm on 20. 11. 12..
//

#ifndef SE4_RENDERER_H
#define SE4_RENDERER_H

#include "EngineConfig.h"
#include <world/World.hpp>
#include "window/Window.h"
#include SDK_RENDERER_H

namespace se4 {
    class Renderer : public SDK_RENDERER, private se4::Updater {
    public:
        Renderer(se4::World world);

        auto init() -> bool;

        auto render(double time) -> void;

    private:
        std::unique_ptr<World> world;
    };
}

#endif //SE4_RENDERER_H
