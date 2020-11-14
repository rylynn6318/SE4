//
// Created by ssm on 20. 11. 13..
//

#ifndef SE4_POSITION2D_H
#define SE4_POSITION2D_H


#include "component/Component.hpp"

namespace se4 {
    struct Position2d : public Component<Position2d> {
        Position2d(double x, double y) : x(x), y(y) {}

        double x, y;
    };
}

#endif //SE4_POSITION2D_H
