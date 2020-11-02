#include <updater/Updater.hpp>

namespace se4 {
    void Updater::registerWorld(World* world) { parentWorld = world; }

    void Updater::registerEntity(se4::Entity const& entity) { registeredEntities.push_back(entity); }

    void Updater::unRegisterEntity(se4::Entity const& entity) {
        for (auto it = registeredEntities.begin(); it != registeredEntities.end(); ++it) {
            Entity e = *it;
            if (e.id == entity.id) {
                registeredEntities.erase(it);
                return;
            }
        }
    }

    ComponentMask& Updater::getSignature() { return signature; }
}  
