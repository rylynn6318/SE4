//
// Created by ssm on 20. 10. 14..
//

#ifndef SE4_INPUTSTATE_H
#define SE4_INPUTSTATE_H

namespace se4 {
    enum class InputState {
        DEFAULT,
        PRESSED,
        HELD_DOWN,
        RELEASED,
        NONE // null
    };
}

#endif //SE4_INPUTSTATE_H
