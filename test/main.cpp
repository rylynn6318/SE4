#include <utility>
#include <functional>
#include <cstring>
#include <chrono>
#include <thread>

#include "SDL.h"
#include "SDL_image.h"
#include "glog/logging.h"

#include "se4.hpp"
#include "updater/UpdaterFunction.h"
#include "input/InputComponent.h"
#include "component/Position2d.h"
#include "component/Volume2d.h"
#include "graphics/RenderComponent.h"

#include "input/Input.h"
#include "window/Window.h"

#include "box2d/box2d.h"

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 800;

using namespace std::chrono_literals;
namespace sc = std::chrono;

struct XAxisAcceleration : public se4::Component<XAxisAcceleration> {
    explicit XAxisAcceleration(float x) : acceleration(x) {}

    float acceleration;
};

struct Yeji : public se4::Component<Yeji> {
};

using InputHandle = se4::ComponentHandle<se4::InputComponent>;
using XAxisAccelerationHandle = se4::ComponentHandle<XAxisAcceleration>;
using Position3fHandle = se4::ComponentHandle<se4::Position2d>;
using YejiHandle = se4::ComponentHandle<Yeji>;

se4::Input input;

auto inputCallback(int dt, InputHandle inputHandler, XAxisAccelerationHandle accelerationHandler) {
    if (inputHandler->is_selected) {
        if (input.checkKey(se4::KeyState::PRESSED, se4::Key::A) ||
            input.checkKey(se4::KeyState::HELD_DOWN, se4::Key::A)) {
            accelerationHandler->acceleration -= 0.1;
        }
        if (input.checkKey(se4::KeyState::PRESSED, se4::Key::D) ||
            input.checkKey(se4::KeyState::HELD_DOWN, se4::Key::D)) {
            accelerationHandler->acceleration += 0.1;
        }
    }
}

int main(int argc, char *argv[]) {
    google::InitGoogleLogging(argv[0]);

    auto se4window = std::make_unique<se4::Window>("Title", SCREEN_WIDTH, SCREEN_HEIGHT);

    //For loading PNG images
    IMG_Init(IMG_INIT_PNG);

    auto entityManager = std::make_unique<se4::EntityManager>();
    auto world = std::make_shared<se4::World>(std::move(entityManager));
    world->setRenderWindow(std::move(se4window));

    // 엔티티 선언
    auto entity = world->createEntity();
    auto yeji = world->createEntity();
    auto entity2 = world->createEntity();

    auto input_acc = se4::makeUpdater(inputCallback);
    world->addUpdater(std::move(input_acc));

    auto yejiUpdater = se4::makeUpdater(
            [](int dt, Position3fHandle pos3fHandler, XAxisAccelerationHandle accelerationHandler,
               YejiHandle yeji) -> void {
                pos3fHandler->x += accelerationHandler->acceleration;
            });
    world->addUpdater(std::move(yejiUpdater));

    // 엔티티에 필요한 컴포넌트 선언
    entity.addComponent(se4::Position2d(100, 100));
    entity.addComponent(se4::Volume2d(100.0f, 200.0f));
    entity.addComponent(se4::RenderComponent("resource/walk.png"));

    yeji.addComponent(se4::Position2d(500, 200));
    yeji.addComponent(se4::Volume2d(100.0f, 100.0f));
    yeji.addComponent(se4::InputComponent(true));
    yeji.addComponent(se4::RenderComponent("resource/yeji.png"));
    yeji.addComponent(XAxisAcceleration(0.0f));
    yeji.addComponent(Yeji());
    // yeji.addComponent(InputComponent(액션배열(키조합+액션, ...) or 가변인자 액션))

    entity2.addComponent(se4::Position2d(200, 200));
    entity2.addComponent(se4::Volume2d(100.0f, 200.0f));
    entity2.addComponent(se4::RenderComponent("resource/walk.png"));

    world->init();

    auto const MS_PER_UPDATE = 16ms;
    auto previous = sc::system_clock::now();
    while (!input.checkKey(se4::KeyState::PRESSED, se4::Key::ESC)) {
        auto start = sc::system_clock::now();

        // se4window.pollKeyEvent(input);
\
        world->update(0);
        // renderUpdater->render();

        world->render(0);

        // 일단은 남는 시간동안 sleep 때림
        std::this_thread::sleep_for(start + MS_PER_UPDATE - sc::system_clock::now());
    }

    //For quitting IMG systems
    IMG_Quit();
    SDL_Quit();

    return 0;
}

