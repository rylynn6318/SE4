//
// Created by ssm on 20. 11. 13..
//

#ifndef SE4_RENDERCOMPONENT_H
#define SE4_RENDERCOMPONENT_H

#include <map>

#include "component/Component.hpp"
#include <string>
#include <utility>

// TODO : 이거 나중에 빼야함!! 임시로 넣음!!
#include <SDL.h>

namespace se4 {
    struct RenderComponent : public Component<RenderComponent> {
        explicit RenderComponent(char const *path, bool isFocused = false) : texture_path(path), isFocused(isFocused) {}
        ~RenderComponent();

        char const *texture_path;
        bool isFocused;
        double angle = 0;

        // 나중에 빼야함
        std::map<SDL_Renderer *, SDL_Texture *> textures;
        SDL_RendererFlip flip = SDL_FLIP_NONE;
    };
}

#endif //SE4_RENDERCOMPONENT_H
