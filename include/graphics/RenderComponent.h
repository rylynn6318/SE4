//
// Created by ssm on 20. 11. 13..
//

#ifndef SE4_RENDERCOMPONENT_H
#define SE4_RENDERCOMPONENT_H

#include "EngineConfig.h"
#include <unordered_map>
#include <string>
#include "component/Component.hpp"
// TODO : 이거 나중에 빼야함!! 임시로 넣음!!
#include <SDL.h>

namespace se4 {
    struct RenderComponent : public Component<RenderComponent> {
        explicit RenderComponent(char const *path, bool isFocused = false);
        ~RenderComponent();

        char const *texture_path;
        bool isFocused;
        double angle = 0;

        TextureID id;
        // 나중에 빼야함
        SDL_Surface* surface;
        SDL_RendererFlip flip = SDL_FLIP_NONE;
    };
    // TextureID RenderComponent::counter = 0;
}

#endif //SE4_RENDERCOMPONENT_H
