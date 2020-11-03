#pragma once
#include <memory>
#include "component/Component.hpp"
#include "component/ComponentHandle.hpp"
#include "component/ComponentManager.hpp"
#include "component/ComponentMask.hpp"
#include "entity/Entity.hpp"
#include "entity/EntityManager.hpp"
#include "updater/Updater.hpp"

namespace se4 {
    struct EntityHandle;
    class World {
    public:
        explicit World(std::unique_ptr<EntityManager> entityManager);

        /*
         * Should be called before the first update, but after instantiation
         */
        void init();

        /*
         * Update game logic. This is unrelated to a frame
         * https://gafferongames.com/post/fix_your_timestep/
         */
        void update(double dt);
        /*
         * Render a frame.
         */
        void render();
        EntityHandle createEntity();
        void addUpdater(std::unique_ptr<Updater> updater);
        void destroyEntity(Entity entity);

        /*
         * This is only necessary for bridge component managers.
         * TODO write blog post on bridge component managers
         */
        template <typename ComponentType>
        void addCustomComponentManager(std::unique_ptr<ComponentManager<ComponentType>> manager) {
            int family = GetComponentFamily<ComponentType>();
            if (family >= componentManagers.size()) {
                componentManagers.resize(family + 1);
            }
            componentManagers[family] = manager;
        }

        template <typename ComponentType>
        void addComponent(Entity const& entity, ComponentType&& component) {
            ComponentManager<ComponentType>* manager = getComponentManager<ComponentType>();
            manager->addComponent(entity, component);

            ComponentMask oldMask = entityMasks[entity];
            entityMasks[entity].addComponent<ComponentType>();

            updateEntityMask(entity, oldMask);
        }

        template <typename ComponentType>
        void removeComponent(Entity const& entity) {
            ComponentManager<ComponentType>* manager = getComponentManager<ComponentType>();
            ComponentHandle<ComponentType> component = manager->lookup(entity);
            component.destroy();

            ComponentMask oldMask = entityMasks[entity];
            entityMasks[entity].removeComponent<ComponentType>();

            updateEntityMask(entity, oldMask);
        }

        template <typename ComponentType, typename... Args>
        void unpack(Entity e, ComponentHandle<ComponentType>& handle, ComponentHandle<Args> &... args) {
            typedef ComponentManager<ComponentType> ComponentManagerType;
            auto mgr = getComponentManager<ComponentType>();
            handle = ComponentHandle<ComponentType>(e, mgr->lookup(e), mgr);

            // Recurse
            unpack<Args...>(e, args...);
        }

        // Base case
        template <typename ComponentType>
        void unpack(Entity e, ComponentHandle<ComponentType>& handle) {
            typedef ComponentManager<ComponentType> ComponentManagerType;
            auto mgr = getComponentManager<ComponentType>();
            handle = ComponentHandle<ComponentType>(e, mgr->lookup(e), mgr);
        }

    private:
        std::unique_ptr<EntityManager> entityManager;
        std::vector<std::unique_ptr<Updater>> updaters;
        std::vector<std::unique_ptr<BaseComponentManager>> componentManagers;
        std::map<Entity, ComponentMask> entityMasks;

        void updateEntityMask(Entity const& entity, ComponentMask& oldMask);

        template <typename ComponentType>
        ComponentManager<ComponentType>* getComponentManager() {
            // Need to make sure we actually have a component manager.
            // TODO(taurheim) this is a performance hit every time we add and remove a component
            int family = GetComponentFamily<ComponentType>();

            if (family >= componentManagers.size()) {
                componentManagers.resize(family + 1);
            }

            if (!componentManagers[family]) {
                componentManagers[family] = std::make_unique<ComponentManager<ComponentType>>();
            }

            return static_cast<ComponentManager<ComponentType>*>(componentManagers[family].get());
        }
    };
}  // namespace se4
