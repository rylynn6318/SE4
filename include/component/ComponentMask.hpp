#pragma once
#include "Component.hpp"

namespace se4 {
    struct ComponentMask {
        unsigned mask[32] = {};

        template <typename ComponentType>
        void addComponent() {
            auto family = GetComponentFamily<ComponentType>();
            mask[family / 32] |= (1 << family % 32);
        }

        template <typename ComponentType>
        void removeComponent() {
            auto family = GetComponentFamily<ComponentType>();
            mask[family / 32] &= ~(1 << family % 32);
        }

        bool isNewMatch(ComponentMask& oldMask, ComponentMask& systemMask);

        bool isNoLongerMatched(ComponentMask& oldMask, ComponentMask& systemMask);

        bool matches(ComponentMask& systemMask);
    };
}  // namespace se4
