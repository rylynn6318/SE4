#include "camera/camera.h"
#include <iostream>

namespace se4 {
    SDL_Rect Camera::cam(std::vector<se4::Entity> entities)
	{
        //일단 0번째 entity를 기준으로 포지션을 잡음
        se4::ComponentHandle<se4::Position2d> defaultLocationHandler;
        parentWorld->unpack(entities.at(0), defaultLocationHandler);
        x = defaultLocationHandler->x;
        y = defaultLocationHandler->y;

        //더이상 entity가 없으면 그대로 리턴
        if (entities.size() == 1)
        {
            SDL_Rect result;
            result.x = x;
            result.y = y;
            result.w = screenWidth;
            result.h = screenHeight;
            return result;
        }
        //아닐 경우 첫 entity의 값을 기초값으로 잡고 registeredEntities를 끝까지 돌면서 minX, maxX, minY, maxY를 구함
        int minX = x;
        int maxX = x;
        int minY = y;
        int maxY = y;

        for (auto& entity : entities)
        {
            se4::ComponentHandle < se4::Position2d > posHandler;
            parentWorld->unpack(entity, posHandler);

            if (posHandler->x > maxX)
            {
                maxX = posHandler->x;
            }
            else if (posHandler->x > minX)
            {
                minX = posHandler->x;
            }

            if (posHandler->y > maxY)
            {
                maxY = posHandler->y;
            }
            else if (posHandler->y > minY)
            {
                minY = posHandler->y;
            }
        }
        //xy의 좌표값과 width, height 도출
        x = (minX + maxX) / 2;
        y = (minY + maxY) / 2;
        width = maxX - minX;
        height = maxY - minY;

        //종횡비에 따라 width, height 보정
        if (width * aspectRatio > height)
            height = width * aspectRatio;
        else if (height / aspectRatio > width)
            width = height / aspectRatio;

        //
        if (x + width > screenWidth)
        {
            x = screenWidth - width;
        }

        if (y + height > screenHeight)
        {
            y = screenHeight - height;
        }

        SDL_Rect result;
        result.x = x;
        result.y = y;
        result.w = width;
        result.h = height;

        return result;
	}
}