#include <SDL2/SDL_render.h>
#ifndef UNITY_BUILD
 #pragma once
 #ifdef __MINGW32__
  #include <SDL.h>
  #include <SDL_image.h>
 #else
  #include <SDL2/SDL.h>
  #include <SDL2/SDL_image.h>
 #endif
 #include "p2d_memory.h"
 #include "p2d_sprites.h"
 #include "p2d_globals.h"
 #include "p2d_structs.h"
 
#endif

inline function void
map_Draw(tilemap *Tilemap, SDL_Renderer *Renderer)
{
    int TileSize = Tilemap->TileSize;
    int TargetSize = TileSize * RENDER_SCALE;
    for (int i = 0; i < MAX_MAP_SIZE; ++i) 
    {
        for (int j = 0; j < MAX_MAP_SIZE; ++j) 
        {
            int CurrentTile = Tilemap->Tiles[j][i] - 1; // As 0 means empty, adjust CurrentTile value 
            if (CurrentTile == -1) continue;

            SDL_Rect *SrcRect = Tilemap->TilesRects;
            for (int r = 0; r < Tilemap->TileRectsCount && r < CurrentTile - 1; ++r)
            {
                ++SrcRect;
            }

            SDL_Rect DestRect;
            DestRect = (SDL_Rect){ i * TargetSize, j * TargetSize, TargetSize, TargetSize};            
            SDL_RenderCopy(Renderer, Tilemap->TilesTexture, SrcRect, &DestRect);
        }
    }
}
