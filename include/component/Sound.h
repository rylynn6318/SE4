 #ifndef SE4_SOUND_H
#define SE4_SOUND_H


#include "component/Component.hpp"
#include <queue>

namespace se4 {
    struct SoundRequest
    {
        const char* soundId;
        const char* groupId;
        bool isLoop;
    };

    struct Sound : public Component<Sound> {
        Sound()
        {
        };

        void play(const char* soundId, const char* groupId, bool isLoop = false)
        {
            ids.push({ soundId, groupId, isLoop });
        }

        std::queue <SoundRequest> ids;
    };
}

#endif //SE4_SOUND_H
