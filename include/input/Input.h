//
// Created by ssm on 20. 11. 7..
//

#ifndef SE4_INPUT_H
#define SE4_INPUT_H

#include "EngineConfig.h"

#include <map>
#include "Key.h"
#include "KeyState.h"
#include "ButtonState.h"
#include SDK_INPUT_H

namespace se4 {
    class Input : SDK_INPUT {
    private:
        std::map<Key, KeyState> keymap;

    protected:
        static auto toSE4Key(int keycode) -> Key;

        static auto toButtonState(int state) -> ButtonState;

        auto saveToKeymap(Key key, ButtonState state) -> void;

    public:
        Input();

        auto checkKey(KeyState state, Key key) -> bool;
        template<typename... Keys>
        auto checkKey(KeyState state, Key key, Keys ...keys) -> bool;

        auto saveKeymap(int keycode, int state) -> void;
    };
}

#endif //SE4_INPUT_H
