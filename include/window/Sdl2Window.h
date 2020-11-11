//
// Created by ssm on 20. 11. 10..
//

#ifndef SE4_SDL2WINDOW_H
#define SE4_SDL2WINDOW_H

#include <memory>
#include "SDL.h"

namespace se4 {
    class Sdl2Window {
    protected:
        SDL_Window* window;

        Sdl2Window(char const * title, int width, int height) ;
    };
}

#endif //SE4_SDL2WINDOW_H
