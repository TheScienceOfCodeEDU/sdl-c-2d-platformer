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
 #include "p2d_resources.h"
 #include "p2d_globals.h"
 #include "p2d_structs.h"
 
#endif

inline function character *
character_CreatePlayer(arena *Arena, SDL_Renderer *Renderer)
{
    character_animations *Animations = resources_LoadCharacterAnimations(Arena, Renderer);
    character *Character = (character*) ReserveMemory(Arena, sizeof(character));
    Character->X = 0;
    Character->Y = 200;
    Character->W = 15 * RENDER_SCALE;
    Character->H = 22 * RENDER_SCALE;
    Character->W_MID = Character->W / 2;
    Character->Animations = Animations;

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

inline function SDL_Rect *
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

    // Get current sprite
    animation_sprites *CurrentSprites = character_GetCurrentAnimationSprites(Character);
    SDL_Rect *CurrentRect = character_GetAnimationSpriteRect(CurrentSprites, Character->Animations->Frame);

    // Update animation
    --Character->Animations->NextUpdate;
    if (Character->Animations->NextUpdate == 0)
    {
        Character->Animations->NextUpdate = ANIMATION_NEXT_UPDATE;
        Character->Animations->Frame = (Character->Animations->Frame + 1) % CurrentSprites->Count;
    }

    return CurrentRect;
}
