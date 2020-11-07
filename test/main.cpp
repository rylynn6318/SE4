#include <iostream>
#include <utility>
#include <functional>

#include "SDL.h"
#include "SDL_image.h"
#include "glog/logging.h"
#include "box2d/box2d.h"

#include "se4.hpp"
#include "updater/UpdaterTemplate.h"
#include "input/InputComponent.h"

#include "wrapper/SDL2InputWrapper.h"

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 800;
double dt = 1 / 60.0f; //60fps
double currentTime, lastTime, frameTime, accumulator = 0.0;
float deltaTime = 0;

struct Position3f : public se4::Component<Position3f> {
    Position3f(float x, float y, float z) : posX(x), posY(y), posZ(z) {}

    float posX, posY, posZ;
};

struct XAxisAcceleration : public se4::Component<XAxisAcceleration> {
    explicit XAxisAcceleration(float x) : acceleration(x) {}

    float acceleration;
};

struct Volume4f : public se4::Component<Volume4f> {
    float leftTop, rightTop, rightBot, leftBot;
};

struct Volume2f : public se4::Component<Volume2f> {
    Volume2f(float width, float height) : width(width), height(height) {}

    float width, height;
};

struct Render : public se4::Component<Render> {
    Render(const char *path) : texture(nullptr), path(path) {}

    const char *path;
    SDL_Texture *texture;
};

class RenderUpdater : public se4::Updater {
private:
    SDL_Renderer *renderer;
public:
    RenderUpdater() = delete;

    ~RenderUpdater() override = default;

    RenderUpdater(SDL_Window *window) : renderer(SDL_CreateRenderer(window, -1, 0)) {
        signature.addComponent<Position3f>();
        signature.addComponent<Volume2f>();
        signature.addComponent<Render>();
    }

    void render() {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        for (auto &entity : registeredEntities) {
            se4::ComponentHandle<Position3f> pos3fHandler;
            se4::ComponentHandle<Volume2f> vol2fHandler;
            se4::ComponentHandle<Render> textureHandler;
            parentWorld->unpack(entity, pos3fHandler, vol2fHandler, textureHandler);
            //여기서 상태에 따른 텍스쳐 정하는거 해줘여함
            //애니메이션 구현할 코드 위치

            SDL_Rect rect;
            rect.x = pos3fHandler->posX;
            rect.y = pos3fHandler->posY;
            rect.w = vol2fHandler->width;
            rect.h = vol2fHandler->height;

            SDL_Rect imgPartRect;
            imgPartRect.x = 0; //시작 x좌표 한상태에서는 여기서만
            imgPartRect.y = 0; //시작 y좌표 다른상태로 가야할 때
            imgPartRect.w = rect.w;
            imgPartRect.h = rect.h;

            textureHandler->texture = IMG_LoadTexture(renderer, textureHandler->path);


            SDL_RenderCopy(renderer, textureHandler->texture, &imgPartRect, &rect);
        }
        SDL_RenderPresent(renderer);
    }
};

class InputUpdater : public se4::Updater {
public:
    InputUpdater() = default;

    ~InputUpdater() override = default;

    void init() override {
        // add all signature
        signature.addComponent<se4::InputComponent>();
    }

    std::vector<std::unique_ptr<se4::Updater>> funcs;

    void update(int dtt) override {
        for (auto &func : funcs) {
            func->update(dtt);
        }
    }
};

