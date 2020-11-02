//
// Created by ssm on 20. 10. 29..
//

#ifndef SE4_SDL2INPUTWRAPPER_H
#define SE4_SDL2INPUTWRAPPER_H

#include <SDL.h>

// TODO : 인터페이스 상속
class SDL2InputWrapper {
private:
    // keymap

public:
    // Poll input events from window and save key event to keymap
    void pollInput(); //override

};

#endif //SE4_SDL2INPUTWRAPPER_H
