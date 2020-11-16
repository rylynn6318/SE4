#include <iostream>
#include <utility>
#include <functional>
#include <cstring>
#include <chrono>
#include <thread>

#include "SDL.h"
#include "SDL_image.h"
#include "glog/logging.h"
#include "box2d/box2d.h"

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

b2Vec2 gravity(0.0f, 10.0f);
b2World b2world(gravity);

struct PhysicsBody : public se4::Component<PhysicsBody> {
    PhysicsBody(bool isMovable, float friction, float restitution) :
            isMovable(isMovable),
            body(nullptr),
            lastVec2(0.0f, 0.0f),
            jumpSteps(0) {

        fixtureDef.friction = friction;
        fixtureDef.restitution = restitution;
    }

    bool isMovable;
    b2Body *body;
    b2BodyDef bodyDef;
    b2FixtureDef fixtureDef;
    b2Vec2 lastVec2;
    int jumpSteps;
};

const float SCALE = 100.0f;

class PhysicsUpdater : public se4::Updater {
public:
    PhysicsUpdater() {
        signature.addComponent<se4::Position2d>();
        signature.addComponent<se4::Volume2d>();
        signature.addComponent<PhysicsBody>();
    }

    void update(int dt) {
        for (auto &entity : registeredEntities) {
            se4::ComponentHandle<se4::Position2d> pos2dHandler;
            se4::ComponentHandle<se4::Volume2d> vol2dHandler;
            se4::ComponentHandle<PhysicsBody> physicsHandler;
            parentWorld->unpack(entity, pos2dHandler, vol2dHandler, physicsHandler);


            //동적, 정적 설정
            if (physicsHandler->isMovable) {
                physicsHandler->bodyDef.type = b2_dynamicBody;
            } else {
                physicsHandler->bodyDef.type = b2_staticBody;
            }

            physicsHandler->bodyDef.position.Set(pos2dHandler->x / SCALE, pos2dHandler->y / SCALE);
            if (physicsHandler->body)
                b2world.DestroyBody(physicsHandler->body);

            physicsHandler->body = b2world.CreateBody(&physicsHandler->bodyDef);

            b2PolygonShape dynamicBox;
            dynamicBox.SetAsBox(vol2dHandler->width / 2 / SCALE, vol2dHandler->height / 2 / SCALE);

            physicsHandler->fixtureDef.shape = &dynamicBox;
            physicsHandler->fixtureDef.density = 1.0f;

            //fixture delete?
            physicsHandler->body->CreateFixture(&physicsHandler->fixtureDef);

            //이전 속도값 더해줌
            physicsHandler->body->SetLinearVelocity(physicsHandler->lastVec2);

            while (physicsHandler->jumpSteps > 0) {
                float force = physicsHandler->body->GetMass() * 100 / (1 / 60.0); //f = mv/t
                //spread this over 6 time steps
                force /= 6.0;
                physicsHandler->body->ApplyForceToCenter(b2Vec2(0, force), true);
                physicsHandler->jumpSteps--;
            }


            //포지션 표시
            //std::cout<< physicsHandler->body->GetLinearVelocity().x << ", " << physicsHandler->body->GetLinearVelocity().y << std::endl;

            b2world.Step(1.0f / 60.0f, 6, 2);

            b2Vec2 pos = physicsHandler->body->GetPosition();
            //속도값 저장
            physicsHandler->lastVec2 = physicsHandler->body->GetLinearVelocity();

            //포지션 갱신
            pos2dHandler->x = pos.x * SCALE;
            pos2dHandler->y = pos.y * SCALE;
        }
    }
};

class PlayerListener : public b2ContactListener, se4::Updater {
public:
    PlayerListener() {
        signature.addComponent<se4::Position2d>();
        signature.addComponent<se4::Volume2d>();
        signature.addComponent<PhysicsBody>();
    }

    void BeginContact(b2Contact *contact) override {
        auto entity = (void *) contact->GetFixtureA()->GetBody()->GetUserData().pointer;
        auto phygics = static_cast<PhysicsBody *>(entity);
    }

    void update(int dt) {
        for (auto &entity : registeredEntities) {
            se4::ComponentHandle<se4::Position2d> pos2dHandler;
            se4::ComponentHandle<se4::Volume2d> vol2dHandler;
            se4::ComponentHandle<PhysicsBody> physicsHandler;
            parentWorld->unpack(entity, pos2dHandler, vol2dHandler, physicsHandler);
            for (b2Contact *contact = b2world.GetContactList(); contact != nullptr; contact->GetNext()) {
                // contact->GetFixtureA()->GetBody()->GetUserData();
            }
        }
    }

};

