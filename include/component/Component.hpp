#pragma once
#include <type_traits>
#include "ComponentManager.hpp"
#include "entity/Entity.hpp"

namespace se4 {
    struct ComponentCounter {
        static int familyCounter;
    };

    template <typename ComponentType>
    struct Component {
        static inline int family() {
            static int family = ComponentCounter::familyCounter++;
            return family;
        }
    };

    // Util method for getting family given type
    template <typename C>
    static int GetComponentFamily() {
        return Component<typename std::remove_const<C>::type>::family();
    }
}  // namespace se4
