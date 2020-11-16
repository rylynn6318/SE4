#include "component/Component.hpp"
#include "component/ComponentHandle.hpp"
#include "component/ComponentManager.hpp"
#include "entity/Entity.hpp"
#include "entity/EntityHandle.hpp"
#include "updater/Updater.hpp"
#include "world/World.hpp"
#include "updater/UpdaterFunction.h"
#include "component/Position2d.h"
#include "component/Volume2d.h"
#include "component/Cam.h"
#include <SDL_rect.h>
#include <tuple>

#ifndef SE4_CAMERA_H
#define SE4_CAMERA_H

namespace se4 {
	class Camera : public se4::Updater
	{
		typedef std::tuple<Position2d, Volume2d> subject;
		float aspectRatio; // 종횡비
		int width;
		int height;
		int x;
		int y;
		int levelWidth;
		int levelHeight;

	public:
		Camera(float ratio, int levelWidth, int levelHeight, int width, int height) : aspectRatio(ratio), levelWidth(levelWidth), levelHeight(levelHeight), width(width), height(height)
		{
			//se4::Updater::signature.addComponent<se4::Position2d>();
			x = levelWidth / 2 - width;
			y = levelHeight / 2 - height;
		}

		SDL_Rect cam();
		void findRect();
	};
}  // namespace se4

#endif //SE4_CAMERA_H