//
// Created by ssm on 20. 11. 12..
//

#ifndef SE4_RENDERUPDATER_H
#define SE4_RENDERUPDATER_H

#include "EngineConfig.h"
#include "updater/Updater.hpp"
#include "window/Window.h"
#include SDK_RENDERER_H

namespace se4 {
    class RenderUpdater : public se4::Updater, SDK_RENDERER {
    public:
        explicit RenderUpdater(std::any& context);

        auto init() -> bool override;

        auto render(int time) -> void;

    public:
        // 일단 pulbic으로 둠
        std::any windowContext;
    };
}

#endif //SE4_RENDERUPDATER_H
