#ifndef SE4_SOUND_H
#define SE4_SOUND_H


#include "component/Component.hpp"
#include<Vector>

namespace se4 {
    struct Sound : public Component<Sound> {
        Sound(int soundID) : soundID(soundID) {}
    };

    int soundID;
}

#endif //SE4_SOUND_H
