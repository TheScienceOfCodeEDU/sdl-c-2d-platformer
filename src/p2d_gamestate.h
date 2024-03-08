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
 #include "p2d_characters.h"
 #include "p2d_camera.h"
 #include "p2d_tilemap.h"
#endif


//
// Game state
//
struct {
    SDL_Renderer *Renderer;
    character Player;
    tilemap Tilemap;
    camera Camera;    
} typedef gamestate;


inline function gamestate *
resources_LoadResources_MakeGamestate(arena *Arena, SDL_Renderer *Renderer)
{
    gamestate *Gamestate = (gamestate*) ReserveMemory(Arena, sizeof(gamestate));
    Gamestate->Camera = camera_MakeCamera(&Gamestate->Player);
    Gamestate->Renderer = Renderer;

    // Player character
    character_animations *Animations = resources_LoadCharacterAnimations(Arena, Renderer);
    Gamestate->Player = character_MakePlayer(Animations);

    // Tile map
    tilemap_resources *TilemapResources = resources_LoadTilemapResources(Arena, Renderer);
    Gamestate->Tilemap = resources_LoadTilemap(TilemapResources);    

    return Gamestate;
}
