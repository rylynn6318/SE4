//
// Created by ssm on 20. 11. 13..
//

#ifndef SE4_RENDERCOMPONENT_H
#define SE4_RENDERCOMPONENT_H

#include "component/Component.hpp"
#include <string>
#include <utility>

// TODO : 이거 나중에 빼야함!! 임시로 넣음!!
#include <SDL.h>

namespace se4 {
    struct RenderComponent : public Component<RenderComponent> {
        explicit RenderComponent(char const * path) : texture_path(path) 
        {
            isFocused = false;
        }
        explicit RenderComponent(char const* path, bool shouldBeSeen) : texture_path(path), isFocused(shouldBeSeen) {}

        char const * texture_path;
        // 나중에 빼야함
        SDL_Texture * texture;
        bool isFocused;
        double angle = 0;
        SDL_RendererFlip flip = SDL_FLIP_NONE;
    };
}

#endif //SE4_RENDERCOMPONENT_H
