//
// Created by ssm on 20. 10. 14..
//

#ifndef SE4_INPUTMANAGER_H
#define SE4_INPUTMANAGER_H

namespace se4 {
    // 윈도우 하나당 하나의 InputManager가 존재할 수 있다.
    // InputManager는 Window의 키 입력을 감지하여 멤버변수 키맵에 저장하는 함수 checkKey(가칭)을 제공한다.
    // 그 과정에서 윈도우에서 제공하는 키와 이벤트 타입을 se4::Key, InputState 등으로 전환한다.
    // 키 입력과 실제 수행할 작업은 BindAction으로 연결해서 컨테이너로 관리한다
    // InputManager는 현재 키맵을 보고 조건에 맞는 BindAction을 실행하는 함수 processInput(가칭)을 제공한다.
    class InputManager {

    };
}

#endif //SE4_INPUTMANAGER_H
