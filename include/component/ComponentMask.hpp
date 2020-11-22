#pragma once

#include <bitset>

#include "Component.hpp"

namespace se4 {
    struct ComponentMask {
        std::bitset<MAX_COMPONENT_FAMILY> mask;

        template<is_component ComponentType>
        void addComponent() {
            auto family = GetComponentFamily<ComponentType>();
            mask.set(family);
        }

        template<is_component ComponentType>
        void removeComponent() {
            auto family = GetComponentFamily<ComponentType>();
            mask.reset(family);
        }

        [[nodiscard]]
        bool isNewMatch(const ComponentMask &oldMask, const ComponentMask &systemMask) const;

        [[nodiscard]]
        bool isNoLongerMatched(const ComponentMask &oldMask, const ComponentMask &systemMask) const;

        [[nodiscard]]
        bool matches(ComponentMask const &systemMask) const;
    };
}  // namespace se4
