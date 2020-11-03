//
// Created by ssm on 20. 10. 31..
//

#ifndef SE4_GAME_H
#define SE4_GAME_H

namespace se4 {
    class Game {
    private:
        // TODO : 멤버 변수, 혹은 서비스로 InputManager, FileIO, AudioSystem, GameConfig 등 게임 전체에 하나만 존재하는 객체들 : 싱글톤 아님!

    public:
        auto run() -> void;
        auto stop() -> void;
    };
}

#endif //SE4_GAME_H
