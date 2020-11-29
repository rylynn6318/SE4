//
// Created by ssm on 20. 11. 11..
//

#include "input/Sdl2Input.h"
#include "input/Input.h"
#include "SDL.h"

auto se4::Input::saveKeymap(se4::Key key, se4::ButtonState state) -> void {
    // std::map []로 접근하면, 키가 없으면 만들고 있으면 그걸 씀
    // https://en.cppreference.com/w/cpp/container/map/operator_at
    auto& key_state = keymap[key];
    if (state == se4::ButtonState::DOWN) {
        switch (key_state) {
            case se4::KeyState::DEFAULT:
            case se4::KeyState::RELEASED:
                key_state = se4::KeyState::PRESSED;
                break;
            case se4::KeyState::PRESSED:
            case se4::KeyState::HELD_DOWN:
                key_state = se4::KeyState::HELD_DOWN;
                break;
        }
    } else if ( state == se4::ButtonState::UP) {
        switch (key_state) {
            case se4::KeyState::DEFAULT:
            case se4::KeyState::RELEASED:
                key_state = se4::KeyState::DEFAULT;
                break;
            case se4::KeyState::PRESSED:
            case se4::KeyState::HELD_DOWN:
                key_state = se4::KeyState::RELEASED;
                break;
        }
    }
}

se4::Input::Input() {
    for ( auto key : se4::Key()){
        keymap[key] = se4::KeyState::DEFAULT;
    }
}

auto se4::Input::toSE4Key(int keycode) -> se4::Key {
    switch (keycode) {
        case SDL_SCANCODE_A:
            return se4::Key::A;
        case SDL_SCANCODE_B:
            return se4::Key::B;
        case SDL_SCANCODE_C:
            return se4::Key::C;
        case SDL_SCANCODE_D:
            return se4::Key::D;
        case SDL_SCANCODE_E:
            return se4::Key::E;
        case SDL_SCANCODE_F:
            return se4::Key::F;
        case SDL_SCANCODE_G:
            return se4::Key::G;
        case SDL_SCANCODE_H:
            return se4::Key::H;
        case SDL_SCANCODE_I:
            return se4::Key::I;
        case SDL_SCANCODE_J:
            return se4::Key::J;
        case SDL_SCANCODE_K:
            return se4::Key::K;
        case SDL_SCANCODE_L:
            return se4::Key::L;
        case SDL_SCANCODE_M:
            return se4::Key::M;
        case SDL_SCANCODE_N:
            return se4::Key::N;
        case SDL_SCANCODE_O:
            return se4::Key::O;
        case SDL_SCANCODE_P:
            return se4::Key::P;
        case SDL_SCANCODE_Q:
            return se4::Key::Q;
        case SDL_SCANCODE_R:
            return se4::Key::R;
        case SDL_SCANCODE_S:
            return se4::Key::S;
        case SDL_SCANCODE_T:
            return se4::Key::T;
        case SDL_SCANCODE_U:
            return se4::Key::U;
        case SDL_SCANCODE_V:
            return se4::Key::V;
        case SDL_SCANCODE_W:
            return se4::Key::W;
        case SDL_SCANCODE_X:
            return se4::Key::X;
        case SDL_SCANCODE_Y:
            return se4::Key::Y;
        case SDL_SCANCODE_Z:
            return se4::Key::Z;
        case SDL_SCANCODE_ESCAPE:
            return se4::Key::ESC;
        default:
            return se4::Key::NONE;
    }
}

auto se4::Input::toButtonState(int state) -> se4::ButtonState {
    if (state)
        return se4::ButtonState::DOWN;
    else
        return se4::ButtonState::UP;
}

// TODO : 이런게 SDK_INPUT 필요없는건 Input.cpp 만들어서 거기 둬야할 듯
auto se4::Input::checkKey(se4::KeyState state, se4::Key key) -> bool {
    return keymap.at(key) == state;
}

template<typename... Keys>
auto se4::Input::checkKey(se4::KeyState state, se4::Key key, Keys... keys) -> bool {
    return checkKey(state, key) && checkKey(state, keys...);
}

auto se4::Input::saveKeymap(int keycode, int state) -> void {
    saveKeymap(toSE4Key(keycode), toButtonState(state));
}

auto se4::Input::pollKeyEvent() -> void {
    SDL_PumpEvents();
    Uint8 const *keystate = SDL_GetKeyboardState(nullptr);
    for (int keycode = 0; keycode < SDL_NUM_SCANCODES; ++keycode) {
        saveKeymap(keycode, keystate[keycode]);
    }

    saveMouseButtonToKeymap(SDL_GetMouseState(&mouse_x, &mouse_y));
}

auto se4::Input::getMousePosition() -> std::pair<int, int> {
    return {mouse_x, mouse_y};
}

auto se4::Input::saveMouseButtonToKeymap(int state) -> void {
    if (state & SDL_BUTTON(SDL_BUTTON_LEFT))
        saveKeymap(Key::MOUSE_1, toButtonState(state));
    else if (state & SDL_BUTTON(SDL_BUTTON_MIDDLE))
        saveKeymap(Key::MOUSE_2, toButtonState(state));
    else if (state & SDL_BUTTON(SDL_BUTTON_RIGHT))
        saveKeymap(Key::MOUSE_3, toButtonState(state));
    else if (state & SDL_BUTTON(SDL_BUTTON_X1))
        saveKeymap(Key::MOUSE_4, toButtonState(state));
    else if (state & SDL_BUTTON(SDL_BUTTON_X2))
        saveKeymap(Key::MOUSE_5, toButtonState(state));
}
