//
// Created by lunaoeu on 11/10/20.
//

#ifndef SE4_WINDOW_H
#define SE4_WINDOW_H

#include <string_view>
#include "input/Input.h"
#include "EngineConfig.h"
#include SDK_WINDOW_H

namespace se4 {
    class Window : public SDK_WINDOW {
    public:
        int width;
        int height;
        Window(std::string_view title, int width, int height);

        ~Window();

        Input input;

        auto show() -> void;

        auto setRenderLevel(LevelID) -> void;
        [[nodiscard]]
        auto renderingLevelId() const -> LevelID;
        auto initLevelRender() -> void;
    private:
        LevelID level_id;
        std::string_view title;

    };
}

#endif //SE4_WINDOW_H