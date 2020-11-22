//
// Created by lunaoeu on 11/10/20.
//

#ifndef SE4_WINDOW_H
#define SE4_WINDOW_H

#include <string_view>
#include <any>
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

        auto pollKeyEvent(Input &i) -> void;
        auto pollKeyEvent() -> void;

        auto setRenderLevel(int) -> void;
    private:
        int level_id;
        std::any context;

        std::string_view title;
    };
}

#endif //SE4_WINDOW_H