//
// Created by ssm on 20. 11. 10..
//

#include <cassert>
#include "window/Window.h"
#include "window/Sdl2Window.h"
#include "SDL.h"

namespace se4 {
    // 2020.11.11 non null terminated 문자열 넘겨도 잘 되는거 확임 함
    Window::Window(std::string_view title, int width, int height)
            : Sdl2Window(), title(title), width(width), height(height) {
    }

    Window::~Window() {
        SDL_DestroyWindow(window);
    }

    auto Window::pollKeyEvent(se4::Input &i) -> void {
        SDL_PumpEvents();
        Uint8 const *keystate = SDL_GetKeyboardState(nullptr);
        for (int keycode = 0; keycode < SDL_NUM_SCANCODES; ++keycode) {
            i.saveKeymap(keycode, keystate[keycode]);
        }
    }

    auto Window::getHandle() -> std::any& {
        ++burrowed_count;
        assert(burrowed_count == 1);
        return context;
    }

    auto Window::returnHandle(std::any &handle) -> void {
//        assert(&handle == &context);
//        std::move(handle);
//        --burrowed_count;
//        assert(burrowed_count == 0);
    }

    auto Window::show() -> void {
        window = SDL_CreateWindow(
                title.data(),
                SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED,
                width,
                height,
                SDL_WINDOW_SHOWN
        );
        context = SDL_CreateRenderer(window, -1, 0);;
    }

    auto Window::pollKeyEvent() -> void {
        pollKeyEvent(input);
    }

    Sdl2Window::Sdl2Window() {
        std::call_once(isInit, []() {
            SDL_Init(SDL_INIT_EVERYTHING);
            std::atexit(SDL_Quit);
        });
    }
}