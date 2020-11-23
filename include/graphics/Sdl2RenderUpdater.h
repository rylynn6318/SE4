//
// Created by ssm on 20. 11. 12..
//

#ifndef SE4_SDL2RENDERUPDATER_H
#define SE4_SDL2RENDERUPDATER_H

#include "EngineConfig.h"

#include <map>
#include "SDL_render.h"

namespace se4 {
    class Sdl2RenderUpdater {
    protected:
        std::map<WindowID, SDL_Renderer *> renderers;
        std::map<std::pair<WindowID, TextureID>, SDL_Texture *> textures;
    };
}

#endif //SE4_SDL2RENDERUPDATER_H
