//
// Created by ssm on 20. 11. 12..
//

#ifndef SE4_SDL2RENDERUPDATER_H
#define SE4_SDL2RENDERUPDATER_H

#include <SDL_render.h>
#include <vector>

namespace se4 {
    class Sdl2RenderUpdater {
    protected:
        //std::vector<SDL_Renderer *> renderers;
        SDL_Renderer *mainRenderer = nullptr;
    };
}

#endif //SE4_SDL2RENDERUPDATER_H
