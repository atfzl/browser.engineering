#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>

// Define MAX and MIN macros
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

// Define screen dimensions
#define SCREEN_WIDTH_INITIAL 800
#define SCREEN_HEIGHT_INITIAL 600

#define FONT_PATH "JetBrainsMono-Regular.ttf"

bool init(SDL_Window **window, SDL_Renderer **renderer, TTF_Font **font) {
    // Initialize SDL2
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf(
            "SDL2 could not be initialized!\n"
            "SDL2 Error: %s\n",
            SDL_GetError());
        return false;
    }

    // Initialize SDL2_ttf
    if (TTF_Init() < 0) {
        printf(
            "SDL2_ttf could not be initialized!\n"
            "SDL2_ttf Error: %s\n",
            TTF_GetError());
        return false;
    }

    // Create window
    *window = SDL_CreateWindow("SDL2_ttf sample", SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH_INITIAL,
                               SCREEN_HEIGHT_INITIAL,
                               SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
    if (!*window) {
        printf(
            "Window could not be created!\n"
            "SDL_Error: %s\n",
            SDL_GetError());
        return false;
    }

    // Create renderer
    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (!*renderer) {
        printf(
            "Renderer could not be created!\n"
            "SDL_Error: %s\n",
            SDL_GetError());
        return false;
    }

    *font = TTF_OpenFont(FONT_PATH, 40);
    if (!*font) {
        printf(
            "Unable to load font: '%s'!\n"
            "SDL2_ttf Error: %s\n",
            FONT_PATH, TTF_GetError());
        return false;
    };

    return true;
}

void cleanup(SDL_Window *window, SDL_Renderer *renderer) {
    // Destroy renderer
    SDL_DestroyRenderer(renderer);

    // Destroy window
    SDL_DestroyWindow(window);

    // Quit SDL2_ttf
    TTF_Quit();

    // Quit SDL
    SDL_Quit();
}

bool eventLoop(SDL_Renderer *renderer, SDL_Window *window, TTF_Font *font,
               const char *message) {
    SDL_Color textColor = {0x00, 0x00, 0x00, 0xFF};
    SDL_Color textBackgroundColor = {0xFF, 0xFF, 0xFF, 0xFF};

    // Initialize renderer color white for the background
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

    // Clear screen
    SDL_RenderClear(renderer);

    int SCREEN_WIDTH;
    int SCREEN_HEIGHT;
    SDL_GL_GetDrawableSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    const int HSTEP = 24;
    const int VSTEP = 48;
    int cursor_x = HSTEP;
    int cursor_y = VSTEP;

    for (size_t i = 0; i < strlen(message); ++i) {
        const char c[] = {message[i], '\0'};
        SDL_Surface *textSurface =
            TTF_RenderText_Shaded(font, c, textColor, textBackgroundColor);
        if (!textSurface) {
            printf(
                "Unable to render text surface!\n"
                "SDL2_ttf Error: %s\n",
                TTF_GetError());
            return false;
        }

        SDL_SetColorKey(textSurface, SDL_TRUE,
                        SDL_MapRGB(textSurface->format, 0xFF, 0xFF, 0xFF));

        // Create texture from surface pixels
        SDL_Texture *textTexture =
            SDL_CreateTextureFromSurface(renderer, textSurface);
        if (!textTexture) {
            printf(
                "Unable to create texture from rendered text!\n"
                "SDL2 Error: %s\n",
                SDL_GetError());
            return false;
        }

        SDL_Rect textRect = {cursor_x, cursor_y, textSurface->w,
                             textSurface->h};

        cursor_x += HSTEP;

        if (cursor_x >= SCREEN_WIDTH - HSTEP) {
            cursor_y += VSTEP;
            cursor_x = HSTEP;
        }

        SDL_FreeSurface(textSurface);

        // Draw text
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    }

    // Update screen
    SDL_RenderPresent(renderer);

    return true;
}

int main(int argc, char *argv[]) {
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    TTF_Font *font = NULL;

    if (!init(&window, &renderer, &font)) {
        return EXIT_FAILURE;
    }

    const char message[] =
        "hello world /usr/bin/clang -Wall -std=c11 -g "
        "/Users/aafzal/scratch/browser.engineering/graphics.c -o "
        "/Users/aafzal/scratch/browser.engineering/graphics -framework OpenGL "
        "-I/usr/local/opt/openssl/include -L/usr/local/opt/openssl/lib -lssl "
        "-lcrypto -lSDL2 -lSDL2_ttf -lSDL2_image -lglfw Build finished "
        "successfully.";

    // Event loop
    while (true) {
        SDL_Event e;

        // Wait indefinitely for the next available event
        SDL_WaitEvent(&e);

        // User requests quit
        if (e.type == SDL_QUIT) {
            break;
        }

        if (!eventLoop(renderer, window, font, message)) {
            return EXIT_FAILURE;
        }
    }

    cleanup(window, renderer);

    return 0;
}
