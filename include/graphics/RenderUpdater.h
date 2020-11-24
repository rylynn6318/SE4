//
// Created by ssm on 20. 11. 12..
//

#ifndef SE4_RENDERUPDATER_H
#define SE4_RENDERUPDATER_H

#include "EngineConfig.h"
#include "updater/Updater.hpp"
#include SDK_RENDERER_H

namespace se4 {
    class Window;

    class RenderUpdater : public se4::Updater, public SDK_RENDERER {
    public:
        explicit RenderUpdater();
        ~RenderUpdater() override;

        auto init() -> bool override;

        auto render(WindowID, int time) -> void;

        // todo : SDL_Rect 말고 다른걸로 변경
        auto getCamViewprot() -> SDL_Rect;

        static auto createRenderContext(Window*) -> void;
    private:
        //아래 두 변수는 추후 사용자의 입력을 받아서 초기화되도록 바꿔야됨
        int winMinWidth = 1920;
        float padding = 0.03;
    };
}

#endif //SE4_RENDERUPDATER_H
