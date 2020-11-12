//
// Created by lunaoeu on 11/10/20.
//

#ifndef SE4_WINDOW_H
#define SE4_WINDOW_H

#include <string_view>
#include <input/Key.h>
#include <input/Input.h>
#include "EngineConfig.h"
#include SDK_WINDOW_H

namespace se4 {
class Window : public SDK_WINDOW {
    public:
        Window(std::string_view title, int width, int height);
        ~Window();

        auto pollKeyEvent(se4::Input& input) -> void;
        auto tmp_getWindow() -> SDL_Window*;
    private:
    };
}

#endif //SE4_WINDOW_H
