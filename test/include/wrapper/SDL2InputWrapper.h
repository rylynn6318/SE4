//
// Created by ssm on 20. 10. 29..
//

#ifndef SE4_SDL2INPUTWRAPPER_H
#define SE4_SDL2INPUTWRAPPER_H

#include <SDL.h>
#include "input/InputManager.h"
#include "input/KeyInput.h"

class SDL2InputWrapper : public se4::InputManager, public se4::KeyInput {
private:
    auto toSE4Key(int keycode) -> se4::Key override;
    auto toButtonState(int state) -> se4::ButtonState override;

public:
    // Poll input events from window and save key event to keymap
    auto pollInput() -> void override; //override
    auto registerKeyEvent() -> void;

    // TODO : 기본적으로 InputManager에는 윈도우 관련 이벤트만 넣고 적절히 처리
    // quit 을 비롯한 것들 일단은 public에 박음
    bool quit = false;
};

#endif //SE4_SDL2INPUTWRAPPER_H
