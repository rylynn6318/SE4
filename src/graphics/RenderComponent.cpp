//
// Created by ssm on 20. 11. 13..
//

#include "graphics/RenderComponent.h"

se4::RenderComponent::~RenderComponent() {
    for(auto [renderer, texture] : textures){
        SDL_DestroyTexture(texture);
    }
}
