#include <SDL.h>
#include <cstdio>

// Screen (rendering/display surface) dimensions
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main()
{
    SDL_Window* window = nullptr;
    SDL_Surface* screen = nullptr;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    } else {
        window = SDL_CreateWindow("SDL Tutorial",
                                  SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                  SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN
        );
        if (window == nullptr) {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        } else {
            // Go fullscreen
            //SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
            screen = SDL_GetWindowSurface(window);

            SDL_FillRect(screen, nullptr,
                         SDL_MapRGB(screen->format, 0x00, 0x00, 0x00)
            );
            SDL_UpdateWindowSurface(window);
            SDL_Delay(10000);
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}