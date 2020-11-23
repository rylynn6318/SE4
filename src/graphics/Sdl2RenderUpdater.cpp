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

const int fieldWidth = 1920;
const int fieldHeight = 1080;
float winWidth = 1920;
float winHeight = 1080;

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

	SDL_Rect camera = getCamViewprot();

    for (auto &entity : registeredEntities) {
        se4::ComponentHandle<Position2d> pos2dHandler;
        se4::ComponentHandle<Volume2d> vol2dHandler;
        se4::ComponentHandle<RenderComponent> renderHandler;
        parentWorld->unpack(entity, pos2dHandler, vol2dHandler, renderHandler);

		SDL_Rect destRect;
		destRect.w = vol2dHandler->width;
		destRect.h = vol2dHandler->height;

		destRect.x = pos2dHandler->x - camera.x - (destRect.w/2);
		destRect.y = pos2dHandler->y - camera.y - (destRect.h/2);

        SDL_RenderCopyEx(mainRenderer, renderHandler->texture, nullptr, &destRect, renderHandler->angle, 0, renderHandler->flip);
    }
	SDL_RenderSetScale(mainRenderer, winWidth / camera.w, winHeight / camera.h);

    SDL_RenderPresent(mainRenderer);
}

se4::RenderUpdater::RenderUpdater(std::any &context) {
    signature.addComponent<Position2d>();
    signature.addComponent<Volume2d>();
    signature.addComponent<RenderComponent>();
    windowContext = context;
    mainRenderer = std::any_cast<SDL_Renderer*>(context);
}

auto se4::RenderUpdater::getCamViewprot() -> SDL_Rect {

    //모든 entity의 좌표는 0 ~ 필드의 크기 사이

    if (registeredEntities.empty())
    {
        return { 0, 0, fieldWidth, fieldHeight };
    }

    int left = fieldWidth;
    int right = 0;
    int top = fieldHeight;
    int bottom = 0;

    for (auto& entity : registeredEntities)
    {
        ComponentHandle<Position2d> posHandler;
        ComponentHandle<Volume2d> volHandler;
        ComponentHandle<RenderComponent> renderHandler;
        parentWorld->unpack(entity, posHandler, volHandler, renderHandler);

        if (renderHandler->isFocused)
        {
            if (posHandler->x + volHandler->width / 2 > right)
            {
                right = posHandler->x + volHandler->width / 2;
            }
            if (posHandler->x - volHandler->width / 2 < left)
            {
                left = posHandler->x - volHandler->width / 2;
            }

            if (posHandler->y + volHandler->height / 2 > bottom)
            {
                bottom = posHandler->y + volHandler->height / 2;
            }
            if (posHandler->y - volHandler->height / 2 < top)
            {
                top = posHandler->y - volHandler->height / 2;
            }
        }

    }

    left -= padding;
    if (left < 0)
    {
        left = 0;
    }
    right += padding;
    if (right > fieldWidth)
    {
        right = fieldWidth;
    }
    top -= padding;
    if (top < 0)
    {
        top = 0;
    }
    bottom += padding;
    if (bottom > fieldHeight)
    {
        bottom = fieldHeight;
    }

    //최소크기는 fieldMinWidth와의 비교로 간단하게 가능함.. 지금은 안짬

    // 둘 중에 큰 놈을 적용
    float ratio = (float)winWidth / winHeight;

    int width1 = right - left;
    int width2 = (bottom - top) * ratio;

    int width = (width1 > width2 ? width1 : width2);
    int height = width / ratio;

    int x = (left + right) / 2.0f - width / 2.0f;
    int y = (top + bottom) / 2.0f - height / 2.0f;

    return SDL_Rect{x, y, width, height};
}