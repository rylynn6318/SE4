//
// Created by ssm on 20. 11. 12..
//

#include <component/Volume2d.h>
#include <graphics/RenderComponent.h>
#include <component/Position2d.h>
#include <SDL_image.h>
#include <component/ComponentHandle.hpp>
#include "graphics/Sdl2Renderer.h"
#include "graphics/Renderer.h"
#include "world/World.hpp"
#include <iostream>

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
		destRect.w = vol2dHandler->width;
		destRect.h = vol2dHandler->height;
		
		destRect.x = pos2dHandler->x -camera.x;
		destRect.y = pos2dHandler->y -camera.y;

		//1920*1080
        SDL_RenderCopyEx(mainRenderer, renderHandler->texture, nullptr, &destRect, renderHandler->angle, 0, renderHandler->flip);
    }
	SDL_RenderSetScale(mainRenderer, (double)window->winWidth / camera.w, (double)window->winHeight / camera.h);

    SDL_RenderPresent(mainRenderer);
}

auto se4::Renderer::getCamViewprot() -> SDL_Rect {
	const int fieldWidth = 1920;
	const int fieldHeight = 1080;

	//모든 entity의 좌표는 0 ~ 필드의 크기 사이
	int minX = fieldWidth + 1;
	int maxX = -1;
	int minY = fieldHeight + 1;
	int maxY = -1;

	int maxXVolume = 0;
	int maxYVolume = 0;

	for (auto& entity : registeredEntities)
	{
		ComponentHandle<Position2d> posHandler;
		ComponentHandle<Volume2d> volHandler;
		se4::ComponentHandle<RenderComponent> renderHandler;
		parentWorld->unpack(entity, posHandler, volHandler, renderHandler);
		
		if (renderHandler->isFocused)
		{
			if (posHandler->x > maxX)
			{
				maxX = posHandler->x;
				maxXVolume = volHandler->width;
			}
			if (posHandler->x < minX)
			{
				minX = posHandler->x;
			}

			if (posHandler->y > maxY)
			{
				maxY = posHandler->y;
				maxYVolume = volHandler->height;
			}
			if (posHandler->y < minY)
			{
				minY = posHandler->y;
			}
		}
		
	}

	double aspectRatio = window->winHeight / (double)window->winWidth;
	if (minX == maxX && minY == maxY)
	{
		int winMinHeight = (int)(winMinWidth * aspectRatio);
		SDL_Rect view{ maxX - (winMinWidth - maxXVolume) / 2 - winMinWidth*padding, 
			maxY - (winMinHeight - maxYVolume) / 2 - winMinHeight * padding, 
			winMinWidth + winMinWidth * 2 * padding, 
			winMinHeight + winMinHeight * 2 * padding };
		
		if (view.x + view.w > fieldWidth)
			view.x = fieldWidth - view.w;

		if (view.y + view.h > fieldHeight)
			view.y = fieldHeight - view.h;

		return view;
	}
	
	//xy의 좌표값과 width, height 도출
	int width = maxX - minX + maxXVolume;
	int height = maxY - minY + maxYVolume;

	//최소크기 검사
	if (width < winMinWidth)
		width = winMinWidth;
	
	//종횡비에 따라 width, height 보정
	if (width * aspectRatio > height)
		height = (int)(width * aspectRatio);
	else if (height / aspectRatio > width)
		width = (int)(height / aspectRatio);
	
	int x = minX - width * padding;
	int y = minY - height * padding;

	width *= (1 + padding * 2);
	height *= (1 + padding * 2);

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

se4::Renderer::Renderer() {
    signature.addComponent<Position2d>();
    signature.addComponent<Volume2d>();
    signature.addComponent<RenderComponent>();
}
