//
// Created by ssm on 20. 11. 10..
//

#ifndef SE4_SDL2WINDOW_H
#define SE4_SDL2WINDOW_H

#include <mutex>
#include "SDL.h"
#include "graphics/RenderUpdater.h"

namespace se4 {
    class Sdl2Window {
        friend RenderUpdater;
    protected:
        Sdl2Window();

        std::once_flag isInit;
        SDL_Window *window = nullptr;
    };
}

#endif //SE4_SDL2WINDOW_H
