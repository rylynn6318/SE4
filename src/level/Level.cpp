#include <level/Level.hpp>
#include <iostream>
#include <entity/EntityHandle.hpp>

namespace se4 {
    Level::Level(std::unique_ptr<EntityManager> entityManager, std::any& windowContext) : entityManager(std::move(entityManager)) {
        auto r = std::make_unique<RenderUpdater>(windowContext);
        renderer = r.get();
        addUpdater(std::move(r));
    }

    void Level::init() {
        for (auto &updater : updaters) {
            updater->init();
        }
    }

    void Level::update(double dt) {
        // TODO(taurheim) check to make sure that the world has called init
        for (auto &updater : updaters) {
            updater->update(dt);
        }
    }

    // TODO : render, updater 분리
    void Level::render(int dt) {
        renderer->render(dt);
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
}  // namespace se4
