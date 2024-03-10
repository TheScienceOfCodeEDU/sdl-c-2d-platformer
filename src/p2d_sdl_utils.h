#ifndef UNITY_BUILD
 #pragma once
 #include <stdio.h>
 #ifdef _WIN64
  #include <SDL.h>
  #include <SDL_image.h>
 #else
  #include <SDL2/SDL.h>
  #include <SDL2/SDL_image.h>
 #endif
 #include "p2d_globals.h"
#endif


function inline bool
sdl_utils_Init(const char *Title, SDL_Window **Window, SDL_Renderer **Renderer, bool TextureFiltering)
{
    if (SDL_Init(SDL_INIT_VIDEO ) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 0;
    }
    if (TextureFiltering)
    {
        if (!SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1"))
        {
            printf("Warning: Linear texture filtering not enabled!");
        }
    }

    *Window = SDL_CreateWindow(Title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (Window == 0)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 0;
    }

    *Renderer = SDL_CreateRenderer(*Window, -1, SDL_RENDERER_ACCELERATED);
    if (Renderer == 0)
    {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return 0;
    }
    SDL_SetRenderDrawColor(*Renderer, BG_R, BG_G, BG_B, 0xFF);
    
    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return 0;
    }
    
    return 1;
}

function void
sdl_utils_Quit(SDL_Window *Window, SDL_Renderer *Renderer)
{
    if (Renderer) SDL_DestroyRenderer(Renderer);
    if (Window) SDL_DestroyWindow(Window);

    IMG_Quit();
    SDL_Quit();
}
