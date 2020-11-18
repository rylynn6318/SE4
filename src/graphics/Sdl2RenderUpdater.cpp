//
// Created by ssm on 20. 11. 12..
//

#include <component/Volume2d.h>
#include <graphics/RenderComponent.h>
#include <component/Position2d.h>
#include <SDL_image.h>
#include <component/ComponentHandle.hpp>
#include "graphics/Sdl2RenderUpdater.h"
#include "graphics/RenderUpdater.h"
#include "world/World.hpp"

auto se4::RenderUpdater::init() -> bool {
    for (auto &entity : registeredEntities) {
        se4::ComponentHandle<RenderComponent> renderHandler;
        parentWorld->unpack(entity, renderHandler);

        renderHandler->texture = IMG_LoadTexture(mainRenderer, renderHandler->texture_path);
    }

    return true;
}

auto se4::RenderUpdater::render(int time) -> void {
    SDL_SetRenderDrawColor(mainRenderer, 0, 0, 0, 255);
    SDL_RenderClear(mainRenderer);

    for (auto &entity : registeredEntities) {
        se4::ComponentHandle<Position2d> pos2dHandler;
        se4::ComponentHandle<Volume2d> vol2dHandler;
        se4::ComponentHandle<RenderComponent> renderHandler;
        parentWorld->unpack(entity, pos2dHandler, vol2dHandler, renderHandler);

        SDL_Rect rect;
        rect.w = vol2dHandler->width;
        rect.h = vol2dHandler->height;
        rect.x = pos2dHandler->x - (rect.w / 2);
        rect.y = pos2dHandler->y - (rect.h / 2);

        SDL_RenderCopy(mainRenderer, renderHandler->texture, nullptr, &rect);
    }
    SDL_RenderPresent(mainRenderer);
}

se4::RenderUpdater::RenderUpdater(std::any &context) {
    signature.addComponent<Position2d>();
    signature.addComponent<Volume2d>();
    signature.addComponent<RenderComponent>();
    windowContext = context;
    mainRenderer = std::any_cast<SDL_Renderer*>(context);
}