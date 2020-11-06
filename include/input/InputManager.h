//
// Created by ssm on 20. 10. 14..
//

#ifndef SE4_INPUTMANAGER_H
#define SE4_INPUTMANAGER_H

#include <map>
#include "Key.h"
#include "InputState.h"

namespace se4 {
    // InputManager : InputManager 하나에 윈도우 1개 이상이 올 수 있다. 아님 싱글톤인가?
    // InputManager는 Window의 키 입력을 감지하여 멤버변수 키맵에 저장하는 함수 pollEvent(가칭)을 제공한다.
    // 그 과정에서 윈도우에서 제공하는 키와 이벤트 타입을 se4::Key, InputState 등으로 전환한다.
    // 키 입력과 실제 수행할 작업은 BindAction으로 연결해서 컨테이너로 관리한다
    // InputManager는 현재 키맵을 보고 조건에 맞는 BindAction을 실행하는 함수 processInput(가칭)을 제공한다.
    class InputManager {
    protected:
        // 입력 맵(데이터)은 외부에 구조체나 뭐 그런걸로 빼고 인터페이스만 남기는게 나을 듯
        std::map<Key, InputState> keymap; // on-off 상태가 있는 입력
        // std::map<KeyType, InputData> keymap2; // 데이터를 가지는 입력

    public:
        auto Keymap() -> std::map<Key, InputState> const;

        virtual auto pollInput() -> void = 0;
    };
}

#endif //SE4_INPUTMANAGER_H
