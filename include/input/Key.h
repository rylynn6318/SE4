#ifndef SE4_KEY_H
#define SE4_KEY_H

namespace se4 {
    enum class Key {
        NONE,
        ESC,
        A,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
        I,
        J,
        K,
        L,
        M,
        N,
        O,
        P,
        Q,
        R,
        S,
        T,
        U,
        V,
        W,
        X,
        Y,
        Z,

        // for iter
        FIRST = NONE,
        LAST = Z
    };

    auto operator++(Key &x) -> Key;

    auto operator*(Key k) -> Key;

    auto begin(Key) -> Key;

    auto end(Key) -> Key;
}

#endif