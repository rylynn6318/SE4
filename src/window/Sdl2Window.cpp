//
// Created by ssm on 20. 11. 10..
//

#include "window/Window.h"
#include "window/Sdl2Window.h"
#include "SDL.h"

namespace se4 {
    // 2020.11.11 non null terminated 문자열 넘겨도 잘 되는거 확임 함
    Window::Window(std::string_view title, int width, int height)
            : Sdl2Window(title.data(), width, height) {

    }

    Window::~Window() {
        SDL_DestroyWindow(window);
    }

    auto Window::pollKeyEvent(se4::Input& input) -> void {
        SDL_PumpEvents();
        Uint8 const *keystate = SDL_GetKeyboardState(nullptr);
        for (int keycode = 0; keycode < SDL_NUM_SCANCODES; ++keycode) {
            input.saveKeymap(keycode, keystate[keycode]);
        }
    }

    auto Window::tmp_getWindow() -> SDL_Window * {
        return window;
    }
}

namespace se4 {
    Sdl2Window::Sdl2Window(char const *title, int width, int height) {
        SDL_Init(SDL_INIT_EVERYTHING);
        window = SDL_CreateWindow(
                        title,
                        SDL_WINDOWPOS_UNDEFINED,
                        SDL_WINDOWPOS_UNDEFINED,
                        width,
                        height,
                        SDL_WINDOW_SHOWN
                );
    }
}