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
#include "core/Game.h"
#include "input/Input.h"
#include "window/Window.h"


#include "box2d/box2d.h"

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

using namespace std::chrono_literals;
namespace sc = std::chrono;

auto se4window = std::make_unique<se4::Window>("Title", SCREEN_WIDTH, SCREEN_HEIGHT);

struct Yeji : public se4::Component<Yeji> {
};

using InputHandle = se4::ComponentHandle<se4::InputComponent>;

b2Vec2 gravity(0.0f, 10.0f);
b2World b2world(gravity);

namespace se4 {
    enum class BodyType {
        CIRCLE,
        RECTANGLE,
        EDGE
    };
}

//std::bit 해서 생성자로 받은거 각각 category bit설정
//body들 뒤지는거 관리해줘야함;

struct PhysicsBody : public se4::Component<PhysicsBody> {
    PhysicsBody(bool isMovable, float friction, float restitution, float density, se4::BodyType bodyType) :
            isMovable(isMovable),
            body(nullptr),
            lastVec2(0.0f, 0.0f),
            forceX(0),
            forceY(0),
            jumpSteps(0),
            bodyType(bodyType) {
        fixtureDef.friction = friction;
        fixtureDef.restitution = restitution;
        fixtureDef.density = density;
    }

    se4::BodyType bodyType;
    bool isMovable;
    b2Body *body;
    b2BodyDef bodyDef;
    b2FixtureDef fixtureDef;
    b2Vec2 lastVec2;
    float forceX;
    float forceY;
    int jumpSteps;
    bool die = false;
};

const float SCALE = 100.0f;

class PhysicsUpdater : public se4::Updater {
public:
    PhysicsUpdater() {
        signature.addComponent<se4::Position2d>();
        signature.addComponent<se4::Volume2d>();
        signature.addComponent<PhysicsBody>();
    }

    void update(int dt) override {
        for (auto &entity : registeredEntities) {
            se4::ComponentHandle<se4::Position2d> pos2dHandler;
            se4::ComponentHandle<se4::Volume2d> vol2dHandler;
            se4::ComponentHandle<PhysicsBody> physicsHandler;
            parentWorld->unpack(entity, pos2dHandler, vol2dHandler, physicsHandler);

            //동적, 정적 설정
            if (physicsHandler->isMovable) {
                physicsHandler->bodyDef.type = b2_dynamicBody;
                physicsHandler->fixtureDef.filter.categoryBits = 0x0002;
                physicsHandler->fixtureDef.filter.maskBits = 0x0004;
            } else {
                physicsHandler->bodyDef.type = b2_staticBody;
                physicsHandler->fixtureDef.filter.categoryBits = 0x0004;
                physicsHandler->fixtureDef.filter.maskBits = 0x0002;
            }

            physicsHandler->bodyDef.position.Set(pos2dHandler->x / SCALE, pos2dHandler->y / SCALE);

            //TODO: make destroy
            if (physicsHandler->body)
                b2world.DestroyBody(physicsHandler->body);

            physicsHandler->body = b2world.CreateBody(&physicsHandler->bodyDef);

            b2PolygonShape dynamicBox;
            b2CircleShape circleBox;
            b2EdgeShape edge;

            if (physicsHandler->bodyType == se4::BodyType::RECTANGLE) {
                dynamicBox.SetAsBox(vol2dHandler->width / 2 / SCALE, vol2dHandler->height / 2 / SCALE);
                physicsHandler->fixtureDef.shape = &dynamicBox;
            } else if (physicsHandler->bodyType == se4::BodyType::CIRCLE) {
                circleBox.m_p = (b2Vec2(pos2dHandler->x / SCALE, pos2dHandler->y / SCALE));
                circleBox.m_radius = std::max(vol2dHandler->height, vol2dHandler->width) / 2 / SCALE;
                physicsHandler->fixtureDef.shape = &circleBox;
            } else if (physicsHandler->bodyType == se4::BodyType::EDGE) {
                edge.m_vertex1 = (b2Vec2(pos2dHandler->x / SCALE, pos2dHandler->y / SCALE));
                edge.m_vertex2 = edge.m_vertex1 + b2Vec2(vol2dHandler->width / SCALE, 0);
                physicsHandler->fixtureDef.shape = &edge;
            }

            physicsHandler->body->CreateFixture(&physicsHandler->fixtureDef);

            //이전 속도값 더해줌
            physicsHandler->body->SetLinearVelocity(physicsHandler->lastVec2);

            //TODO : 필드가 만들어지면 나중에 force벡터로 통합해야함 ㅇㅇ
            physicsHandler->body->ApplyForceToCenter(b2Vec2(physicsHandler->forceX, -physicsHandler->forceY), true);
            physicsHandler->forceY = 0;
            physicsHandler->forceX = 0;

            //TODO: deltatime으로 바꿀 것
            b2world.Step(1.0f / 60.0f, 6, 2);

            b2Vec2 pos = physicsHandler->body->GetPosition();

            //속도값 저장
            physicsHandler->lastVec2 = physicsHandler->body->GetLinearVelocity();

            //포지션 갱신
            pos2dHandler->x = pos.x * SCALE;
            pos2dHandler->y = pos.y * SCALE;

            if (physicsHandler->body->GetUserData().pointer) {
                std::cout << "죽음" << std::endl;
                b2world.DestroyBody(physicsHandler->body);
            }
        }
    }
};

