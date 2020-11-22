//
// Created by ssm on 20. 11. 12..
//

#ifndef SE4_SDL2RENDERUPDATER_H
#define SE4_SDL2RENDERUPDATER_H

#include <SDL_render.h>
#include <vector>

namespace se4 {
    class Sdl2RenderUpdater {
    public:
        virtual ~Sdl2RenderUpdater() = default;

    protected:
        std::vector<SDL_Renderer *> renderers;
    };
}

#endif //SE4_SDL2RENDERUPDATER_H
