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

    for (int i = 0; i < OneBit::RENDER_WIDTH * OneBit::RENDER_HEIGHT; i++)
    {
        uint32_t index = i * 4;
        uint8_t colour = pixels[i];
        texPixels[index] = 255;
        texPixels[index + 1] = colour;
        texPixels[index + 2] = colour;
        texPixels[index + 3] = colour;
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
    SDL_Window* window = SDL_CreateWindow("OneBitRender", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
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

            obRenderer.SetLineColour(OneBit::LineColour::BLACK);

            std::vector<OneBit::Vertex> vertices = {
                {0.0, 0.0, 0, 0, 0, 0}, {0.999, 0.999, 0, 0, 0, 128}, {0.0, 0.999, 0, 0, 0, 255},
                //{0.1, 0.1, 0, 0, 0, 0}, {0.2, 0.15, 0, 0, 0, 128},    {0.1, 0.2, 0, 0, 0, 255},
            };

            // Event loop exit flag
            bool quit = false;

            uint32_t t = 0;

            auto shadingType = OneBit::ShadingType::FLAT;

            // Event loop
            while (!quit)
            {
                SDL_Event e;

                // Wait indefinitely for the next available event
                while (SDL_PollEvent(&e))
                {
                    // User requests quit
                    if (e.type == SDL_QUIT)
                    {
                        quit = true;
                    }
                }

                // if (t % 1000 == 0)
                //{
                //    shadingType = shadingType == OneBit::ShadingType::FLAT ? OneBit::ShadingType::WIREFRAME
                //                                                           : OneBit::ShadingType::FLAT;
                //}

                //                vertices[0].y = 0.25 + (0.2 * sin(0.01 * t));
                //                vertices[1].y = 0.75 + (0.2 * sin(0.01 * t + 10));
                //                vertices[2].y = 0.75 + (0.2 * sin(0.01 * t + 20));
                //
                // vertices[0].x = t % OneBit::RENDER_WIDTH;
                vertices[0].x = static_cast<float>(((t / 2) % OneBit::RENDER_WIDTH)) / OneBit::RENDER_WIDTH;
                obRenderer.Clear(255);

                obRenderer.SetShadingType(shadingType);
                obRenderer.Render(vertices);

                // obRenderer.DrawLine({{50, 239}, {350, 120}});

                auto backbuffer = obRenderer.GetBackbuffer();
                CopyImageToTexture(renderTexture, backbuffer.data());

                SDL_RenderCopy(renderer, renderTexture, NULL, NULL);

                // Update screen
                SDL_RenderPresent(renderer);

                t++;
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
