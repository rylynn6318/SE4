#include <iostream>

#include "SDL.h"
#include "SDL_image.h"
#include "glog/logging.h"
#include "box2d/box2d.h"

#include "se4.hpp"

using namespace se4;

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 800;
double dt = 1 / 60.0f; //60fps
double currentTime, lastTime, frameTime, accumulator = 0.0;
float deltaTime = 0;

struct Position3f : public Component<Position3f>
{
    Position3f(float x, float y, float z) :posX(x), posY(y), posZ(z) {}
    float posX, posY, posZ;
};

struct Volume4f :public Component<Volume4f>
{
    Volume4f(float lt, float rt, float rb, float lb) :leftTop(lt), rightTop(rt), rightBot(rb), leftBot(lb) {}   
    float leftTop, rightTop, rightBot, leftBot;
};

struct Volume2f : public Component<Volume2f>
{
    Volume2f(float width, float height) :width(width), height(height) {}
    float width, height;
};


struct Render : public Component<Render>
{
    Render(const char *path) : texture(nullptr), path(path) {}
    
    const char* path;
    SDL_Texture* texture;
};

class RenderUpdater : public Updater
{
public:
    RenderUpdater() = delete;
    virtual ~RenderUpdater() = default;

    RenderUpdater(SDL_Window* window) : renderer(SDL_CreateRenderer(window, -1, 0))
    {
        signature.addComponent<Position3f>();
        signature.addComponent<Volume2f>();
        signature.addComponent<Render>();
    }
    
    void render()
    {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        
        for (auto& entity : registeredEntities)
        {
            ComponentHandle<Position3f> pos3fHandler;
            ComponentHandle<Volume2f> vol2fHandler;
            ComponentHandle<Render> textureHandler;
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

    SDL_Renderer* renderer;
};

int main(int argc, char* argv[])
{
    google::InitGoogleLogging(argv[0]);
    LOG(INFO) << "test app start";
    b2Vec2 gravity = b2Vec2(0.0f, -9.8f);
    b2World test_world(gravity);
    LOG(ERROR) << "error log test";

    SDL_Init(SDL_INIT_EVERYTHING);
    //For loading PNG images
    IMG_Init(IMG_INIT_PNG);

    SDL_Window* window = SDL_CreateWindow(
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

    // Add Updater
    std::unique_ptr<Updater> renderUpdater = std::make_unique<RenderUpdater>(window);
    world->addUpdater(std::move(renderUpdater));

    world->init();



    auto entity = world->createEntity();
    entity.addComponent(Position3f(100.0f, 100.0f, 0.0f));
    entity.addComponent(Volume2f(100.0f, 200.0f));
    entity.addComponent(Render("resource/walk.png"));

    auto entity2 = world->createEntity();
    entity2.addComponent(Position3f(200.0f, 100.0f, 0.0f));
    entity2.addComponent(Volume2f(100.0f, 200.0f));
    entity2.addComponent(Render("resource/walk.png"));

    currentTime = SDL_GetTicks();
    double t = 0.0;
    while (!quit)
    {
        SDL_PollEvent(&input);

        lastTime = SDL_GetTicks();
        frameTime = lastTime - currentTime;
        if (frameTime > 0.25) frameTime = 0.25;
        currentTime = lastTime;
        accumulator += frameTime;

        while (frameTime > 0.0)
        {            
            deltaTime = std::min(frameTime, dt);
            world->update(deltaTime);
            accumulator += dt;
            frameTime -= dt;
            LOG(ERROR) << deltaTime;
        }
        
        world->render();
        if (input.type == SDL_QUIT) quit = true;
    }


    SDL_DestroyWindow(window);
    //For quitting IMG systems
    IMG_Quit();
    SDL_Quit();

    return 0;
}

