#ifndef TRAINING_H
#define TRAINING_H

// include stuff
#include "misc.h"
#include "LTexture.h"
#include<iostream>
#include<SDL_image.h>
#include "LButton.h"



// loads all media for training
bool loadMediaTraining();

// runs training
bool runTraining(SDL_Window* window, SDL_Surface* screenSurface, SDL_Renderer* renderer);

#endif