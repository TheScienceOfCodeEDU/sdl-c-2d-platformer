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
 #include "p2d_structs.h"
 #include "p2d_sdl_utils.h" 
#endif

function character_animations *
resources_LoadCharacterAnimations(arena *Arena, SDL_Renderer *Renderer)
{
    assert(E_CHARACTER_STATES_SIZE == 2);
    assert(E_CHARACTER_STATE_IDLE == 0);
    assert(E_CHARACTER_STATE_WALKING == 1);

    const int IDLE_COUNT = 1;
    SDL_Rect *IdleSpritesRects = (SDL_Rect *) ReserveMemory(Arena, sizeof(SDL_Rect) * IDLE_COUNT);
    SDL_Rect *CurrentIdleSprite = IdleSpritesRects;
    *IdleSpritesRects = (SDL_Rect) { 9, 42, 15, 22};

    const int WALKING_COUNT = 4;
    SDL_Rect *WalkingSpritesRects = (SDL_Rect *) ReserveMemory(Arena, sizeof(SDL_Rect) * WALKING_COUNT);
    SDL_Rect *CurrentWalkingSprite = WalkingSpritesRects;
    *CurrentWalkingSprite = (SDL_Rect) { 41, 41, 15, 22};
    *(++CurrentWalkingSprite) = (SDL_Rect) {72, 42, 16, 22};
    *(++CurrentWalkingSprite) = (SDL_Rect) {104, 41, 17, 22};
    *(++CurrentWalkingSprite) = (SDL_Rect) {9, 42, 15, 22};

    
    animation_sprites *Sprites = (animation_sprites *) ReserveMemory(Arena, sizeof(animation_sprites) * E_CHARACTER_STATES_SIZE);
    animation_sprites *CurrentSprite = Sprites;
    CurrentSprite->SpritesRects = IdleSpritesRects;
    CurrentSprite->Count = IDLE_COUNT;

    ++CurrentSprite;
    CurrentSprite->SpritesRects = WalkingSpritesRects;
    CurrentSprite->Count = WALKING_COUNT;

    character_animations *Anim = (character_animations *) ReserveMemory(Arena, sizeof(character_animations));
    Anim->State = E_CHARACTER_STATE_IDLE;
    Anim->Frame = 0;
    Anim->NextUpdate = ANIMATION_NEXT_UPDATE;
    Anim->AnimationsPerState = Sprites;
    Anim->SpritesTexture = IMG_LoadTexture(Renderer, "res/characters.png");

    return Anim;
}

function tilemap*
resources_LoadTilemap(arena *Arena, SDL_Renderer *Renderer)
{
    tilemap *Tilemap = (tilemap *) ReserveMemory(Arena, sizeof(tilemap));
    Tilemap->TilesTexture = IMG_LoadTexture(Renderer, "res/sheet.png");
    Tilemap->TileSize = 16;

    Tilemap->TilesRects = (SDL_Rect *) ReserveMemory(Arena, sizeof(SDL_Rect));
    *Tilemap->TilesRects = (SDL_Rect) {129, 0, 16, 16};
    Tilemap->TileRectsCount = 1;

    size_t DataSize;
    void *MapContents = SDL_LoadFile("res/map.p2d", &DataSize);

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
            Tilemap->Tiles[MapY][MapX] = strtol(CurrentNumber, 0, 10);
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