class PlayerListener : public se4::Updater {
public:
    PlayerListener() {
        signature.addComponent<se4::Position2d>();
        signature.addComponent<se4::Volume2d>();
        signature.addComponent<PhysicsBody>();
    }

    //void BeginContact(b2Contact *contact) override {
    //    auto entity = (void *) contact->GetFixtureA()->GetBody()->GetUserData().pointer;
    //    auto phygics = static_cast<PhysicsBody *>(entity);
    //}
    void update(int dt) override {
        for (auto &entity : registeredEntities) {
            se4::ComponentHandle<se4::Position2d> pos2dHandler;
            se4::ComponentHandle<se4::Volume2d> vol2dHandler;
            se4::ComponentHandle<PhysicsBody> physicsHandler;
            parentWorld->unpack(entity, pos2dHandler, vol2dHandler, physicsHandler);
            for (b2Contact *contact = b2world.GetContactList(); contact != nullptr; contact = contact->GetNext()) {

            }
        }
    }

};

std::unique_ptr<se4::Level> getLevel() {
    auto level = std::make_unique<se4::Level>();

    // 엔티티 선언
    auto entity = level->createEntity();
    auto yeji = level->createEntity();
    auto entity2 = level->createEntity();
    auto floor = level->createEntity();
    auto leftWall = level->createEntity();
    auto rightWall = level->createEntity();
    auto roof = level->createEntity();

    auto physicsUpdater = std::make_unique<PhysicsUpdater>();
    level->addUpdater(std::move(physicsUpdater));

    auto playerListener = std::make_unique<PlayerListener>();
    level->addUpdater(std::move(playerListener));

    auto input_acc = se4::makeUpdater(
            [](int dt, InputHandle inputHandler, se4::ComponentHandle<PhysicsBody> physicsHandler) {
                if (inputHandler->is_selected) {
                    if (se4::Game::Instance().inputManager.checkKey(se4::KeyState::PRESSED, se4::Key::A) ||
                        se4::Game::Instance().inputManager.checkKey(se4::KeyState::HELD_DOWN, se4::Key::A)) {
                        physicsHandler->lastVec2 = physicsHandler->lastVec2 + b2Vec2(-0.1, 0);
                    }
                    if (se4::Game::Instance().inputManager.checkKey(se4::KeyState::PRESSED, se4::Key::D) ||
                        se4::Game::Instance().inputManager.checkKey(se4::KeyState::HELD_DOWN, se4::Key::D)) {
                        physicsHandler->lastVec2 = physicsHandler->lastVec2 + b2Vec2(0.1, 0);
                    }
                    if (se4::Game::Instance().inputManager.checkKey(se4::KeyState::PRESSED, se4::Key::W)) {
                        physicsHandler->forceY = physicsHandler->body->GetMass() * 5 / (1 / 60.0); //f = mv/t , dt로 바꿔야함
                    }
                    if (se4::Game::Instance().inputManager.checkKey(se4::KeyState::HELD_DOWN, se4::Key::W)) {
                        physicsHandler->forceY = physicsHandler->body->GetMass() / (1 / 60.0);
                    }
                    if (se4::Game::Instance().inputManager.checkKey(se4::KeyState::PRESSED, se4::Key::S)) {
                        physicsHandler->lastVec2 = physicsHandler->lastVec2 + b2Vec2(0, 0.1);
                    }
                    if (se4::Game::Instance().inputManager.checkKey(se4::KeyState::PRESSED, se4::Key::Q)) {
                        //se4window->setRenderLevel(2);
                        //se4::Game::Instance().levelManager.setCurrentLevelID(2);
                    }
                }
            });

    // 엔티티에 필요한 컴포넌트 선언
    entity.addComponent(se4::Position2d(SCREEN_WIDTH / 2.0, SCREEN_HEIGHT / 2.0));
    entity.addComponent(se4::Volume2d(SCREEN_WIDTH, SCREEN_HEIGHT));
    entity.addComponent(se4::RenderComponent("resource/bg.png"));

    yeji.addComponent(se4::Position2d(500, 200));
    yeji.addComponent(se4::Volume2d(100.0f, 100.0f));
    yeji.addComponent(se4::InputComponent(true));
    yeji.addComponent(se4::RenderComponent("resource/yeji.png", true));
    yeji.addComponent(PhysicsBody(true, 0.15f, 0.0f, 1, se4::BodyType::RECTANGLE));
    yeji.addComponent(Yeji());
    // yeji.addComponent(InputComponent(액션배열(키조합+액션, ...) or 가변인자 액션))

    entity2.addComponent(se4::Position2d(200, 200));
    entity2.addComponent(se4::Volume2d(100.0f, 200.0f));
    entity2.addComponent(PhysicsBody(true, 0.15f, 0.0f, 1, se4::BodyType::RECTANGLE));
    entity2.addComponent(se4::RenderComponent("resource/walk.png", true));

    //지형 관련 엔티티, 이건 추후 지형관련 옵션으로 따로 빼서
//ShapePolygon 말고 Edge로 처리해서 더 깔끔하게 코드짤 수 있을듯
//https://www.iforce2d.net/b2dtut/fixtures
    floor.addComponent(se4::Position2d(0, SCREEN_HEIGHT));
    floor.addComponent(se4::Volume2d(SCREEN_WIDTH * 2, 1.0f));
    floor.addComponent(PhysicsBody(false, 1.0f, 0.0f, 1, se4::BodyType::RECTANGLE));

    roof.addComponent(se4::Position2d(0, 0));
    roof.addComponent(se4::Volume2d(SCREEN_WIDTH * 2, 1.0f));
    roof.addComponent(PhysicsBody(false, 1.0f, 0.0f, 1, se4::BodyType::RECTANGLE));

    leftWall.addComponent(se4::Position2d(0.0f, 0.0f));
    leftWall.addComponent(se4::Volume2d(0.0f, SCREEN_HEIGHT * 200));
    leftWall.addComponent(PhysicsBody(false, 1.0f, 0.0f, 1, se4::BodyType::RECTANGLE));

    rightWall.addComponent(se4::Position2d(SCREEN_WIDTH, 0.0f));
    rightWall.addComponent(se4::Volume2d(0.0f, SCREEN_HEIGHT * 2));
    rightWall.addComponent(PhysicsBody(false, 1.0f, 0.0f, 1, se4::BodyType::RECTANGLE));

    level->addUpdater(std::move(input_acc));
    level->init();
    return level;
}

