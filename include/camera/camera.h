#include "component/Component.hpp"
#include "component/ComponentHandle.hpp"
#include "component/ComponentManager.hpp"
#include "entity/Entity.hpp"
#include "entity/EntityHandle.hpp"
#include "updater/Updater.hpp"
#include "world/World.hpp"
#include "updater/UpdaterFunction.h"
#include "component/Position2d.h"
#include <SDL_rect.h>

namespace se4 {
    class Camera : public se4::Updater
    {
        float aspectRatio; // 종횡비
        int width;
        int height;
        int x;
        int y;
        int screenWidth;
        int screenHeight;

    public:
        //SCREEN width, height를 필드의 width, height로 바꿔야 할 부분은 주석으로 V 써놓음
        Camera(float ratio, int screenWidth, int screenHeight) : aspectRatio(ratio), screenWidth(screenWidth), screenHeight(screenHeight)
        {
            se4::Updater::signature.addComponent<se4::Position2d>();
            this->screenWidth = screenWidth; //V
            this->screenHeight = screenHeight; //V
            x = screenWidth / 2; // 처음 화면 나올 때 화면의 중앙을 잡음 여기껀 SCREEN 맞음
            y = screenHeight / 2;
        }

        SDL_Rect cam(std::vector<se4::Entity> entities);
    };
}  // namespace se4