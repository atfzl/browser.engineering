#include <SDL2/SDL.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);

    SDL_Window* win =
        SDL_CreateWindow("Window Title", SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);

    SDL_Event e;
    int quit = 0;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
        }
        SDL_Delay(16);
    }

    SDL_DestroyWindow(win);
    win = NULL;

    SDL_Quit();

    return EXIT_SUCCESS;
}
