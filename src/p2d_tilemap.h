#ifndef UNITY_BUILD
 #pragma once
 #ifdef _WIN64
  #include <SDL.h>
  #include <SDL_image.h>
 #else
  #include <SDL2/SDL.h>
  #include <SDL2/SDL_image.h> 
 #endif
 #include "p2d_globals.h"
 #include "p2d_camera.h"
#endif

//
// Tile map
//

#define MAX_MAP_SIZE 32

struct {
    int TileSize;    
    SDL_Texture *TilesTexture;

    SDL_Rect *TilesRects;
    int TileRectsCount;
} typedef tilemap_resources;

struct {    
    int Tiles[MAX_MAP_SIZE][MAX_MAP_SIZE];
    tilemap_resources *Resources;    
} typedef tilemap;


//
// Tilemap resources
//
inline function tilemap_resources*
resources_LoadTilemapResources(arena *Arena, SDL_Renderer *Renderer)
{
    tilemap_resources *Resources = (tilemap_resources *) ReserveMemory(Arena, sizeof(tilemap_resources));
    Resources->TilesTexture = IMG_LoadTexture(Renderer, "res/sheet.png");
    Resources->TileSize = 16;

    Resources->TilesRects = (SDL_Rect *) ReserveMemory(Arena, sizeof(SDL_Rect));
    *Resources->TilesRects = (SDL_Rect) {129, 0, 16, 16};
    Resources->TileRectsCount = 1;

    return Resources;
}

inline function tilemap
resources_LoadTilemap(tilemap_resources *Resources)
{
    tilemap Tilemap = {};
    Tilemap.Resources = Resources;

    // Load file
    size_t DataSize;
    void *MapContents = SDL_LoadFile("res/map.p2d", &DataSize);

    // Parse it
    int MapX = 0;
    int MapY = 0;
    char *Current = (char *) MapContents;
    char *CurrentNumber = Current;
    while (*Current != '\0') 
    {
        bool NewLine = 0;
        if (*Current == '\n') 
        {
            NewLine = 1;
        }

        if (*Current == ',' || *Current == '\n')
        {
            *Current = '\0';
            Tilemap.Tiles[MapY][MapX] = strtol(CurrentNumber, 0, 10);
            CurrentNumber = Current + 1;
            ++MapX;
        }

        if (NewLine) 
        {
            ++MapY;
            MapX = 0;
        }
        ++Current;
    }
    
    SDL_free(MapContents);
    return Tilemap;
}
