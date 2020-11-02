#pragma once
#include <array>
#include <map>
#include <cstdlib>
#include "entity/Entity.hpp"
#include "entity/EntityMap.hpp"

namespace se4 {
    template <typename ComponentType>
    struct ComponentData {
        unsigned int size = 1;
        std::array<ComponentType, MAX_NUMBER_OF_COMPONENTS>* data;
    };

    class BaseComponentManager {
    public:
        BaseComponentManager() = default;
        virtual ~BaseComponentManager() = default;
        BaseComponentManager(const BaseComponentManager&) = default;
        BaseComponentManager& operator=(const BaseComponentManager&) = default;
        BaseComponentManager(BaseComponentManager&&) = default;
        BaseComponentManager& operator=(BaseComponentManager&&) = default;
    };

    template <typename ComponentType>
    class ComponentManager : public BaseComponentManager {
    public:
        using LookupType = ComponentType;

        ComponentManager() {
            componentData.data = static_cast<std::array<ComponentType, 1024>*>(malloc(sizeof(ComponentType) * 1024));
        }
        ComponentInstance addComponent(Entity entity, ComponentType& component) {
            ComponentInstance newInstance = componentData.size;
            componentData.data->at(newInstance) = component;
            entityMap.add(entity, newInstance);
            componentData.size++;
            return newInstance;
        }

        void destroyComponent(Entity entity) {
            // TODO(taurheim) write a blog post about delayed garbage collection
            // For now, there's a bug with concurrency
            ComponentInstance instance = entityMap.getInstance(entity);

            // Move last component to the deleted position to maintain data coherence
            ComponentInstance lastComponent = componentData.size - 1;
            componentData.data[instance] = componentData.data[lastComponent];
            Entity lastEntity = entityMap.getEntity(lastComponent);

            // Update our map
            entityMap.remove(entity);
            entityMap.update(lastEntity, instance);

            componentData.size--;
        }

        LookupType* lookup(Entity entity) {
            ComponentInstance instance = entityMap.getInstance(entity);
            return &componentData.data->at(instance);
        }

    private:
        ComponentData<ComponentType> componentData;
        EntityMap entityMap;
    };
}  // namespace se4
