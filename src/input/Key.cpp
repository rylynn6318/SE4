//
// Created by ssm on 20. 11. 7..
//

#include <type_traits>
#include "input/Key.h"

auto se4::operator++(se4::Key &x) -> se4::Key { return x = (Key) (std::underlying_type<Key>::type(x) + 1); }

auto se4::operator*(se4::Key k) -> se4::Key { return k; }

auto se4::begin(se4::Key) -> se4::Key { return Key::FIRST; }

auto se4::end(se4::Key) -> se4::Key {
    Key l = Key::LAST;
    return ++l;
}
