#ifndef UNITY_BUILD
 #pragma once
 #ifdef _WIN64
  #include <SDL.h>
  #include <SDL_image.h>
 #else
  #include <SDL2/SDL.h>
  #include <SDL2/SDL_image.h>
 #endif
 #include "p2d_memory.h"
 #include "p2d_globals.h"
 #include "p2d_gamestate.h"
#endif

inline function void
render_Draw(gamestate *Gamestate)
{
    SDL_Renderer *Renderer = Gamestate->Renderer;
    tilemap *Tilemap = &Gamestate->Tilemap;
    character *Player = &Gamestate->Player;
    camera *Camera = &Gamestate->Camera;

    SDL_RenderClear(Renderer);

    //
    // Tilemap
    //
    int TileSize = Tilemap->Resources->TileSize;
    int TargetSize = TileSize * RENDER_SCALE;
    for (int i = 0; i < MAX_MAP_SIZE; ++i) 
    {
        for (int j = 0; j < MAX_MAP_SIZE; ++j) 
        {
            int CurrentTile = Tilemap->Tiles[j][i] - 1; // As 0 means empty, adjust CurrentTile value 
            if (CurrentTile == -1) continue;

            SDL_Rect *SrcRect = Tilemap->Resources->TilesRects;
            for (int r = 0; r < Tilemap->Resources->TileRectsCount && r < CurrentTile - 1; ++r)
            {
                ++SrcRect;
            }

            SDL_Rect DestRect;
            DestRect = (SDL_Rect){ i * TargetSize - Camera->X, j * TargetSize - Camera->Y, TargetSize, TargetSize};            
            SDL_RenderCopy(Renderer, Tilemap->Resources->TilesTexture, SrcRect, &DestRect);
        }
    }

    //
    // Character
    //
    SDL_Rect *CurrentSrcRect = character_GetCurrentSprite(Player);
    SDL_Rect DestRect;
    DestRect.x = Player->X - Camera->X;
    DestRect.y = Player->Y - Camera->Y;
    DestRect.w = CurrentSrcRect->w * RENDER_SCALE;
    DestRect.h = CurrentSrcRect->h * RENDER_SCALE;
    SDL_RenderCopy(Renderer, Player->Animations->SpritesTexture, CurrentSrcRect, &DestRect);
    
    SDL_RenderPresent(Renderer);
}
