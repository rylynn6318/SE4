//
// Created by ssm on 20. 11. 7..
//

#ifndef SE4_KEYINPUT_H
#define SE4_KEYINPUT_H

#include <map>
#include "Key.h"
#include "KeyState.h"
#include "ButtonState.h"

namespace se4 {
    class KeyInput {
    private:
        std::map<Key, KeyState> keymap;

    protected:
        virtual auto toSE4Key(int keycode) -> Key = 0;

        virtual auto toButtonState(int state) -> ButtonState = 0;

        auto saveToKeymap(Key key, ButtonState state) -> void;

    public:
        KeyInput();

        [[nodiscard]]
        auto Keymap() const -> std::map<Key, KeyState> const &;

        auto saveKeymap(int const keycode, int const state) {
            saveToKeymap(toSE4Key(keycode), toButtonState(state));
        }
    };
}

#endif //SE4_KEYINPUT_H
