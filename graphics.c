#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>

// Define MAX and MIN macros
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

// Define screen dimensions
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define FONT_PATH "JetBrainsMono-Regular.ttf"

int main(int argc, char *argv[]) {
    // Initialize SDL2
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf(
            "SDL2 could not be initialized!\n"
            "SDL2 Error: %s\n",
            SDL_GetError());
        return 1;
    }

    // Initialize SDL2_ttf
    TTF_Init();

    // Create window
    SDL_Window *window = SDL_CreateWindow(
        "SDL2_ttf sample", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        printf(
            "Window could not be created!\n"
            "SDL_Error: %s\n",
            SDL_GetError());
        return 1;
    }

    // Create renderer
    SDL_Renderer *renderer =
        SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf(
            "Renderer could not be created!\n"
            "SDL_Error: %s\n",
            SDL_GetError());
    }

    TTF_Font *font = TTF_OpenFont(FONT_PATH, 40);
    if (!font) {
        printf(
            "Unable to load font: '%s'!\n"
            "SDL2_ttf Error: %s\n",
            FONT_PATH, TTF_GetError());
        return 1;
    }

    SDL_Color textColor = {0x00, 0x00, 0x00, 0xFF};
    SDL_Color textBackgroundColor = {0xFF, 0xFF, 0xFF, 0xFF};

    SDL_Surface *textSurface = TTF_RenderText_Shaded(
        font, "Red square", textColor, textBackgroundColor);
    if (!textSurface) {
        printf(
            "Unable to render text surface!\n"
            "SDL2_ttf Error: %s\n",
            TTF_GetError());
        return 1;
    }

    // Create texture from surface pixels
    SDL_Texture *text = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!text) {
        printf(
            "Unable to create texture from rendered text!\n"
            "SDL2 Error: %s\n",
            SDL_GetError());
        return 1;
    }

    SDL_Rect textRect;
    {
        // Get text dimensions
        textRect.w = textSurface->w;
        textRect.h = textSurface->h;
        textRect.x = 0;
        textRect.y = 0;
    }

    SDL_FreeSurface(textSurface);

    // Event loop
    while (true) {
        SDL_Event e;

        // Wait indefinitely for the next available event
        SDL_WaitEvent(&e);

        // User requests quit
        if (e.type == SDL_QUIT) {
            break;
        }

        // Initialize renderer color white for the background
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

        // Clear screen
        SDL_RenderClear(renderer);

        // Draw text
        SDL_RenderCopy(renderer, text, NULL, &textRect);

        // Update screen
        SDL_RenderPresent(renderer);
    }

    // Destroy renderer
    SDL_DestroyRenderer(renderer);

    // Destroy window
    SDL_DestroyWindow(window);

    // Quit SDL2_ttf
    TTF_Quit();

    // Quit SDL
    SDL_Quit();

    return 0;
}
