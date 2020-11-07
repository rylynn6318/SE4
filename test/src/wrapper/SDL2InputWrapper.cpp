//
// Created by ssm on 20. 10. 29..
//

#include "../../include/wrapper/SDL2InputWrapper.h"

auto SDL2InputWrapper::saveToKeymap(se4::Key key, se4::InputState state) {
    auto last_state = keymap[key];
    if (last_state == se4::InputState::DEFAULT)
        keymap[key] = state == se4::InputState::NONE ? se4::InputState::DEFAULT : state;
    if (last_state == se4::InputState::PRESSED)
        keymap[key] = state == se4::InputState::PRESSED ? se4::InputState::HELD_DOWN : state;
    if (last_state == se4::InputState::HELD_DOWN)
        keymap[key] = state == se4::InputState::PRESSED ? se4::InputState::HELD_DOWN : state;
    if (last_state == se4::InputState::RELEASED)
        keymap[key] = state == se4::InputState::RELEASED ? se4::InputState::DEFAULT : state;
}

auto SDL2InputWrapper::toSE4Key(int keycode) -> se4::Key {
    switch (keycode) {
        case SDLK_a:
            return se4::Key::A;
        case SDLK_b:
            return se4::Key::B;
        case SDLK_d:
            return se4::Key::D;
        default:
            return se4::Key::NONE;
    }
}

auto SDL2InputWrapper::toSE4State(int state) -> se4::InputState {
    switch (state) {
        case SDL_KEYDOWN:
            return se4::InputState::PRESSED;
        case SDL_KEYUP:
            return se4::InputState::RELEASED;
        default:
            return se4::InputState::NONE;
    }
}

auto SDL2InputWrapper::pollInput() -> void {
    SDL_Event input;
    SDL_PollEvent(&input);

    if(input.type == SDL_QUIT)
        quit = true;

    saveToKeymap(toSE4Key(input.key.keysym.sym), toSE4State(input.type));
}

auto SDL2InputWrapper::registerKeyEvent() -> void {

}
