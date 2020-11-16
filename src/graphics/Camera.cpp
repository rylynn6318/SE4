#include "graphics/Camera.h"
#include <iostream>
#include <algorithm>

namespace se4 {
	SDL_Rect Camera::cam()
	{
		////일단 0번째 entity를 기준으로 포지션을 잡음
		//ComponentHandle<Position2d> positionHandler;
		//ComponentHandle<Cam> camHandler;
		//ComponentHandle<Volume2d> volumeHandler;

		//std::vector<subject> entities;
		//for (auto& entity : registeredEntities)
		//{
		//	parentWorld->unpack(entity, positionHandler, volumeHandler, camHandler);
		//	//에러 나면 아래 코드 사용
		//	entities.push_back(std::make_tuple(Position2d(positionHandler->x, positionHandler->y), Volume2d(volumeHandler->width, volumeHandler->height)));
		//	//entities.push_back(subject(Position2d(positionHandler->x, positionHandler->y), Volume2d(volumeHandler->width, volumeHandler->height)));
		//}

		//std::vector<subject>::iterator result;
		//
		//result = std::max_element(entities.begin(), entities.end(), [](subject const& l, subject const& r) {
		//	return std::get<0>(l).x < std::get<0>(l).x;
		//});
		//
		//int maxX = get<0>(entities.at(std::distance(entities.begin(), result))).x;
		
		//더이상 entity가 없으면 그대로 리턴
		/*if (entities.size() == 1)
		{
			SDL_Rect result;
			result.x = x;
			result.y = y;
			result.w = screenWidth;
			result.h = screenHeight;
			return result;
		}*/
		//아닐 경우 첫 entity의 값을 기초값으로 잡고 registeredEntities를 끝까지 돌면서 minX, maxX, minY, maxY를 구함
		int minX = levelWidth + 1;
		int maxX = -1;
		int minY = levelHeight + 1;
		int maxY = -1;

		int xMinVol;
		int xMaxVol;
		int yMinVol;
		int yMaxVol;
		
		for (auto& entity : registeredEntities)
		{
			ComponentHandle<Position2d> posHandler;
			ComponentHandle<Volume2d> volHandler;
			ComponentHandle<Cam> camHandler;
			parentWorld->unpack(entity, posHandler, volHandler, camHandler);

			if (posHandler->x > maxX)
			{
				maxX = posHandler->x;
				xMaxVol = volHandler->width / 2;
			}
			else if (posHandler->x > minX)
			{
				minX = posHandler->x;
				xMinVol = volHandler->width / 2;
			}

			if (posHandler->y > maxY)
			{
				maxY = posHandler->y;
				yMaxVol = volHandler->height / 2;
			}
			else if (posHandler->y > minY)
			{
				minY = posHandler->y;
				yMinVol = volHandler->height / 2;
			}
		}
		//xy의 좌표값과 width, height 도출
		x = (minX + maxX) / 2;
		y = (minY + maxY) / 2;
		width = maxX - minX + xMaxVol + xMinVol;
		height = maxY - minY + yMaxVol + yMinVol;

		//종횡비에 따라 width, height 보정
		if (width * aspectRatio > height)
			height = width * aspectRatio;
		else if (height / aspectRatio > width)
			width = height / aspectRatio;

		//
		if (x + width > levelWidth)
		{
			x = levelWidth - width;
		}

		if (y + height > levelHeight)
		{
			y = levelHeight - height;
		}

		SDL_Rect result;
		result.x = x;
		result.y = y;
		result.w = width;
		result.h = height;

		return result;
	}
}