int main(int argc, char *argv[]) {
    google::InitGoogleLogging(argv[0]);

    LOG(INFO) << "test app start";
    b2Vec2 gravity = b2Vec2(0.0f, -9.8f);
    b2World test_world(gravity);
    LOG(ERROR) << "error log test";

    SDL_Init(SDL_INIT_EVERYTHING);
    //For loading PNG images
    IMG_Init(IMG_INIT_PNG);

    SDL_Window *window = SDL_CreateWindow(
            "SE4 engine",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN
    );

//    SDL_Window *window2 = SDL_CreateWindow(
//            "SE4 engine 222",
//            SDL_WINDOWPOS_UNDEFINED,
//            SDL_WINDOWPOS_UNDEFINED,
//            SCREEN_WIDTH,
//            SCREEN_HEIGHT,
//            SDL_WINDOW_SHOWN
//    );

    bool quit = false;

    // 이건 Game.h 혹은 World.h에 있어야 함
    SDL2InputWrapper inputWrapper;

    auto entityManager = std::make_unique<se4::EntityManager>();
    auto world = std::make_unique<se4::World>(std::move(entityManager));

    // 엔티티 선언
    auto entity = world->createEntity();
    auto yeji = world->createEntity();
    auto entity2 = world->createEntity();

    // Input 값을 처리하는 Updater
    auto input_acc_callback = [&inputWrapper](se4::ComponentHandle<se4::InputComponent> inputHandler,
                                              se4::ComponentHandle<XAxisAcceleration> accelerationHandler) -> void {
        auto keymap = inputWrapper.Keymap();
        if (inputHandler->is_selected) {
            if (keymap[se4::Key::A] == se4::InputState::PRESSED)
                accelerationHandler->acceleration =
                        accelerationHandler->acceleration >= 0 ? -1 : accelerationHandler->acceleration - 1;
            if (keymap[se4::Key::D] == se4::InputState::PRESSED)
                accelerationHandler->acceleration =
                        accelerationHandler->acceleration <= 0 ? 1 : accelerationHandler->acceleration + 1;
        }
    };
    auto input_acc = std::make_unique<
            se4::UpdaterTemplate<
                    se4::ComponentHandle<se4::InputComponent>, se4::ComponentHandle<XAxisAcceleration>
            >
    >(input_acc_callback);
    world->addUpdater(std::move(input_acc));

    // Add Updater
    // 업데이터 안에서 사용할 콜백 정의
    auto callback = [](se4::ComponentHandle<Position3f> pos3fHandler,
                       se4::ComponentHandle<XAxisAcceleration> accelerationHandler) -> void {
        pos3fHandler->posX += accelerationHandler->acceleration;
    };
    // 예지만 움직이게 하기 위한 함수 정의
    auto compare_id = [yeji](int id) -> bool { return id == yeji.entity.id; };
    // 생성자의 템플릿 파라메터로 사용할 컴포넌트의 핸들러 넘겨주고 생성자에는 위에서 선언한 함수 2개 넣어줌
    auto yejiUpdater = std::make_unique<
            se4::UpdaterTemplate<
                    se4::ComponentHandle<Position3f>, se4::ComponentHandle<XAxisAcceleration>
            >
    >(callback, compare_id);
    world->addUpdater(std::move(yejiUpdater));

    auto renderUpdater = std::make_unique<RenderUpdater>(window);
    world->addUpdater(std::move(renderUpdater));

    // 엔티티에 필요한 컴포넌트 선언
    entity.addComponent(Position3f(100.0f, 100.0f, 0.0f));
    entity.addComponent(Volume2f(100.0f, 200.0f));
    entity.addComponent(Render("resource/walk.png"));

    yeji.addComponent(Position3f(500.0f, 200.0f, 0.0f));
    yeji.addComponent(Volume2f(800.0f, 521.0f));
    yeji.addComponent(XAxisAcceleration(0.0f));
    yeji.addComponent(se4::InputComponent(true));
    yeji.addComponent(Render("resource/yeji.png"));
    // yeji.addComponent(InputComponent(액션배열(키조합+액션, ...) or 가변인자 액션))

    entity2.addComponent(Position3f(200.0f, 200.0f, 0.0f));
    entity2.addComponent(Volume2f(100.0f, 200.0f));
    entity2.addComponent(Render("resource/walk.png"));

    world->init();

    currentTime = SDL_GetTicks();
    double t = 0.0;
    while (!quit) {
        inputWrapper.pollInput();
        quit = inputWrapper.quit;

        inputUpdater.update(0);
        world->update(0);
        world->render();
    }

    SDL_DestroyWindow(window);
    //For quitting IMG systems
    IMG_Quit();
    SDL_Quit();

    return 0;
}

