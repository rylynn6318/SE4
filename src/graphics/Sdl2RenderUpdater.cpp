//
// Created by ssm on 20. 11. 12..
//

#include <component/Volume2d.h>
#include <graphics/RenderComponent.h>
#include <component/Position2d.h>
#include <SDL.h>
#include <SDL_image.h>
#include <component/ComponentHandle.hpp>
#include "graphics/RenderUpdater.h"
#include "level/Level.hpp"
#include "core/Game.h"

const int fieldWidth = 1920;
const int fieldHeight = 1080;
float winWidth = 1920;
float winHeight = 1080;

std::map<WindowID, SDL_Renderer *> se4::Sdl2RenderUpdater::renderers = {};

se4::RenderUpdater::RenderUpdater() {
    signature.addComponent<Position2d>();
    signature.addComponent<Volume2d>();
    signature.addComponent<RenderComponent>();
}

se4::RenderUpdater::~RenderUpdater() {
    for (auto[id, texture] : textures) {
        if (texture) SDL_DestroyTexture(texture);
    }
//    for (auto[id, renderer] : renderers) {
//        if (renderer) SDL_DestroyRenderer(renderer);
//    }
}

auto se4::RenderUpdater::init() -> bool {
    for (auto &entity : registeredEntities) {
        se4::ComponentHandle<RenderComponent> renderHandler;
        parentWorld->unpack(entity, renderHandler);

        for (auto[id, renderer] : renderers) {
            auto key = std::make_pair(id, renderHandler->id);
            if (textures.find(key) == textures.end())
                // textures[id] = SDL_CreateTextureFromSurface(renderer, renderHandler->surface);
                textures[key] = IMG_LoadTexture(renderer, renderHandler->texture_path);
        }
    }
    return true;
}

auto se4::RenderUpdater::render(WindowID window_id, int time) -> void {
    SDL_SetRenderDrawColor(renderers.at(window_id), 0, 0, 0, 255);
    SDL_RenderClear(renderers.at(window_id));

    SDL_Rect camera = getCamViewprot();

    for (auto &entity : registeredEntities) {
        se4::ComponentHandle<Position2d> pos2dHandler;
        se4::ComponentHandle<Volume2d> vol2dHandler;
        se4::ComponentHandle<RenderComponent> renderHandler;
        parentWorld->unpack(entity, pos2dHandler, vol2dHandler, renderHandler);

        SDL_Rect destRect;
        destRect.w = vol2dHandler->width;
        destRect.h = vol2dHandler->height;

        destRect.x = pos2dHandler->x - camera.x - (destRect.w / 2);
        destRect.y = pos2dHandler->y - camera.y - (destRect.h / 2);

        SDL_RenderCopyEx(renderers.at(window_id), textures[{window_id, renderHandler->id}], nullptr, &destRect,
                         renderHandler->angle, 0, renderHandler->flip);
    }
    SDL_RenderSetScale(renderers.at(window_id), winWidth / camera.w, winHeight / camera.h);

    SDL_RenderPresent(renderers.at(window_id));
}

auto se4::RenderUpdater::getCamViewprot() -> SDL_Rect {

    //모든 entity의 좌표는 0 ~ 필드의 크기 사이
    int minX = fieldWidth + 1;
    int maxX = -1;
    int minY = fieldHeight + 1;
    int maxY = -1;

    int maxXVolume = 0;
    int maxYVolume = 0;
    int minXVolume = 0;
    int minYVolume = 0;

    for (auto &entity : registeredEntities) {
        ComponentHandle<Position2d> posHandler;
        ComponentHandle<Volume2d> volHandler;
        ComponentHandle<RenderComponent> renderHandler;
        parentWorld->unpack(entity, posHandler, volHandler, renderHandler);


        if (renderHandler->isFocused)
        {
            if (posHandler->x > maxX)
            {
                maxX = posHandler->x;
                maxXVolume = volHandler->width / 2;
            }
            if (posHandler->x < minX)
            {
                minX = posHandler->x;
                minXVolume = volHandler->width / 2;
            }

            if (posHandler->y > maxY)
            {
                maxY = posHandler->y;
                maxYVolume = volHandler->height / 2;
            }
            if (posHandler->y < minY)
            {
                minY = posHandler->y;
                minYVolume = volHandler->height / 2;
            }
        }
    }

    double aspectRatio = winHeight / winWidth;
    if (minX == maxX && minY == maxY)
    {
        int winMinHeight = (int)(winMinWidth * aspectRatio);

        SDL_Rect view{ maxX - (winMinWidth) / 2 ,
                       maxY - (winMinHeight) / 2 ,
                       winMinWidth,
                       winMinHeight };

        if (view.x + view.w > fieldWidth)
            view.x = fieldWidth - view.w;
        if (view.x < 0)
            view.x = 0;

        if (view.y + view.h > fieldHeight)
            view.y = fieldHeight - view.h;
        if (view.y < 0)
            view.y = 0;

        return view;
    }

    //xy의 좌표값과 width, height 도출
    int width = maxX - minX + maxXVolume + minXVolume;
    int height = maxY - minY + maxYVolume + minYVolume;

    //최소크기 검사
    if (width < winMinWidth)
        width = winMinWidth;


    int x = (minX + maxX) / 2 - width / 2 - width * padding;
    if (width * aspectRatio > height)
        height = (int)(width * aspectRatio);
    else if (height / aspectRatio > width)
        width = (int)(height / aspectRatio);
    int y = (minY + maxY) / 2 - height / 2 - height * padding;

    width *= (1 + padding * 2);
    if (width > fieldWidth) {
        width = fieldWidth;
        x = 0;
        y = 0;
    }
    height = (int)(width * aspectRatio);

    if (x + width > fieldWidth)
        x = fieldWidth - width;
    if (x < 0)
        x = 0;

    if (y + height > fieldHeight)
        y = fieldHeight - height;
    if (y < 0)
        y = 0;

    return SDL_Rect{x, y, width, height};
}

auto se4::RenderUpdater::createRenderContext(se4::Window *window) -> void {
    if (renderers.find(window->id) == renderers.end()){
        auto renderer = SDL_CreateRenderer(window->window, -1, 0);
        renderers[window->id] = renderer;
    }
}
