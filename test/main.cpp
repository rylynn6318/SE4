#include <iostream>
#include <utility>
#include <functional>

#include "SDL.h"
#include "SDL_image.h"
#include "glog/logging.h"
#include "box2d/box2d.h"

#include "se4.hpp"

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 800;


struct Position3f : public se4::Component<Position3f> {
    Position3f(float x, float y, float z) : posX(x), posY(y), posZ(z) {}

    float posX, posY, posZ;
};

struct Volume4f : public se4::Component<Volume4f> {
    //Volume4f(float lt, float rt, float rb, float lb) :leftTop(lt), rightTop(rt), rightBot(rb), leftBot(lb) {}
    Volume4f(float a, float b) : leftTop(a), rightTop(b) {}

    float leftTop, rightTop, rightBot, leftBot;
};


struct Texture : public se4::Component<Texture> {
    Texture(const char *path) : texture(nullptr), path(path) {}

    const char *path;
    SDL_Texture *texture;
};

class RenderUpdater : public se4::Updater {
public:
    RenderUpdater() = delete;

    ~RenderUpdater() override = default;

    RenderUpdater(SDL_Window *window) : renderer(SDL_CreateRenderer(window, -1, 0)) {
        signature.addComponent<Position3f>();
        signature.addComponent<Volume4f>();
        signature.addComponent<Texture>();
    }

    void render() override {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        for (auto &entity : registeredEntities) {
            se4::ComponentHandle<Position3f> pos3fHandler;
            se4::ComponentHandle<Volume4f> vol4fHandler;
            se4::ComponentHandle<Texture> textureHandler;
            parentWorld->unpack(entity, pos3fHandler, vol4fHandler, textureHandler);

            //여기서 상태에 따른 텍스쳐 정하는거 해줘여함

            SDL_Rect rect;
            rect.x = pos3fHandler->posX;
            rect.y = pos3fHandler->posY;
            rect.w = vol4fHandler->leftTop;
            rect.h = vol4fHandler->rightTop;

            //textureHandler->texture = SDL_CreateTextureFromSurface(renderer, textureHandler->surface);            
            textureHandler->texture = IMG_LoadTexture(renderer, textureHandler->path);

            SDL_RenderCopy(renderer, textureHandler->texture, NULL, &rect);
        }
        SDL_RenderPresent(renderer);
    }

    SDL_Renderer *renderer;
};


class YejiUpdater : public se4::Updater {
private:
    std::shared_ptr<int> delta;
    int yeji_id;

public:
    YejiUpdater() = delete;

    ~YejiUpdater() override = default;

    YejiUpdater(std::shared_ptr<int> delta, int yeji) : delta(std::move(delta)), yeji_id(yeji) {
        signature.addComponent<Position3f>();
    }

//    void update(int dt) override {
//        for (auto &entity : registeredEntities) {
//            se4::ComponentHandle<Position3f> pos3fHandler;
//            parentWorld->unpack(entity, pos3fHandler);
//
//            if (entity.id == yeji_id) {
//                pos3fHandler->posX += *delta;
//            }
//        }
//    }

    void update(int dt) override {
        se4::ComponentHandle<Position3f> pos3fHandler;
        update( dt,
                [this](int id) -> bool { return id == yeji_id; },
                [this](se4::ComponentHandle<Position3f> pos3fHandler) -> void { pos3fHandler->posX += *delta; },
                pos3fHandler);
    }

    // args : ComponentHandles
    template<typename Function, typename ComponentType, typename... Args>
    void update(int dt, const std::function<bool(int)> &compare_id, Function &&callback, se4::ComponentHandle<ComponentType> &handle, Args &... args) {
        for (auto &entity : registeredEntities) {
            parentWorld->unpack(entity, handle, args...);

            if (compare_id(entity.id)) {
                std::forward<Function>(callback)(handle, args...);
            }
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

    SDL_Event input;
    bool quit = false;

    auto entityManager = std::make_unique<se4::EntityManager>();
    auto world = std::make_unique<se4::World>(std::move(entityManager));

    world->init();

    // 엔티티 선언
    auto entity = world->createEntity();
    auto entity2 = world->createEntity();

    // Add Updater
    auto yeji_x = std::make_shared<int>();
    auto yejiUpdater = std::make_unique<YejiUpdater>(yeji_x, entity2.entity.id);
    world->addUpdater(std::move(yejiUpdater));
    auto renderUpdater = std::make_unique<RenderUpdater>(window);
    world->addUpdater(std::move(renderUpdater));

    // 엔티티에 필요한 컴포넌트 선언
    entity.addComponent(Position3f(100.0f, 100.0f, 0.0f));
    entity.addComponent(Volume4f(100.0f, 100.0f));
    entity.addComponent(Texture("resource/fuck.png"));

    entity2.addComponent(Position3f(500.0f, 200.0f, 0.0f));
    entity2.addComponent(Volume4f(800.0f, 521.0f));
    entity2.addComponent(Texture("resource/yeji.png"));

    while (!quit) {
        SDL_PollEvent(&input);
        switch (input.type) {
            case SDL_KEYDOWN:
                switch (input.key.keysym.sym) {
                    case SDLK_a:
                        *yeji_x = *yeji_x >= 0 ? -1 : *yeji_x - 1;
                        break;
                    case SDLK_d:
                        *yeji_x = *yeji_x <= 0 ? 1 : *yeji_x + 1;
                        break;
                }
                break;
            case SDL_QUIT:
                quit = true;
                break;
        }

        world->update(20);
        world->render();
    }


    SDL_DestroyWindow(window);
    //For quitting IMG systems
    IMG_Quit();
    SDL_Quit();

    return 0;
}

