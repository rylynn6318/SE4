#pragma once
#include "component/ComponentHandle.hpp"
#include "Entity.hpp"
#include "world/World.hpp"

namespace se4 {
    struct EntityHandle {
        Entity entity;
        World* world;
        
        void destroy() { world->destroyEntity(entity); }

        template <is_component ComponentType>
        void addComponent(ComponentType&& component) {
            world->addComponent<ComponentType>(entity, std::forward<ComponentType>(component));
        }

        template <is_component ComponentType>
        void removeComponent() {
            world->removeComponent<ComponentType>(entity);
        }

        template <is_component ComponentType>
        ComponentHandle<ComponentType> getComponent() {
            ComponentHandle<ComponentType> handle;
            world->unpack(entity, handle);
            return handle;
        }
    };
}  // namespace se4