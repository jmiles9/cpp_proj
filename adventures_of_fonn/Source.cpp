// include stuff

#include "misc.h"
#include "LButton.h"
#include "LTexture.h"

#include "char_select.h"
#include "training.h"

#include<stdio.h>
#include<iostream>
#include<Windows.h>

// DECLARATIONS ///////////////////////////////////////////////////////////////////////////
int G_SCREEN_WIDTH_SCALED;
int G_SCREEN_HEIGHT_SCALED;
float G_SCALING_FACTOR;

enum eMainTextures {
	TEXTURE_HOME_SCREEN,
	TEXTURE_ACCEPT_JOB,
	TEXTURE_NEXT,
	TEXTURE_TOTAL, // keep this at end to keep track of how many surfaces used
};


std::string mainMedia[TEXTURE_TOTAL] = {
	"Resource Files\\welcometoubc2.png",
	"Resource Files\\acceptjob.png",
	"Resource Files\\enter.png"
};

LTexture* mainTextures[TEXTURE_TOTAL];

//The renderer
SDL_Renderer* gRenderer = NULL;

//The window we'll be rendering to
SDL_Window* window = NULL;

//The surface contained by the window
SDL_Surface* screenSurface = NULL;

// loads all media used in main.cpp
bool loadMediaMain();

// starts up SDL and creates window
bool init();

//frees media and shuts down SDL
void close();

// FUNCTIONS //////////////////////////////////////////////////////////////////////////////
bool loadMediaMain() {
	bool success = true;

	// load all textures to texture-holding array
	int count = 0;
	for (std::string fileName : mainMedia) {
		mainTextures[count] = new LTexture(gRenderer);
		if (!mainTextures[count]->loadFromFile(fileName)) {
			std::cout << "Failed to load image: " << fileName;
			success = false;
		}
		count++;
	}

	return success;
}

bool init() {
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return -1;
	}
	else {
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, G_SCREEN_WIDTH_SCALED, G_SCREEN_HEIGHT_SCALED, SDL_WINDOW_SHOWN);
		if (window == NULL) {
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			return -1;
		}
		else {
			screenSurface = SDL_GetWindowSurface(window);

			//Create renderer for window
			gRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL) {
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				return -1;
			}
			else {
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					return -1;
				}

				//Initialize SDL_ttf
				if (TTF_Init() == -1)
				{
					printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
					return -1;
				}
			}
		}
	}

	return success;
}

void close() {
	//deallocate surface
	SDL_FreeSurface(screenSurface);
	screenSurface = NULL;

	//Destroy window
	SDL_DestroyWindow(window);
	window = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

// MAIN //////////////////////////////////////////////////////////////////////////////////

int main(int argc, char* args[]) {
	//screen dimension calculations (fill height, then scale width so it is 9:16)
	int winScreenW = GetSystemMetrics(SM_CXSCREEN);
	int winScreenH = GetSystemMetrics(SM_CYSCREEN);

	int dpi = GetDpiForSystem();

	std::cout << "\n dpi window" << dpi;

	std::cout << "\n screen width: " << winScreenW;
	std::cout << "\n screen height: " << winScreenH;

	G_SCREEN_HEIGHT_SCALED = winScreenH - 50; // the 100 is so that the x is visible lol
	G_SCREEN_WIDTH_SCALED = GCS::G_SCREEN_WIDTH * G_SCREEN_HEIGHT_SCALED / GCS::G_SCREEN_HEIGHT;

	G_SCALING_FACTOR = (float) G_SCREEN_HEIGHT_SCALED / (float) GCS::G_SCREEN_HEIGHT;
	std::cout << "\n scaling factor: " << G_SCALING_FACTOR;

	std::cout << "\n scaled screen width: " << G_SCREEN_WIDTH_SCALED;
	std::cout << "\n scaled screen height: " << G_SCREEN_HEIGHT_SCALED;

	bool quit = false;
	bool next = false;

	bool testmode = true; /////////TEST MODE /////////////////////////////////////////////////
	SDL_Event e;

	// initialize
	if (!init()) {
		printf("failed to init");
	}

	// TEST MODE for testing one part at a  time 
	else if (testmode) {
		return runTraining(window, screenSurface, gRenderer);

	}

	else {
		if (!loadMediaMain()) {
			printf("failed to load media");
		}
		else {
			printf("media loaded uwu");
			SDL_RenderClear(gRenderer);

			//render homescreen background
			mainTextures[TEXTURE_HOME_SCREEN]->render(0, 0);

			//update surface
			SDL_RenderPresent(gRenderer);

			SDL_Delay(2000);

			SDL_Rect plcmnt;
			plcmnt.x = 95;
			plcmnt.y = 200;
			plcmnt.w = 630;
			plcmnt.h = 188;

			//render accept job after a short delay
			mainTextures[TEXTURE_ACCEPT_JOB]->render(plcmnt.x, plcmnt.y);
			SDL_RenderPresent(gRenderer);
		}


		// run game
		while (!quit && !next) {

			//Handle events on queue
			while (SDL_PollEvent(&e) != 0) {

				//User requests quit
				if (e.type == SDL_QUIT) {
					quit = true;
				}
				//User presses a key
				else if (e.type == SDL_KEYDOWN) {
					//Select surfaces based on key press
					switch (e.key.keysym.sym)
					{
					case SDLK_RETURN:
						next = true;
						break;

					default:
						break;
					}

				}
			}

		} // while (!quit && !next) 

		if (next) {
			bool csSuccess;
			csSuccess = runCharacterSelect(window, screenSurface, gRenderer);
		}
	}
	
	close();
	return 0;
}
