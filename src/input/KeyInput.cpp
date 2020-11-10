//
// Created by ssm on 20. 11. 7..
//

#include "input/KeyInput.h"

auto se4::KeyInput::saveToKeymap(se4::Key key, se4::ButtonState state) -> void {
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

auto se4::KeyInput::Keymap() const -> std::map<Key, KeyState> const & {
    return keymap;
}

se4::KeyInput::KeyInput() {
    for ( auto key : se4::Key()){
        keymap[key] = se4::KeyState::DEFAULT;
    }
}
