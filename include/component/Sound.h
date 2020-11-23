 #ifndef SE4_SOUND_H
#define SE4_SOUND_H


#include "component/Component.hpp"
#include <queue>

namespace se4 {
    struct Sound : public Component<Sound> {
        Sound()
        {
        };

        void play(const char* soundId)
        {
            ids.push(soundId);
        }

        std::queue<const char*> ids;
    };
}

#endif //SE4_SOUND_H
