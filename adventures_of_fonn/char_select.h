#ifndef CHAR_SELECT_H
#define CHAR_SELECT_H

// include stuff
#include "misc.h"
#include "LTexture.h"
#include<iostream>
#include<SDL_image.h>
#include "LButton.h"

// loads all media for character select
bool loadMediaCS();

// runs character select
bool runCharacterSelect(SDL_Window* window, SDL_Surface* screenSurface, SDL_Renderer* renderer);

#endif
