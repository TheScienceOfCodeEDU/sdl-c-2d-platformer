
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
        
        SDL_Rect DestRect = {0, 0, 15 * 4, 22 * 4};
        int desired_fps = 30;
        int last_ticks = SDL_GetTicks();
        
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

            if (SDL_GetTicks() - last_ticks < 1000/desired_fps) 
            {
                continue;
            }
            last_ticks = SDL_GetTicks();

            SDL_RenderClear(Renderer);

            character_anim_state LastState = Player->Animation->State;
            if (!Player->Left && !Player->Right)
            {
                Player->Animation->State = E_ANIM_STATE_IDLE;
            }                
            else
            {
                Player->Animation->State = E_ANIM_STATE_WALKING;
                if (Player->Left)
                    Player->X -= 5;
                if (Player->Right)
                    Player->X += 5;
            }

            if (LastState != Player->Animation->State)
            {
                Player->Animation->Frame = 0;
            }
            
            // Find current set of sprites
            anim_sprites *CurrentSprites = character_GetCurrentAnimationSet(Player);
            
            SDL_Rect *CurrentRect = character_GetSpriteRect(CurrentSprites, Player->Animation->Frame);
            DestRect.x = Player->X;
            DestRect.y = Player->Y;
            DestRect.w = CurrentRect->w * 4;
            DestRect.h = CurrentRect->h * 4;       
            SDL_RenderCopy(Renderer, Player->Animation->SpritesTexture, CurrentRect, &DestRect);

            --Player->Animation->NextUpdate;
            if (Player->Animation->NextUpdate == 0)
            {
                Player->Animation->NextUpdate = ANIMATION_NEXT_UPDATE;
                Player->Animation->Frame = (Player->Animation->Frame + 1) % CurrentSprites->Count;
            }
            SDL_RenderPresent(Renderer); 
        }

        SDL_DestroyTexture(Player->Animation->SpritesTexture);	
    }
    sdl_utils_Quit(Window, Renderer);
    return 0;
}
