//
// Created by ssm on 20. 11. 13..
//

#ifndef SE4_VOLUME2D_H
#define SE4_VOLUME2D_H


#include "component/Component.hpp"

namespace se4 {
    struct Volume2d : public Component<Volume2d> {
        Volume2d(double w, double h) : width(w), height(h) {}

        double width, height;
    };
}

#endif //SE4_VOLUME2D_H
