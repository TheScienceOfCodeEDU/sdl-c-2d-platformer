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
 #include "p2d_globals.h"
#endif

//
// Character animation data
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

//
// Character entity data
//
struct {
    int X, Y;
    int W, H;
    int W_MID;
    bool Left, Right, Up, Down;

    character_animations *Animations;
} typedef character;


inline function character
character_MakePlayer(character_animations *Animations)
{
    character Character = {};
    Character.X = 0;
    Character.Y = 200;
    Character.W = 15 * RENDER_SCALE;
    Character.H = 22 * RENDER_SCALE;
    Character.W_MID = Character.W / 2;
    Character.Animations = Animations;

    return Character;
}

inline function animation_sprites *
character_GetCurrentAnimationSprites(character *Character)
{
    animation_sprites *CurrentSprites = Character->Animations->AnimationsPerState;
    for (int i = 0; i < Character->Animations->State; ++i)
    {
        ++CurrentSprites;
    }
    return CurrentSprites;
}

inline function SDL_Rect *
character_GetAnimationSpriteRect(animation_sprites *Sprites, int Frame) 
{
    SDL_Rect *CurrentRect = Sprites->SpritesRects;
    for (int i = 0; i < Frame; ++i) 
    {
        CurrentRect++;
    }
    return CurrentRect;
}

inline function void
character_Update(character *Character)
{
    // State
    character_state LastState = Character->Animations->State;

    Character->Animations->State = E_CHARACTER_STATE_IDLE;
    if (Character->Left)
    {
        Character->X -= CHARACTER_SPEED;
        Character->Animations->State = E_CHARACTER_STATE_WALKING;
    }
    if (Character->Right)
    {
        Character->X += CHARACTER_SPEED;
        Character->Animations->State = E_CHARACTER_STATE_WALKING;
    }
    if (Character->Up)
    {
        Character->Y -= CHARACTER_SPEED;
        Character->Animations->State = E_CHARACTER_STATE_WALKING;
    }
    if (Character->Down)
    {
        Character->Y += CHARACTER_SPEED;
        Character->Animations->State = E_CHARACTER_STATE_WALKING;
    }

    // Changed state?
    if (LastState != Character->Animations->State)
    {
        Character->Animations->NextUpdate = ANIMATION_NEXT_UPDATE;
        Character->Animations->Frame = 0;
    }
    else
    {   
        --Character->Animations->NextUpdate;
        if (Character->Animations->NextUpdate == 0)
        {
            Character->Animations->NextUpdate = ANIMATION_NEXT_UPDATE;
            animation_sprites *CurrentSprites = character_GetCurrentAnimationSprites(Character);
            Character->Animations->Frame = (Character->Animations->Frame + 1) % CurrentSprites->Count;
        }
    }
}

inline function SDL_Rect *
character_GetCurrentSprite(character *Character)
{
    animation_sprites *CurrentSprites = character_GetCurrentAnimationSprites(Character);
    SDL_Rect *CurrentRect = character_GetAnimationSpriteRect(CurrentSprites, Character->Animations->Frame);
    return CurrentRect;
}

inline function void
character_ProcessKeyboardEvents(character *Player, SDL_Event *Event)
{
    switch(Event->type) {
    case SDL_KEYDOWN:
        switch(Event->key.keysym.sym) {
            case SDLK_LEFT:
                Player->Left = 1;
                break;
            case SDLK_RIGHT:
                Player->Right = 1;
                break;
            case SDLK_UP:
                Player->Up = 1;
                break;
            case SDLK_DOWN:
                Player->Down = 1;
                break;
        }
        break;
    case SDL_KEYUP:
        switch(Event->key.keysym.sym) {
            case SDLK_LEFT:
                Player->Left = 0;
                break;
            case SDLK_RIGHT:
                Player->Right = 0;
                break;
            case SDLK_UP:
                Player->Up = 0;
                break;
            case SDLK_DOWN:
                Player->Down = 0;
                break;
        }
        break;
    }
}


//
// Character resources
//
inline function character_animations *
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
