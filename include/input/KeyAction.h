//
// Created by ssm on 20. 10. 14..
//

#ifndef SE4_KEYACTION_H
#define SE4_KEYACTION_H

#include <functional>

#include "Key.h"
#include "InputState.h"

namespace se4 {
    // TODO : Callable 한 객체를 모두 받을 수 있게 할것
    // 파라메터 : 키와 상태, 그리고 실행할 함수, 혹은
    //           조건(함수)와 실행할 함수
    struct KeyAction {
        Key key;
        // TODO : 조건을 발동시킬 함수 혹은 enum을 받아야 할

        // void 함수 포인터는 선넘은것 같
        void (*callback)(void);

        // auto callable(void) -> void (*)(void);

        KeyAction(Key key, void (*callback)(void))
                : key(key), callback(callback) {}

    };
}

#endif //SE4_KEYACTION_H
