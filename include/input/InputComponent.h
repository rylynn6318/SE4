//
// Created by ssm on 20. 10. 29..
//

#ifndef SE4_INPUTCOMPONENT_H
#define SE4_INPUTCOMPONENT_H

#include "component/Component.hpp"

namespace se4 {
    class InputComponent : public Component<InputComponent> {
    private:
        bool is_selected;

    public:
        auto isSelected() const -> bool;
    };
}

#endif //SE4_INPUTCOMPONENT_H
