#include <algorithm>
#include <updater/Updater.hpp>

namespace se4 {
    void Updater::registerWorld(Level *world) { parentWorld = world; }

    void Updater::registerEntity(se4::Entity const &entity) { registeredEntities.push_back(entity); }

    void Updater::unRegisterEntity(se4::Entity const &entity) {
        registeredEntities.erase(std::ranges::find(registeredEntities, entity));
    }

    ComponentMask &Updater::getSignature() { return signature; }
}  
