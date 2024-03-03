#define UNITY_BUILD 1
#include <stdio.h>
#include <cstring>
#include <stdlib.h>
#include <assert.h>
#ifdef _WIN64
 #include <SDL.h>
 #include <SDL_image.h>
#else
 #include <SDL2/SDL.h>
 #include <SDL2/SDL_image.h>
#endif
#include "p2d_globals.h"
#include "p2d_common.h"
#include "p2d_structs.h"
#include "p2d_memory.h"
#include "p2d_sdl_utils.h"
#include "p2d_sprites.h"
#include "p2d_characters.h"

int
main(int argc, char *args[])
{
    arena Arena = MakeArena();

    SDL_Window *Window;
    SDL_Renderer *Renderer;
    // Init SDL without texture filtering for better pixelart results
    if (sdl_utils_Init("SDL Tutorial", &Window, &Renderer, 0)) 
    {
        character *Player = character_CreatePlayer(&Arena, Renderer);
        
        Uint64 LastTicks = 0;
        while (1)
        {            
            // Update SDL events
            SDL_Event Event;
            while (SDL_PollEvent(&Event))
            {
                if (Event.type == SDL_QUIT) 
                {
                    SDL_DestroyTexture(Player->Animations->SpritesTexture);
                    sdl_utils_Quit(Window, Renderer);
                    return 0;
                }

                switch(Event.type) {
                case SDL_KEYDOWN:
                    switch(Event.key.keysym.sym){
                        case SDLK_LEFT:
                            Player->Left = 1;
                            break;
                        case SDLK_RIGHT:
                            Player->Right = 1;
                            break;
                    }
                    break;
                case SDL_KEYUP:
                    switch(Event.key.keysym.sym){
                        case SDLK_LEFT:
                            Player->Left = 0;
                            break;
                        case SDLK_RIGHT:
                            Player->Right = 0;
                            break;
                    }
                    break;
                }
            }
            // Fixed FPS
            while (SDL_GetTicks64() - LastTicks < TICKS_FPS) { }
            LastTicks = SDL_GetTicks64();

            // Update player
            SDL_Rect *CurrentSrcRect = character_Update(Player);            

            // Render
            SDL_RenderClear(Renderer);

            SDL_Rect DestRect;
            DestRect.x = Player->X;
            DestRect.y = Player->Y;
            DestRect.w = CurrentSrcRect->w * RENDER_SCALE;
            DestRect.h = CurrentSrcRect->h * RENDER_SCALE;
            SDL_RenderCopy(Renderer, Player->Animations->SpritesTexture, CurrentSrcRect, &DestRect);

            SDL_RenderPresent(Renderer);
        }
    }
    
    return 0;
}