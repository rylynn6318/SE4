#include <iostream>
#include <utility>

#include "SDL.h"
#include "SDL_image.h"
#include "glog/logging.h"
#include "box2d/box2d.h"

#include "se4.hpp"

using namespace se4;

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 800;


struct Position3f : public Component<Position3f> {
    Position3f(float x, float y, float z) : posX(x), posY(y), posZ(z) {}

    float posX, posY, posZ;
};

struct Volume4f : public Component<Volume4f> {
    //Volume4f(float lt, float rt, float rb, float lb) :leftTop(lt), rightTop(rt), rightBot(rb), leftBot(lb) {}
    Volume4f(float a, float b) : leftTop(a), rightTop(b) {}

    float leftTop, rightTop, rightBot, leftBot;
};


struct Texture : public Component<Texture> {
    Texture(const char *path) : texture(nullptr), path(path) {}

    const char *path;
    SDL_Texture *texture;
};

class RenderUpdater : public Updater {
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
            ComponentHandle<Position3f> pos3fHandler;
            ComponentHandle<Volume4f> vol4fHandler;
            ComponentHandle<Texture> textureHandler;
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


class YejiUpdater : public Updater {
private:
    std::shared_ptr<int> delta;
    int yeji_id;

public:
    YejiUpdater() = delete;

    ~YejiUpdater() override = default;

    YejiUpdater(std::shared_ptr<int> delta, int yeji) : delta(std::move(delta)), yeji_id(yeji) {
        signature.addComponent<Position3f>();
    }


    void update(int dt) override {
        for (auto &entity : registeredEntities) {
            ComponentHandle<Position3f> pos3fHandler;
            parentWorld->unpack(entity, pos3fHandler);

            LOG(ERROR) << "entity : " << entity.id;
            if(entity.id == yeji_id){
                pos3fHandler->posX += *delta;
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

    SDL_Event input;
    bool quit = false;

    auto entityManager = std::make_unique<EntityManager>();
    auto world = std::make_unique<World>(std::move(entityManager));

    world->init();

    auto entity = world->createEntity();
    entity.addComponent(Position3f(100.0f, 100.0f, 0.0f));
    entity.addComponent(Volume4f(100.0f, 100.0f));
    entity.addComponent(Texture("resource/fuck.png"));

    auto entity2 = world->createEntity();
    entity2.addComponent(Position3f(500.0f, 200.0f, 0.0f));
    entity2.addComponent(Volume4f(800.0f, 521.0f));
    entity2.addComponent(Texture("resource/yeji.png"));

    // Add Updater
    auto yeji_x = std::make_shared<int>();
    std::unique_ptr<Updater> renderUpdater = std::make_unique<RenderUpdater>(window);
    std::unique_ptr<Updater> yejiUpdater = std::make_unique<YejiUpdater>(yeji_x, entity2.entity.id);
    world->addUpdater(std::move(renderUpdater));
    world->addUpdater(std::move(yejiUpdater));

    while (!quit) {
        while (SDL_PollEvent(&input) > 0) {
            switch (input.type) {
                case SDL_KEYDOWN:
                    switch (input.key.keysym.sym) {
                        case SDLK_a:
                            *yeji_x = -1;
                            break;
                        case SDLK_d:
                            *yeji_x = 1;
                            break;
                    }
                    LOG(ERROR) << "yeji_x : " << *yeji_x;
                    break;
                case SDL_QUIT:
                    quit = true;
                    break;
            }

            world->update(20);
            world->render();
        }
    }


    SDL_DestroyWindow(window);
    //For quitting IMG systems
    IMG_Quit();
    SDL_Quit();

    return 0;
}

