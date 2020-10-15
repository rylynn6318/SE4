#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <se4.hpp>

using namespace se4;

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 800;

struct Position3f : public Component<Position3f>
{
    Position3f(float x, float y, float z) :posX(x), posY(y), posZ(z) {}
    float posX, posY, posZ;
};

struct Volume4f :public Component<Volume4f>
{
    //Volume4f(float lt, float rt, float rb, float lb) :leftTop(lt), rightTop(rt), rightBot(rb), leftBot(lb) {}
    Volume4f(float a, float b) :leftTop(a), rightTop(b) {}
    float leftTop, rightTop, rightBot, leftBot;
};

struct Render : public Component<Render>
{

};

struct Texture : public Component<Texture>
{
    Texture(std::string path) : surface(IMG_Load(path.c_str())), texture(nullptr)
    {
        if (surface)
            std::clog << "image load failed : " << IMG_GetError() << std::endl;
    }
    virtual ~Texture()
    {
        //SDL_FreeSurface(surface);
    }


    SDL_Surface* surface;
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
        signature.addComponent<Volume4f>();
        signature.addComponent<Texture>();
    }

    void render()
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        for (auto& entity : registeredEntities)
        {
            ComponentHandle<Position3f> pos3f;
            ComponentHandle<Volume4f> vol4f;
            ComponentHandle<Texture> texture;
            parentWorld->unpack(entity, pos3f, vol4f, texture);

            SDL_Rect rect;
            rect.x = pos3f->posX;
            rect.y = pos3f->posY;
            rect.w = vol4f->leftTop;
            rect.h = vol4f->rightTop;

            texture->texture = SDL_CreateTextureFromSurface(renderer, texture->surface);
            //Copying the texture on to the window using renderer and rectangle
            SDL_RenderCopy(renderer, texture->texture, NULL, &rect);
        }
        SDL_RenderPresent(renderer);
    }

    SDL_Renderer* renderer;
};

int main(int argc, char** args)
{
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
    entity.addComponent(Volume4f(100.0f, 100.0f));
    entity.addComponent(Texture("fuck.png"));

    auto entity2 = world->createEntity();
    entity2.addComponent(Position3f(500.0f, 200.0f, 0.0f));
    entity2.addComponent(Volume4f(100.0f, 100.0f));
    entity2.addComponent(Texture("fuck.png"));

    while (!quit)
    {
        while (SDL_PollEvent(&input) > 0)
        {
            if (input.type == SDL_QUIT) quit = true;
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