//
// Created by ssm on 20. 11. 10..
//

#ifndef SE4_SDL2WINDOW_H
#define SE4_SDL2WINDOW_H

#include <mutex>
#include "SDL.h"

namespace se4 {
    class Sdl2Window {
    protected:
        Sdl2Window();

        std::once_flag isInit;
        SDL_Window *window = nullptr;
        SDL_Renderer *renderer = nullptr;
    };
}

#endif //SE4_SDL2WINDOW_H
