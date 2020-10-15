#pragma once
#include "Component.hpp"

namespace se4 {
    struct ComponentMask {
        unsigned int mask = 0;

        template <typename ComponentType>
        void addComponent() {
            mask |= (1 << GetComponentFamily<ComponentType>());
        }

        template <typename ComponentType>
        void removeComponent() {
            mask &= ~(1 << GetComponentFamily<ComponentType>());
        }

        bool isNewMatch(ComponentMask oldMask, ComponentMask systemMask);

        bool isNoLongerMatched(ComponentMask oldMask, ComponentMask systemMask);

        bool matches(ComponentMask systemMask);
    };
}  // namespace se4