std::unique_ptr<se4::Level> getLevel2() {
    auto level = std::make_unique<se4::Level>();

    // 엔티티 선언
    auto entity = level->createEntity();
    auto yeji = level->createEntity();
    auto floor = level->createEntity();
    auto leftWall = level->createEntity();
    auto rightWall = level->createEntity();
    auto roof = level->createEntity();

    auto physicsUpdater = std::make_unique<PhysicsUpdater>();
    level->addUpdater(std::move(physicsUpdater));

    auto playerListener = std::make_unique<PlayerListener>();
    level->addUpdater(std::move(playerListener));

    auto input_acc = se4::makeUpdater(
            [](int dt, InputHandle inputHandler, se4::ComponentHandle<PhysicsBody> physicsHandler) {
                if (inputHandler->is_selected) {
                    if (se4::Game::Instance().inputManager.checkKey(se4::KeyState::PRESSED, se4::Key::A) ||
                        se4::Game::Instance().inputManager.checkKey(se4::KeyState::HELD_DOWN, se4::Key::A)) {
                        physicsHandler->lastVec2 = physicsHandler->lastVec2 + b2Vec2(-0.1, 0);
                    }
                    if (se4::Game::Instance().inputManager.checkKey(se4::KeyState::PRESSED, se4::Key::D) ||
                        se4::Game::Instance().inputManager.checkKey(se4::KeyState::HELD_DOWN, se4::Key::D)) {
                        physicsHandler->lastVec2 = physicsHandler->lastVec2 + b2Vec2(0.1, 0);
                    }
                    if (se4::Game::Instance().inputManager.checkKey(se4::KeyState::PRESSED, se4::Key::W)) {
                        physicsHandler->forceY = physicsHandler->body->GetMass() * 5 / (1 / 60.0); //f = mv/t , dt로 바꿔야함
                    }
                    if (se4::Game::Instance().inputManager.checkKey(se4::KeyState::HELD_DOWN, se4::Key::W)) {
                        physicsHandler->forceY = physicsHandler->body->GetMass() / (1 / 60.0);
                    }
                    if (se4::Game::Instance().inputManager.checkKey(se4::KeyState::PRESSED, se4::Key::S)) {
                        physicsHandler->lastVec2 = physicsHandler->lastVec2 + b2Vec2(0, 0.1);
                    }
                    if (se4::Game::Instance().inputManager.checkKey(se4::KeyState::PRESSED, se4::Key::Q)) {
                        se4window->setRenderLevel(1);
                        se4::Game::Instance().levelManager.setCurrentLevelID(1);
                    }
                }
            });

    // 엔티티에 필요한 컴포넌트 선언
    entity.addComponent(se4::Position2d(SCREEN_WIDTH / 2.0, SCREEN_HEIGHT / 2.0));
    entity.addComponent(se4::Volume2d(SCREEN_WIDTH, SCREEN_HEIGHT));
    entity.addComponent(se4::RenderComponent("resource/background.png"));

    yeji.addComponent(se4::Position2d(500, 200));
    yeji.addComponent(se4::Volume2d(100.0f, 100.0f));
    yeji.addComponent(se4::InputComponent(true));
    yeji.addComponent(se4::RenderComponent("resource/yeji.png", true));
    yeji.addComponent(PhysicsBody(true, 0.15f, 0.0f, 1, se4::BodyType::RECTANGLE));
    yeji.addComponent(Yeji());
    // yeji.addComponent(InputComponent(액션배열(키조합+액션, ...) or 가변인자 액션))

    //지형 관련 엔티티, 이건 추후 지형관련 옵션으로 따로 빼서
//ShapePolygon 말고 Edge로 처리해서 더 깔끔하게 코드짤 수 있을듯
//https://www.iforce2d.net/b2dtut/fixtures
    floor.addComponent(se4::Position2d(0, SCREEN_HEIGHT));
    floor.addComponent(se4::Volume2d(SCREEN_WIDTH * 2, 1.0f));
    floor.addComponent(PhysicsBody(false, 1.0f, 0.0f, 1, se4::BodyType::RECTANGLE));

    roof.addComponent(se4::Position2d(0, 0));
    roof.addComponent(se4::Volume2d(SCREEN_WIDTH * 2, 1.0f));
    roof.addComponent(PhysicsBody(false, 1.0f, 0.0f, 1, se4::BodyType::RECTANGLE));

    leftWall.addComponent(se4::Position2d(0.0f, 0.0f));
    leftWall.addComponent(se4::Volume2d(0.0f, SCREEN_HEIGHT * 200));
    leftWall.addComponent(PhysicsBody(false, 1.0f, 0.0f, 1, se4::BodyType::RECTANGLE));

    rightWall.addComponent(se4::Position2d(SCREEN_WIDTH, 0.0f));
    rightWall.addComponent(se4::Volume2d(0.0f, SCREEN_HEIGHT * 2));
    rightWall.addComponent(PhysicsBody(false, 1.0f, 0.0f, 1, se4::BodyType::RECTANGLE));

    level->addUpdater(std::move(input_acc));
    level->init();
    return level;
}

