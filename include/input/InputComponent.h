//
// Created by ssm on 20. 10. 29..
//

#ifndef SE4_INPUTCOMPONENT_H
#define SE4_INPUTCOMPONENT_H

#include "component/Component.hpp"

namespace se4 {
    struct InputComponent : public Component<InputComponent> {
        InputComponent() = delete;
        explicit InputComponent(bool isSelected);

        bool is_selected;
    };
}

#endif //SE4_INPUTCOMPONENT_H
