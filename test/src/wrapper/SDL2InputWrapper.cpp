//
// Created by ssm on 20. 10. 29..
//

#include "../../include/wrapper/SDL2InputWrapper.h"

auto SDL2InputWrapper::toSE4Key(int keycode) -> se4::Key {
    switch (keycode) {
        case SDLK_a:
            return se4::Key::A;
        case SDLK_b:
            return se4::Key::B;
        case SDLK_d:
            return se4::Key::D;
        case SDLK_w:
            return se4::Key::W;
        case SDLK_s:
            return se4::Key::S;
        default:
            return se4::Key::NONE;
    }
}

auto SDL2InputWrapper::toButtonState(int state) -> se4::ButtonState {
    switch (state) {
        case SDL_KEYDOWN:
            return se4::ButtonState::DOWN;
        case SDL_KEYUP:
            return se4::ButtonState::UP;
        default:
            return se4::ButtonState::NONE;
    }
}

auto SDL2InputWrapper::pollInput() -> void {
    SDL_Event input;
    SDL_PollEvent(&input);

    if(input.type == SDL_QUIT)
        quit = true;

    saveToKeymap(toSE4Key(input.key.keysym.sym), toButtonState(input.type));
}

auto SDL2InputWrapper::registerKeyEvent() -> void {

}
