#pragma once
#ifndef MISC_H_INCLUDED
#define MISC_H_INCLUDED

#include <SDL.h>
#include <string>

namespace GCS {
	const int G_SCREEN_WIDTH = 1600; //ratios for calculating
	const int G_SCREEN_HEIGHT = 900;

	const int G_TIMESCALE = 1;
	const int G_SPACESCALE = 1;

	const int G_GRAV_ACCEL = 1;

	const float G_UPDATE_FREQ = 30;

	const float G_ENEMY_VEL = 3;
}


extern int G_SCREEN_WIDTH_SCALED;
extern int G_SCREEN_HEIGHT_SCALED;
extern float G_SCALING_FACTOR;

// for niceness later
enum eRectVals {
	X,
	Y,
	W,
	H,
};

/* loads an image to a surface and returns the optimized surface.
	input path: path (relative to /adventures_of_fonn/) where image is found
	input screenSurface: Surface to optimize the image for

	return: SDL_Surface* with image on it 
*/
SDL_Surface* loadSurface(std::string path, SDL_Surface* screenSurface);

/* loads an image to a texture and returns the texture.
	input path: path (relative to /adventures_of_fonn/) where image is found
	input windowRenderer: the renderer for the window (???)

	return: SDL_Texture* with image on it
*/
SDL_Texture* loadTexture(std::string path, SDL_Renderer* windowRenderer);


#endif