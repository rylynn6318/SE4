//
// Created by ssm on 20. 10. 14..
//

#ifndef SE4_INPUTSTATE_H
#define SE4_INPUTSTATE_H

namespace se4 {
    enum class InputState {
        PRESSED,
        눌리는중, // TODO : 네이밍 추천좀
        RELEASED,
        기본상태
    };
}

#endif //SE4_INPUTSTATE_H
