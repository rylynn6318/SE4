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
        WindowID const id;
        int width;
        int height;
        Window(WindowID id, std::string_view title, int width, int height);

        ~Window();

        Input input;

        auto setRenderLevel(LevelID) -> void;
        [[nodiscard]]
        auto renderingLevelId() const -> LevelID;

    private:
        LevelID level_id = 0;
        std::string_view title;
    };
}

#endif //SE4_WINDOW_H