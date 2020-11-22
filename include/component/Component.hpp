#pragma once

#include <concepts>
#include <cassert>

namespace se4 {
    constexpr int MAX_COMPONENT_FAMILY = 1024;

    struct ComponentCounter {
        static int familyCounter;
    };

    template<typename ComponentType>
    struct Component {
        static inline int family() {
            static int const family = ComponentCounter::familyCounter++;
            assert(family <= MAX_COMPONENT_FAMILY);
            return family;
        }
    };

    // Util method for getting family given type
    template<typename C>
    static int GetComponentFamily() {
        return Component<typename std::remove_const<C>::type>::family();
    }

    template<class T>
    concept is_component = std::derived_from<T, Component<T>>;
}  // namespace se4
