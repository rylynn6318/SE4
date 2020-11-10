//
// Created by ssm on 20. 11. 7..
//

#ifndef SE4_GAMECONFIG_H
#define SE4_GAMECONFIG_H

namespace se4 {
    struct GameConfig {
        GameConfig(int width, int height, int max_fps, std::string_view title)
                : WINDOW_WIDTH(width),
                  WINDOW_HEIGHT(height),
                  MAX_FPS(max_fps),
                  TITLE_NAME(title) {

        }

        int WINDOW_WIDTH;
        int WINDOW_HEIGHT;
        int MAX_FPS;
        std::string TITLE_NAME;
    };
}

#endif //SE4_GAMECONFIG_H
