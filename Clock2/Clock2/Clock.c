#include <SDL.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <SDL_ttf.h>

void closeProgram(SDL_Renderer* renderer, SDL_Window* window);
void renderTime(SDL_Renderer* renderer, const char* timeString);
char* currentTime();

int main() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1; // Exit if initialization fails
    }

    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        printf("SDL_ttf could not initialize! TTF_Error: %s\n", TTF_GetError());
        return 1; // Exit if initialization fails
    }

    // Create window and renderer
    SDL_Window* window = SDL_CreateWindow("Clock", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1500, 1000, 0);
    if (!window) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1; // Exit if window creation fails
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1; // Exit if renderer creation fails
    }

    // Set background color
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);  // Grey background

    // Main loop
    int quit = 0;
    while (!quit) {
        // Poll for events
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            // If the user presses a key, or clicks the close button, exit
            if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN)) {
                quit = 1;  // Close button is clicked or any key is pressed
            }
        }

        // Clear the renderer
        SDL_RenderClear(renderer); // Clear the screen with the background color

        // Get the current time
        char* timeString = currentTime(); // Get the current time string

        // Render the time using rectangles
        renderTime(renderer, timeString); // Render time using rectangles

        // Present the renderer
        SDL_RenderPresent(renderer);  // Update the screen

        SDL_Delay(16); // Delay to prevent CPU overuse
    }

    // Close program
    closeProgram(renderer, window);
    return 0;
}

// Destroys renderer and window, quits
void closeProgram(SDL_Renderer* renderer, SDL_Window* window) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

// Returns the current time in the format HH:MM:SS
char* currentTime() {
    // Creates a static variable representing time
    static char timeString[100];

    // Get the current time as a time_t object
    time_t now = time(NULL);

    // Convert time_t object into local time representation
    struct tm* local = localtime(&now);

    // Format the local time into the timeString array in the format HH:MM:SS
    strftime(timeString, sizeof(timeString), "%H:%M:%S", local);

    return timeString;
}

// Renders and displays time on screen
void renderTime(SDL_Renderer* renderer, const char* timeString) {
    // open font
    TTF_Font* font = TTF_OpenFont("digital-7.ttf", 150);
    if (!font) {
        printf("Failed to load font! TTF_Error: %s\n", TTF_GetError());
        return; // Exit if font loading fails
    }

    // Set font color (white)
    SDL_Color color = { 255, 255, 255, 255 };

    // Create a surface from the rendered text
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, timeString, color);
    if (!textSurface) {
        printf("Failed to create text surface! TTF_Error: %s\n", TTF_GetError());
        TTF_CloseFont(font);
        return; // Exit if surface creation fails
    }

    // Create texture from the surface
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!textTexture) {
        printf("Failed to create text texture! SDL_Error: %s\n", SDL_GetError());
        SDL_FreeSurface(textSurface);
        TTF_CloseFont(font);
        return; // Exit if texture creation fails
    }

    // get size of current window
    int windowWidth, windowHeight;
    SDL_GetRendererOutputSize(renderer, &windowWidth, &windowHeight);

    // Calculate the x and y position for centering the text based on window size
    int textX = (windowWidth - textSurface->w) / 2;
    int textY = (windowHeight - textSurface->h) / 2;

    // Set the position and size of the text
    SDL_Rect textRect = { textX, textY, textSurface->w, textSurface->h };

    // Render the text texture
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    // Clean up resources
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(font);
}