int main(int argc, char *argv[]) {
    //For loading PNG images
    IMG_Init(IMG_INIT_PNG);
    google::InitGoogleLogging(argv[0]);

    se4window->show();
    se4::Game::Instance().windowList.push_back(se4window.get());


    //auto se4window2 = std::make_unique<se4::Window>("Title2", SCREEN_WIDTH, SCREEN_HEIGHT);
    //se4window2->show();
    //se4window2->setRenderLevel(1);
    //se4::Game::Instance().windowList.push_back(se4window2.get());
    //se4::Game::Instance().levelManager.setSharedData("id", std::string("rylynn6318"));
    //if (std::any_cast<std::string>(se4::Game::Instance().levelManager.getSharedData("id")) == "rylynn6318")
    //    LOG(ERROR) << "succes getSharedData";

    se4window->setRenderLevel(1);
    se4::Game::Instance().levelManager.addLevel(1, getLevel);
    se4::Game::Instance().levelManager.addLevel(2, getLevel2);
    se4::Game::Instance().levelManager.loadLevel(1);
    //se4::Game::Instance().levelManager.loadLevel(2);


    se4::Game::Instance().levelManager.setCurrentLevelID(1);


    se4::Game::Instance().isRunning = []() -> bool {
        return !se4::Game::Instance().inputManager.checkKey(se4::KeyState::PRESSED, se4::Key::ESC);
    };

    se4::Game::Instance().run();
    //For quitting IMG systems
    IMG_Quit();

    return 0;
}

