#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdint.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

enum KeyPressSurfaces {
    KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_TOTAL,
};

bool init();
bool loadMedia();
void close();

SDL_Surface* loadSurface(const char* path);

SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Surface* gKeyPressSurfaces[KEY_PRESS_SURFACE_TOTAL];
SDL_Surface* gCurrentSurface = NULL;

SDL_Surface* loadSurface(const char* path) {
    SDL_Surface* loadedSurface = IMG_Load(path);
    if (loadedSurface == NULL) {
        printf("Unable to load image %s! SDL Error: %s\n", path,
               SDL_GetError());
        return NULL;
    }

    SDL_Surface* optimizedSurface =
        SDL_ConvertSurface(loadedSurface, gScreenSurface->format, 0);

    if (optimizedSurface == NULL) {
        printf("Unable to optimize image %s! SDL Error: %s\n", path,
               SDL_GetError());
        return NULL;
    }

    SDL_FreeSurface(loadedSurface);

    return optimizedSurface;
}

bool init() {
    // Initialization flag
    bool success = true;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        success = false;
    } else {
        // Create window
        gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED,
                                   SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                                   SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL) {
            printf("Window could not be created! SDL_Error: %s\n",
                   SDL_GetError());
            success = false;
        } else {
            int imgFlags = IMG_INIT_PNG;
            if (!(IMG_Init(imgFlags) & imgFlags)) {
                printf("SDL_image could not initialize! SDL_image Error: %s\n",
                       IMG_GetError());
                success = false;
            }

            // Get window surface
            gScreenSurface = SDL_GetWindowSurface(gWindow);
        }
    }

    return success;
}

bool loadMedia() {
    // Loading success flag
    bool success = true;

    // Load default surface
    gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] =
        loadSurface("png_example.png");
    if (gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] == NULL) {
        printf("Failed to load default image!\n");
        success = false;
    }

    // Load up surface
    gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] =
        loadSurface("04_key_presses/up.bmp");
    if (gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] == NULL) {
        printf("Failed to load up image!\n");
        success = false;
    }

    // Load down surface
    gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] =
        loadSurface("04_key_presses/down.bmp");
    if (gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] == NULL) {
        printf("Failed to load down image!\n");
        success = false;
    }

    // Load left surface
    gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] =
        loadSurface("04_key_presses/left.bmp");
    if (gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] == NULL) {
        printf("Failed to load left image!\n");
        success = false;
    }

    // Load right surface
    gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] =
        loadSurface("04_key_presses/right.bmp");
    if (gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] == NULL) {
        printf("Failed to load right image!\n");
        success = false;
    }

    return success;
}

void close() {
    // Deallocate surface
    SDL_FreeSurface(gCurrentSurface);
    gCurrentSurface = NULL;

    // Destroy window
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    // Quit SDL subsystems
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    if (!init()) {
        printf("%s\n", "Failed to initialize");
        return 1;
    }

    if (!loadMedia()) {
        printf("%s\n", "Failed to load media");
        return 1;
    }

    // Main loop flag
    bool quit = false;

    // Event handler
    SDL_Event e;

    // Set default current surface
    gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];

    SDL_Rect stretchRect;
    stretchRect.x = 0;
    stretchRect.y = 0;
    stretchRect.w = SCREEN_WIDTH;
    stretchRect.h = SCREEN_HEIGHT;
    SDL_BlitScaled(gCurrentSurface, NULL, gScreenSurface, &stretchRect);

    SDL_UpdateWindowSurface(gWindow);

    // While application is running
    while (!quit) {
        // Handle events on queue
        while (SDL_PollEvent(&e) != 0) {
            // User requests quit
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            // User presses a key
            else if (e.type == SDL_KEYDOWN) {
                // Select surfaces based on key press
                switch (e.key.keysym.sym) {
                    case SDLK_UP:
                        gCurrentSurface =
                            gKeyPressSurfaces[KEY_PRESS_SURFACE_UP];
                        break;

                    case SDLK_DOWN:
                        gCurrentSurface =
                            gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN];
                        break;

                    case SDLK_LEFT:
                        gCurrentSurface =
                            gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT];
                        break;

                    case SDLK_RIGHT:
                        gCurrentSurface =
                            gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT];
                        break;

                    default:
                        gCurrentSurface =
                            gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
                        break;
                }
                SDL_BlitScaled(gCurrentSurface, NULL, gScreenSurface,
                               &stretchRect);
                SDL_UpdateWindowSurface(gWindow);
            }
        }
    }

    close();

    return 0;
}
