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
        Window(std::string_view title, int width, int height);
        ~Window();

        auto show() -> void;

        auto pollKeyEvent(se4::Input& input) -> void;
        auto getHandle() -> std::any&;
        auto returnHandle(std::any& handle) -> void;
    private:
        int burrowed_count = 0;
        std::any context;
        std::string_view title;
        int width;
        int height;
    };
}

#endif //SE4_WINDOW_H
