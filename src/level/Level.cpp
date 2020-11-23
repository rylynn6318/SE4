#include <level/Level.hpp>
#include <iostream>
#include <entity/EntityHandle.hpp>

namespace se4 {
    Level::Level() : entityManager(std::make_unique<se4::EntityManager>()) {
        auto r = std::make_unique<RenderUpdater>();
        renderer = r.get();
        addUpdater(std::move(r));
    }

    void Level::init() {
        for (auto &updater : updaters) {
            updateUpdaterMask(updater.get());
            updater->init();
        }
    }

    void Level::update(double dt) {
        for (auto &updater : updaters) {
            updater->update(dt);
        }
    }

    void Level::render(WindowID id, int dt) {
        renderer->render(id, dt);
    }

    EntityHandle Level::createEntity() {
        return {entityManager->createEntity(), this};
    }

    void Level::destroyEntity(se4::Entity entity) {
        for (auto &updater : updaters) {
            updater->unRegisterEntity(entity);
        }

        entityManager->destroy(entity);
    }

    void Level::addUpdater(std::unique_ptr<se4::Updater> updater) {
        updater->registerWorld(this);
        updaters.push_back(std::move(updater));
    }

    void Level::updateEntityMask(se4::Entity const& entity, se4::ComponentMask& oldMask) {
        ComponentMask& newMask = entityMasks[entity];

        for (auto& updater : updaters) {
            ComponentMask& updaterSignature = updater->getSignature();
            if (newMask.isNewMatch(oldMask, updaterSignature)) {
                // We match but didn't match before
                updater->registerEntity(entity);
            } else if (newMask.isNoLongerMatched(oldMask, updaterSignature)) {
                updater->unRegisterEntity(entity);
            }
        }
    }

    auto Level::updateUpdaterMask(Updater *updater) -> void {
        ComponentMask& updaterSignature = updater->getSignature();

        for (auto const & [entity, mask] : entityMasks){
            if(mask.matches(updaterSignature)) {
                updater->registerEntity(entity);
            }
        }
    }

    auto Level::createRenderContext(Window * win) -> void {
        renderer->createRenderContext(win);
    }
}  // namespace se4
