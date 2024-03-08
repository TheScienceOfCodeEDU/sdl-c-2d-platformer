#include <SDL2/SDL_rect.h>
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
#endif

//
// Characters
//

const uint8 E_CHARACTER_STATES_SIZE = 2;
enum {
    E_CHARACTER_STATE_IDLE,
    E_CHARACTER_STATE_WALKING
} typedef character_state;

struct {
    SDL_Rect *SpritesRects;
    int Count;
} typedef animation_sprites;


struct {
    character_state State;
    int Frame;
    int NextUpdate;

    SDL_Texture *SpritesTexture;
    animation_sprites *AnimationsPerState;
} typedef character_animations;

struct {
    int X, Y;
    int W, H;
    int W_MID;
    bool Left, Right, Up, Down;

    character_animations *Animations;
} typedef  character;


//
// Tile map
//

#define MAX_MAP_SIZE 32

struct {
    int TileSize;
    int Tiles[MAX_MAP_SIZE][MAX_MAP_SIZE];
    SDL_Rect *TilesRects;
    int TileRectsCount;
    SDL_Texture *TilesTexture;
} typedef tilemap;


//
// Camera
//

struct {
    int X, Y;
    int W, H;
    character* Focus;
} typedef  camera;
