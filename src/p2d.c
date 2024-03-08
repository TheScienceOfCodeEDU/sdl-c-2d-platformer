
#define UNITY_BUILD 1
#include <stdio.h>              // IWYU pragma: keep
#include <string.h>             // IWYU pragma: keep
#include <assert.h>             // IWYU pragma: keep
#include <stdbool.h>            // IWYU pragma: keep
#ifdef _WIN64
 #include <SDL.h>
 #include <SDL_image.h>
#else
 #include <SDL2/SDL.h>
 #include <SDL2/SDL_image.h>
#endif
#include "p2d_globals.h"
#include "p2d_common.h"
#include "p2d_memory.h"
#include "p2d_sdl_utils.h"
#include "p2d_structs.h"
#include "p2d_characters.h"
#include "p2d_camera.h"
#include "p2d_render.h"
#include "p2d_resources.h"

int
main(int argc, char *args[])
{
    arena Arena = MakeArena();

    SDL_Window *Window;
    SDL_Renderer *Renderer;
    // Init SDL without texture filtering for better pixelart results
    if (sdl_utils_Init("SDL Tutorial", &Window, &Renderer, 0)) 
    {
        gamestate *Gamestate = resources_CreateGamestate(&Arena, Renderer);

        Uint64 LastTicks = 0;
        while (1)
        {            
            // Update SDL events
            SDL_Event Event;
            while (SDL_PollEvent(&Event))
            {
                if (Event.type == SDL_QUIT) 
                {
                    SDL_DestroyTexture(Gamestate->Player.Animations->SpritesTexture);
                    sdl_utils_Quit(Window, Renderer);
                    return 0;
                }

                character_ProcessKeyboardEvents(Gamestate, &Event);
            }

            // Fixed FPS
            while (SDL_GetTicks64() - LastTicks < TICKS_FPS) { }
            LastTicks = SDL_GetTicks64();

            // Update
            character_Update(&Gamestate->Player);
            camera_Update(&Gamestate->Camera);

            render_Draw(Gamestate);            
        }
    }
    
    return 0;
}
