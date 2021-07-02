#define SDL_MAIN_HANDLED
#include <iostream>

#include "SDL.h"

#include "OneBitRender/renderer.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

void CopyImageToTexture(SDL_Texture* texture, const uint8_t* pixels)
{
    uint8_t* texPixels{nullptr};
    int32_t texPitch = 0;
    SDL_LockTexture(texture, NULL, reinterpret_cast<void**>(&texPixels), &texPitch);

    for (int y = 0; y < OneBit::RENDER_HEIGHT; y++)
    {
        for (int x = 0; x < OneBit::RENDER_WIDTH; x++)
        {
            uint32_t index = y * OneBit::RENDER_WIDTH + x;

            uint8_t colour = pixels[index];
            texPixels[(index * 4) + 0] = 255;
            texPixels[(index * 4) + 1] = pixels[index];
            texPixels[(index * 4) + 2] = pixels[index];
            texPixels[(index * 4) + 3] = pixels[index];
        }
    }

    SDL_UnlockTexture(texture);
}

int main()
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL could not be initialized!" << std::endl << "SDL_Error: " << SDL_GetError() << std::endl;
        return 0;
    }

    // Create window
    SDL_Window* window = SDL_CreateWindow("Basic C++ SDL project", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                          SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window)
    {
        std::cout << "Window could not be created!" << std::endl << "SDL_Error: " << SDL_GetError() << std::endl;
    }
    else
    {
        // Create renderer
        SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (!renderer)
        {
            std::cout << "Renderer could not be created!" << std::endl << "SDL_Error: " << SDL_GetError() << std::endl;
        }
        else
        {
            SDL_RenderSetLogicalSize(renderer, OneBit::RENDER_WIDTH, OneBit::RENDER_HEIGHT);

            SDL_Texture* renderTexture =
                SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, OneBit::RENDER_WIDTH,
                                  OneBit::RENDER_HEIGHT);

            OneBit::Renderer obRenderer;
            obRenderer.Clear(128);

            auto backbuffer = obRenderer.GetBackbuffer();

            CopyImageToTexture(renderTexture, backbuffer.data());

            // Event loop exit flag
            bool quit = false;

            // Event loop
            while (!quit)
            {
                SDL_Event e;

                // Wait indefinitely for the next available event
                SDL_WaitEvent(&e);

                // User requests quit
                if (e.type == SDL_QUIT)
                {
                    quit = true;
                }

                SDL_RenderCopy(renderer, renderTexture, NULL, NULL);

                // Update screen
                SDL_RenderPresent(renderer);
            }

            SDL_DestroyTexture(renderTexture);

            // Destroy renderer
            SDL_DestroyRenderer(renderer);
        }

        // Destroy window
        SDL_DestroyWindow(window);
    }

    // Quit SDL
    SDL_Quit();

    return 0;
}