se4::Input input;

auto inputCallback(int dt, InputHandle inputHandler, se4::ComponentHandle<PhysicsBody> physicsHandler) {
    if (inputHandler->is_selected) {
        if (input.checkKey(se4::KeyState::PRESSED, se4::Key::A) ||
            input.checkKey(se4::KeyState::HELD_DOWN, se4::Key::A))
            physicsHandler->lastVec2 = physicsHandler->lastVec2 + b2Vec2(-0.1, 0);
        if (input.checkKey(se4::KeyState::PRESSED, se4::Key::D) ||
            input.checkKey(se4::KeyState::HELD_DOWN, se4::Key::D))
            physicsHandler->lastVec2 = physicsHandler->lastVec2 + b2Vec2(0.1, 0);
        if (input.checkKey(se4::KeyState::PRESSED, se4::Key::W))
            physicsHandler->jumpSteps = 6;
        if (input.checkKey(se4::KeyState::PRESSED, se4::Key::S))
            physicsHandler->lastVec2 = physicsHandler->lastVec2 + b2Vec2(0, 0.1);
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
    auto background = world->createEntity();
    auto yeji = world->createEntity();
    auto entity2 = world->createEntity();
    auto floor = world->createEntity();
    auto leftWall = world->createEntity();
    auto rightWall = world->createEntity();
    auto ceil = world->createEntity();

    auto physicsUpdater = std::make_unique<PhysicsUpdater>();
    world->addUpdater(std::move(physicsUpdater));

    auto input_acc = se4::makeUpdater(inputCallback);
    world->addUpdater(std::move(input_acc));

    auto yejiUpdater = se4::makeUpdater(
            [](int dt, Position3fHandle pos3fHandler, XAxisAccelerationHandle accelerationHandler,
               YejiHandle yeji) -> void {
                pos3fHandler->x += accelerationHandler->acceleration;
            });
    world->addUpdater(std::move(yejiUpdater));

    // 엔티티에 필요한 컴포넌트 선언
    background.addComponent(se4::Position2d(600, 400));
    background.addComponent(se4::Volume2d(1200.0f, 800.0f));
    background.addComponent(se4::RenderComponent("resource/background.png"));

    yeji.addComponent(se4::Position2d(500, 200));
    yeji.addComponent(se4::Volume2d(100.0f, 100.0f));
    yeji.addComponent(se4::InputComponent(true));
    yeji.addComponent(se4::RenderComponent("resource/yeji.png"));
    yeji.addComponent(PhysicsBody(true, 0.0f, 0.1f));
    yeji.addComponent(Yeji());
    // yeji.addComponent(InputComponent(액션배열(키조합+액션, ...) or 가변인자 액션))

    entity2.addComponent(se4::Position2d(200, 200));
    entity2.addComponent(se4::Volume2d(100.0f, 200.0f));
    entity2.addComponent(se4::RenderComponent("resource/walk.png"));

    //지형 관련 엔티티, 이건 추후 지형관련 옵션으로 따로 빼서
    //ShapePolygon 말고 Edge로 처리해서 더 깔끔하게 코드짤 수 있을듯
    //https://www.iforce2d.net/b2dtut/fixtures
    floor.addComponent(se4::Position2d(0, SCREEN_HEIGHT - 200.0f));
    floor.addComponent(se4::Volume2d(SCREEN_WIDTH * 2, 1.0f));
    floor.addComponent(PhysicsBody(false, 0.3f, 0.0f));

    ceil.addComponent(se4::Position2d(0.0, 0.0f));
    ceil.addComponent(se4::Volume2d(SCREEN_WIDTH * 2, 200.0f));
    ceil.addComponent(PhysicsBody(false, 0.3f, 0.0f));

    leftWall.addComponent(se4::Position2d(0.0f, 0.0f));
    leftWall.addComponent(se4::Volume2d(0.0f, SCREEN_HEIGHT * 200));
    leftWall.addComponent(PhysicsBody(false, 0.3f, 0.0f));

    rightWall.addComponent(se4::Position2d(SCREEN_WIDTH, 0.0f));
    rightWall.addComponent(se4::Volume2d(0.0f, SCREEN_HEIGHT * 2));
    rightWall.addComponent(PhysicsBody(false, 0.3f, 0.0f));


    world->init();

    auto const MS_PER_UPDATE = 16ms;
    auto previous = sc::system_clock::now();
    while (!input.checkKey(se4::KeyState::PRESSED, se4::Key::ESC)) {
        auto start = sc::system_clock::now();

        se4::Window::pollKeyEvent(input);

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

