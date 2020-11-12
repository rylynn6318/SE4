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

#include "input/Input.h"
#include "window/Window.h"

#include "box2d/box2d.h"

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 800;

using namespace std::chrono_literals;
namespace sc = std::chrono;

struct Position3f : public se4::Component<Position3f> {
    Position3f(float x, float y, float z) : posX(x), posY(y), posZ(z) {}

    float posX, posY, posZ;
};

struct XAxisAcceleration : public se4::Component<XAxisAcceleration> {
    explicit XAxisAcceleration(float x) : acceleration(x) {}

    float acceleration;
};

struct Volume2f : public se4::Component<Volume2f> {
    Volume2f(float width, float height) : width(width), height(height) {}

    float width, height;
};

struct Yeji : public se4::Component<Yeji> {
};

//렌더러 이거 나중에 클래스의 변수로 옮겨야함
SDL_Renderer *mainRenderer = nullptr;

struct Texture : public se4::Component<Texture> {
    explicit Texture(const char *path) : texture(IMG_LoadTexture(mainRenderer, path)) {}

    SDL_Texture *texture;
};

class RenderUpdater : public se4::Updater {
public:

    ~RenderUpdater() override = default;

    explicit RenderUpdater() {
        signature.addComponent<Position3f>();
        signature.addComponent<Volume2f>();
        signature.addComponent<Texture>();
    }

    // 텍스쳐 선택 바꿔야함
    void update(int dt) override {
        SDL_SetRenderDrawColor(mainRenderer, 0, 0, 0, 255);
        SDL_RenderClear(mainRenderer);

        for (auto &entity : registeredEntities) {
            se4::ComponentHandle<Position3f> pos3fHandler;
            se4::ComponentHandle<Volume2f> vol2fHandler;
            se4::ComponentHandle<Texture> textureHandler;
            parentWorld->unpack(entity, pos3fHandler, vol2fHandler, textureHandler);

            SDL_Rect rect;
            rect.w = vol2fHandler->width;
            rect.h = vol2fHandler->height;
            rect.x = pos3fHandler->posX - (rect.w / 2);
            rect.y = pos3fHandler->posY - (rect.h / 2);

            SDL_RenderCopy(mainRenderer, textureHandler->texture, NULL, &rect);
        }
        SDL_RenderPresent(mainRenderer);
    }
};

int main(int argc, char *argv[]) {
    google::InitGoogleLogging(argv[0]);

    se4::Window se4window("Title", SCREEN_WIDTH, SCREEN_HEIGHT);
    se4::Input input;

    //For loading PNG images
    IMG_Init(IMG_INIT_PNG);

    mainRenderer = SDL_CreateRenderer(se4window.tmp_getWindow(), -1, 0);

    auto entityManager = std::make_unique<se4::EntityManager>();
    auto world = std::make_shared<se4::World>(std::move(entityManager));

    // 엔티티 선언
    auto entity = world->createEntity();
    auto yeji = world->createEntity();
    auto entity2 = world->createEntity();

    using InputHandle = se4::ComponentHandle<se4::InputComponent>;
    using XAxisAccelerationHandle = se4::ComponentHandle<XAxisAcceleration>;
    using Position3fHandle = se4::ComponentHandle<Position3f>;
    using YejiHandle = se4::ComponentHandle<Yeji>;

    // Input 값을 처리하는 Updater
    auto input_acc_callback = [&input](int dt, InputHandle inputHandler, XAxisAccelerationHandle accelerationHandler) -> void {
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
    };
    auto input_acc = std::make_unique<se4::UpdaterFunction<InputHandle, XAxisAccelerationHandle> >(input_acc_callback);
    world->addUpdater(std::move(input_acc));

    // Add Updater
    // 생성자의 템플릿 파라메터로 사용할 컴포넌트의 핸들러 넘겨주고 생성자에는 위에서 선언한 함수 2개 넣어줌
    auto yejiUpdater = std::make_unique<se4::UpdaterFunction<Position3fHandle, XAxisAccelerationHandle, YejiHandle> >(
            [](int dt, Position3fHandle pos3fHandler, XAxisAccelerationHandle accelerationHandler, YejiHandle yeji) -> void {
                pos3fHandler->posX += accelerationHandler->acceleration;
            }
    );
    std::function yejiUpdater2 = [](int dt, Position3fHandle pos3fHandler, XAxisAccelerationHandle accelerationHandler, YejiHandle yeji) -> void {
        pos3fHandler->posX += accelerationHandler->acceleration;
    };
    // auto wrapper = se4::UpdaterFunctionWrapper(yejiUpdater2);
    world->addUpdater(std::move(yejiUpdater));

    auto renderUpdater = std::make_unique<RenderUpdater>();
    world->addUpdater(std::move(renderUpdater));

    // 엔티티에 필요한 컴포넌트 선언
    entity.addComponent(Position3f(100.0f, 100.0f, 0.0f));
    entity.addComponent(Volume2f(100.0f, 200.0f));
    entity.addComponent(Texture("resource/walk.png"));

    yeji.addComponent(Position3f(500.0f, 200.0f, 0.0f));
    yeji.addComponent(Volume2f(100.0f, 100.0f));
    yeji.addComponent(XAxisAcceleration(0.0f));
    yeji.addComponent(se4::InputComponent(true));
    yeji.addComponent(Texture("resource/yeji.png"));
    yeji.addComponent(Yeji());
    // yeji.addComponent(InputComponent(액션배열(키조합+액션, ...) or 가변인자 액션))

    entity2.addComponent(Position3f(200.0f, 200.0f, 0.0f));
    entity2.addComponent(Volume2f(100.0f, 200.0f));
    entity2.addComponent(Texture("resource/walk.png"));

    world->init();

    auto const MS_PER_UPDATE = 16ms;
    auto previous = sc::system_clock::now();
    while (!input.checkKey(se4::KeyState::PRESSED, se4::Key::ESC)) {
        auto start = sc::system_clock::now();

        se4window.pollKeyEvent(input);

        world->update(0);
        // renderUpdater->render();

        // 일단은 남는 시간동안 sleep 때림
        std::this_thread::sleep_for(start + MS_PER_UPDATE - sc::system_clock::now());
    }

    //For quitting IMG systems
    IMG_Quit();
    SDL_Quit();

    return 0;
}

