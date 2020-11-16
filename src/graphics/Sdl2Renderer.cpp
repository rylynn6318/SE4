//
// Created by ssm on 20. 11. 12..
//

#include <component/Volume2d.h>
#include <graphics/RenderComponent.h>
#include <component/Position2d.h>
#include <component/Cam.h>
#include <SDL_image.h>
#include <component/ComponentHandle.hpp>
#include "graphics/Sdl2Renderer.h"
#include "graphics/Renderer.h"
#include "world/World.hpp"

auto se4::Renderer::init() -> bool {
    mainRenderer = SDL_CreateRenderer(window->tmp_getWindow(), -1, 0);

    for (auto &entity : registeredEntities) {
        se4::ComponentHandle<RenderComponent> renderHandler;
        parentWorld->unpack(entity, renderHandler);

        renderHandler->texture = IMG_LoadTexture(mainRenderer, renderHandler->texture_path);
    }

    return true;
}

auto se4::Renderer::render(int time) -> void {
    SDL_SetRenderDrawColor(mainRenderer, 0, 0, 0, 255);
    SDL_RenderClear(mainRenderer);
    
	SDL_Rect camera = getCamViewprot();

    for (auto &entity : registeredEntities) {
        se4::ComponentHandle<Position2d> pos2dHandler;
        se4::ComponentHandle<Volume2d> vol2dHandler;
        se4::ComponentHandle<RenderComponent> renderHandler;
        parentWorld->unpack(entity, pos2dHandler, vol2dHandler, renderHandler);

		SDL_Rect destRect;
        destRect.w = vol2dHandler->width*2;
        destRect.h = vol2dHandler->height*2;
		destRect.x = pos2dHandler->x - (destRect.w / 2) - camera.x + camera.w/4;
		destRect.y = pos2dHandler->y - (destRect.h / 2) - camera.y + camera.h/4;

        SDL_RenderCopyEx(mainRenderer, renderHandler->texture, nullptr, &destRect, 0, 0, SDL_FLIP_NONE);
    }

	//SDL_RenderSetViewport(mainRenderer, &viewport);

	SDL_RenderSetScale(mainRenderer, window->winWidth / camera.w, window->winHeight / camera.h);

    SDL_RenderPresent(mainRenderer);
}

auto se4::Renderer::getCamViewprot() -> SDL_Rect {
	
	int minX = 1920 + 1;
	int maxX = -1;
	int minY = 1080 + 1;
	int maxY = -1;

	int minXVolume = 0;
	int maxXVolume = 0;
	int minYVolume = 0;
	int maxYVolume = 0;

	for (auto& entity : registeredEntities)
	{
		ComponentHandle<Position2d> posHandler;
		ComponentHandle<Volume2d> volHandler;
		se4::ComponentHandle<RenderComponent> renderHandler;
		parentWorld->unpack(entity, posHandler, volHandler, renderHandler);
		
		if (renderHandler->shouldBeSeen)
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

	double aspectRatio = window->winHeight / (double)window->winWidth;

	if (minX == maxX && minY == maxY)
	{
		SDL_Rect view{ minX - winMinWidth / 2, minY - (int)(winMinWidth * aspectRatio) / 2, winMinWidth, (int)(winMinWidth * aspectRatio) };
		if (view.x + view.w > 1920)
			view.x = 1920 - view.w;
		else if (view.x < 0)
			view.x = 0;

		if (view.y + view.h > 1080)
			view.y = 1080 - view.h;
		else if (view.y < 0)
			view.y = 0;

		return view;
	}

	//xy의 좌표값과 width, height 도출
	
	
	int width = maxX - minX + (minXVolume + maxXVolume) / 2;
	int height = maxY - minY + (minYVolume + maxYVolume) / 2;
	
	//종횡비에 따라 width, height 보정
	if (width * aspectRatio > height)
		height = (int)(width * aspectRatio);
	else if (height / aspectRatio > width)
		width = (int)(height / aspectRatio);

	int x = (minX + maxX) / 2 - width / 2;
	int y = (minY + maxY) / 2 - height / 2;

	//
	if (x + width > 1920)
		x = 1920 - width;
	else if (x < 0)
		x = 0;

	if (y + height > 1080)
		y = 1080 - height;
	else if (y < 0)
		y = 0;

	return SDL_Rect{ x, y, width, height };
}

se4::Renderer::Renderer() {
    signature.addComponent<Position2d>();
    signature.addComponent<Volume2d>();
    signature.addComponent<RenderComponent>();
}
