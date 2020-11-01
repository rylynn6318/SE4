#include <world/World.hpp>
#include <iostream>
#include <entity/EntityHandle.hpp>

namespace se4 {
    World::World(std::unique_ptr<EntityManager> entityManager) : entityManager(std::move(entityManager)) {}

    void World::init() {
        for (auto& updater : updaters) {
            updater->init();
        }
    }

    void World::update(double dt) {
        // TODO(taurheim) check to make sure that the world has called init
        for (auto& updater : updaters) {
            updater->update(dt);
        }
    }

    void World::render() {
        for (auto& updater : updaters) {
            updater->render();
        }
    }

    EntityHandle World::createEntity() { return { entityManager->createEntity(), this }; }

    void World::destroyEntity(se4::Entity entity) {
        for (auto& updater : updaters) {
            updater->unRegisterEntity(entity);
        }

        entityManager->destroy(entity);
    }
    
    void World::addUpdater(std::unique_ptr<se4::Updater> updater) {
        updater->registerWorld(this);
        updaters.push_back(std::move(updater));
    }

    void World::updateEntityMask(se4::Entity const& entity, se4::ComponentMask& oldMask) {
        ComponentMask& newMask = entityMasks[entity];

        for (auto& updater : updaters) {
            ComponentMask& updaterSignature = updater->getSignature();
            if (newMask.isNewMatch(oldMask, updaterSignature)) {
                // We match but didn't match before
                updater->registerEntity(entity);
            }
            else if (newMask.isNoLongerMatched(oldMask, updaterSignature)) {
                updater->unRegisterEntity(entity);
            }
        }
    }
}  // namespace se4
