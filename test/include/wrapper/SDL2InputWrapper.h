//
// Created by ssm on 20. 10. 29..
//

#ifndef SE4_SDL2INPUTWRAPPER_H
#define SE4_SDL2INPUTWRAPPER_H

#include <SDL.h>
#include "input/InputManager.h"

// TODO : 인터페이스 상속
class SDL2InputWrapper : public se4::InputManager {
private:

    // keymap
    auto saveToKeymap(se4::Key key, se4::InputState state);
    static auto toSE4Key(int keycode) -> se4::Key;
    static auto toSE4State(int state) -> se4::InputState;

public:
    // Poll input events from window and save key event to keymap
    auto pollInput() -> void override; //override
    auto registerKeyEvent() -> void;

    // 기본적으로 InputManager에는 윈도우 관련 이벤트만 넣고 적절히 처리
    bool quit;
};

#endif //SE4_SDL2INPUTWRAPPER_H
