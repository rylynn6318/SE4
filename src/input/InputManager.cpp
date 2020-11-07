//
// Created by ssm on 20. 10. 14..
//

#include "input/InputManager.h"

auto se4::InputManager::Keymap() -> std::map<Key, InputState> const {
    return keymap;
}
