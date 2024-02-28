#include <SDL2/SDL_events.h>
#define UNITY_BUILD 1
#include <stdio.h>
#include <cstring>
#include <stdlib.h>
#include <assert.h>
#ifdef __MINGW32__
 #include <SDL.h>
 #include <SDL_image.h>
#else
 #include <SDL2/SDL.h>
 #include <SDL2/SDL_image.h>
#endif
#include "p2d_globals.h"
#include "p2d_common.h"
#include "p2d_sdl_utils.h"
#include "p2d_structs.h"
#include "p2d_memory.h"
#include "p2d_sprites.h"

int
main(int argc, char *args[])
{
    arena Arena = MakeArena();

    SDL_Window *Window;
    SDL_Renderer *Renderer;
    // Init SDL without texture filtering for better pixelart results
    if (sdl_utils_Init("SDL Tutorial", &Window, &Renderer, 0)) 
    {
        SDL_Texture *Texture = sdl_utils_loadTexture("res/characters.png", Renderer);

        character_anim *Animations = LoadCharacterAnimations(&Arena);
        
        // Sprite source rectangle
        /*SDL_Rect SrcRect = {9, 42, 15, 22};
        SDL_Rect SrcRect2 = {41, 41, 15, 22};
        SDL_Rect SrcRect3 = {72, 42, 16, 22};
        SDL_Rect SrcRect4 = {104, 41, 17, 22};
        // Target rectangle (note that we will paint it at x4 its original size)*/
        SDL_Rect DestRect = {0, 0, 15 * 4, 22 * 4};
        int desired_fps = 30;
        int last_ticks = SDL_GetTicks();
        bool Left = 0;
        bool Right = 0;
        while (1)
        {
            SDL_Event Event;
            if (SDL_PollEvent(&Event))
            {
                if (Event.type == SDL_QUIT) break;

                switch(Event.type) {
                case SDL_KEYDOWN:
                    switch(Event.key.keysym.sym){
                        case SDLK_LEFT:
                            Left = 1;
                            break;
                        case SDLK_RIGHT:
                            Right = 1;
                            break;
                    }
                    break;
                case SDL_KEYUP:                    
                    switch(Event.key.keysym.sym){
                        case SDLK_LEFT:
                            Left = 0;
                            break;
                        case SDLK_RIGHT:
                            Right = 0;
                            break;
                    }
                    break;
                }
            }

            if (SDL_GetTicks() - last_ticks < 1000/desired_fps) 
            {
                continue;
            }
            last_ticks = SDL_GetTicks();

            SDL_RenderClear(Renderer);

            if (Left || Right)
                Animations->State = E_ANIM_STATE_WALKING;
            else
                Animations->State = E_ANIM_STATE_IDLE;
            
            if (Left)
                DestRect.x -= 5;
            if (Right)
                DestRect.x  += 5;

            // Find current set of sprites
            anim_sprites *CurrentSprites = Animations->AnimationsPerState;
            for (int i = 0; i < Animations->State; ++i)
            {
                ++CurrentSprites;
            }

            
            SDL_Rect *CurrentRect = CurrentSprites->SpritesRects;
            for (int i = 0; i < Animations->Frame; ++i) 
            {
                CurrentRect++;
            }
            DestRect.w = CurrentRect->w * 4;
            DestRect.h = CurrentRect->h * 4;       
            SDL_RenderCopy(Renderer, Texture, CurrentRect, &DestRect);

            --Animations->NextUpdate;
            if (Animations->NextUpdate == 0)
            {
                Animations->NextUpdate = ANIMATION_NEXT_UPDATE;
                Animations->Frame = (Animations->Frame + 1) % CurrentSprites->Count;
            }
            SDL_RenderPresent(Renderer); 
        }

        SDL_DestroyTexture(Texture);		
    }
    sdl_utils_Quit(Window, Renderer);
    return 0;
}
