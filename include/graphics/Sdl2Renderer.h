//
// Created by ssm on 20. 11. 12..
//

#ifndef SE4_SDL2RENDERER_H
#define SE4_SDL2RENDERER_H

#include <SDL_render.h>

namespace se4 {
    class Sdl2Renderer {
    protected:
        SDL_Renderer *mainRenderer = nullptr;
    };
}

#endif //SE4_SDL2RENDERER_H
