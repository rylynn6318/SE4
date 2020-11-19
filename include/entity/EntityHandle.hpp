#pragma once
#include "component/ComponentHandle.hpp"
#include "Entity.hpp"
#include "level/Level.hpp"

namespace se4 {
    struct EntityHandle {
        Entity entity;
        Level* level;
        
        void destroy() { level->destroyEntity(entity); }

        template <is_component ComponentType>
        void addComponent(ComponentType&& component) {
            level->addComponent<ComponentType>(entity, std::forward<ComponentType>(component));
        }

        template <is_component ComponentType>
        void removeComponent() {
            level->removeComponent<ComponentType>(entity);
        }

        template <is_component ComponentType>
        ComponentHandle<ComponentType> getComponent() {
            ComponentHandle<ComponentType> handle;
            level->unpack(entity, handle);
            return handle;
        }
    };
}  // namespace se4