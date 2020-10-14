//
// Created by ssm on 20. 10. 14..
//

#ifndef SE4_INPUTEVENTTYPE_H
#define SE4_INPUTEVENTTYPE_H

namespace se4{
    // 창 관리자를 통해 받게 되는 입력 이벤트의 타입(상태)
    // 이것을 InputState 로 변환해서 관리함.
    // 키보드, 마우스 클릭, 그외 버튼 종류의 입력만 가능함.
    // TODO : 마우스 좌표와 조이스틱 등의 입력도 처리 가능해야함
    enum class InputEventType {
        NONE,
        DOWN,
        UP
    };
}

#endif //SE4_INPUTEVENTTYPE_H
