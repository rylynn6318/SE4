//
// Created by ssm on 20. 10. 14..
//

#ifndef SE4_KEYACTION_H
#define SE4_KEYACTION_H

#include <functional>
#include "entity/Entity.hpp"
#include "Key.h"
#include "KeyState.h"

namespace se4 {
    // TODO : Callable 한 객체를 모두 받을 수 있게 할것
    // 파라메터 : 키와 상태, 그리고 실행할 함수, 혹은
    //           조건(함수)와 실행할 함수
    struct KeyAction {
        // Map으로 만드는게 나을까?
        Key key;
        // TODO : 조건을 발동시킬 함수 혹은 enum을 받아야 할

        // void 함수 포인터는 선넘은것 같
        // void (*callback)(Entity entity);
        // auto getCallable(void) -> void (*)(void);
        std::function<void(Entity)> callback;

        KeyAction(Key key, void (*callback)())
                : key(key), callback(reinterpret_cast<void (*)(Entity)>(callback)) {}

    };
}

#endif //SE4_KEYACTION_H
