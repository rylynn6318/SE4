//
// Created by ssm on 20. 11. 13..
//

#include "graphics/RenderComponent.h"
#include "SDL_image.h"

se4::RenderComponent::~RenderComponent() {
    if(surface)
        SDL_FreeSurface(surface);
}

se4::RenderComponent::RenderComponent(const char *path, bool isFocused)
        : texture_path(path), isFocused(isFocused) {
    surface = IMG_Load(path);
    id = std::hash<std::string>{}(path);
}
