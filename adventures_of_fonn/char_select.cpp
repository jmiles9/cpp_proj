#include "char_select.h"
#include "misc.h"

// DECLARATIONS ///////////////////////////////////////////////////////////////////////////
enum eCharSelectTextures {
	TEXTURE_CS_FRED,
	TEXTURE_CS_BUTTONS,
	TEXTURE_CS_TOTAL,
};

enum eCharSelectFonts {
	FONT_CS_COMICSANS,
	FONT_CS_TOTAL,
};

enum eCharSelectClips {
	CLIPS_CS_STARTBUTTON,
	CLIPS_CS_STARTBUTTON_P,
	CLIPS_CS_STARTBUTTON_HOVER,
	CLIPS_CS_IMABUTTON,
	CLIPS_CS_TOTAL,
};


// could not include size but idk if it is better to have an exception thrown if i forget to
// add one here, or if it is better to have always the correct size array but with garbage values
std::string charSelectMedia[TEXTURE_CS_TOTAL] = {
	"Resource Files\\Char Select\\fredbody.png",
	"Resource Files\\Char Select\\buttons1.png"
};

std::string charSelectFonts[FONT_CS_TOTAL] = {
	"Resource Files\\Fonts\\LDFComicSans.ttf"
};

// {x, y, w, h} for clips
int charSelectClipInfo[CLIPS_CS_TOTAL][4] = {
	{35, 31, 298, 129},
	{36, 190, 298, 129},
	{37, 349, 298, 129},
	{530, 25, 375, 150}
};

SDL_Renderer* csRenderer;

// stores all loaded textures
LTexture* csTextures2[TEXTURE_CS_TOTAL];

// stores all loaded fonts
TTF_Font* csFonts[FONT_CS_TOTAL];

// stores clip rects 
SDL_Rect charSelectClips[CLIPS_CS_TOTAL];

// start button (replace this with same enum-list format if there are a lot of buttons)
LButton startButton;


// FUNCTIONS //////////////////////////////////////////////////////////////////////////////

bool loadMediaCS() {
	bool success = true;

	// load all textures to texture-holding array
	int count = 0;
	for (std::string fileName : charSelectMedia) {
		csTextures2[count] = new LTexture(csRenderer);
		if (!csTextures2[count]->loadFromFile(fileName)) {
			std::cout << "Failed to load image: " << fileName;
			success = false;
		}
		count++;
	}

	// load all fonts to the font house
	count = 0;
	for (std::string fileName : charSelectFonts) {
		csFonts[count] = TTF_OpenFont(fileName.c_str(), 20);
		if (csFonts[count] == NULL) {
			std::cout << "Failed to load font: " << fileName;
			success = false;
		}
		count++;
	}

	// create the rects for the clips
	count = 0;
	for (auto clipInfo : charSelectClipInfo) {
		charSelectClips[count].x = clipInfo[X];
		charSelectClips[count].y = clipInfo[Y];
		charSelectClips[count].w = clipInfo[W];
		charSelectClips[count].h = clipInfo[H];

		count++;
	}

	return success;
}

// takes a window and enacts the character selection on it
bool runCharacterSelect(SDL_Window* window, SDL_Surface* screenSurface, SDL_Renderer* renderer) {
	bool quit = false;
	
	csRenderer = renderer;

	// load all media
	int count = 0;
	if (!loadMediaCS()) {
		std::cout << "something went wrong loading textures or fonts \n";
		return -1;
	}
	else {
		std::cout << "all media loaded uwu";
	}

	// set up the viewports
	SDL_Rect screenViewport;
	screenViewport.x = 0;
	screenViewport.y = 0;
	screenViewport.w = GCS::G_SCREEN_WIDTH;
	screenViewport.h = GCS::G_SCREEN_HEIGHT;

	SDL_Rect topViewport;
	topViewport.x = 0;
	topViewport.y = 0;
	topViewport.w = GCS::G_SCREEN_WIDTH;
	topViewport.h = GCS::G_SCREEN_HEIGHT / 5;

	SDL_Rect characterViewport;
	characterViewport.x = 0;
	characterViewport.y = topViewport.h;
	characterViewport.w = GCS::G_SCREEN_WIDTH / 2;
	characterViewport.h = GCS::G_SCREEN_HEIGHT - characterViewport.y;

	SDL_Rect startViewport;
	startViewport.x = characterViewport.w;
	startViewport.w = GCS::G_SCREEN_WIDTH - characterViewport.w;
	startViewport.h = GCS::G_SCREEN_HEIGHT / 3;
	startViewport.y = GCS::G_SCREEN_HEIGHT - startViewport.h;


	SDL_Rect choiceViewport;
	choiceViewport.x = characterViewport.w;
	choiceViewport.y = topViewport.h;
	choiceViewport.w = GCS::G_SCREEN_WIDTH - characterViewport.w;
	choiceViewport.h = characterViewport.h - startViewport.h;

	// set up screen textures that aren't already set up (eg text)
	SDL_Color tempColour = { 0, 0, 0 };
	LTexture tempText = LTexture(csRenderer);
	tempText.loadFromRenderedText("ooga booga", tempColour, csFonts[FONT_CS_COMICSANS]);

	LTexture tempText2 = LTexture(csRenderer);
	tempText2.loadFromRenderedText("CHOOSE UR CHARACTER", tempColour, csFonts[FONT_CS_COMICSANS]);

	LTexture tempText3 = LTexture(csRenderer);
	tempText3.loadFromRenderedText("START", tempColour, csFonts[FONT_CS_COMICSANS]);

	startButton.setSprite(&charSelectClips[CLIPS_CS_STARTBUTTON], 
		&charSelectClips[CLIPS_CS_STARTBUTTON_P],
		&charSelectClips[CLIPS_CS_STARTBUTTON_HOVER]);

	startButton.setTexture(csTextures2[TEXTURE_CS_BUTTONS]);
	startButton.setPosition(startViewport.x, startViewport.y);

	SDL_Event e;

	while (!quit) {
		// handle events on queue
		while (SDL_PollEvent(&e) != 0) {
			// quit is pressed
			if (e.type == SDL_QUIT) {
				quit = true;
			}

			// handle events with the start button
			startButton.handleEvent(&e);
			
			if (startButton.clicked()) {
				std::cout << "CLICKED \n";
			}
		}


		
		//Clear screen
		SDL_SetRenderDrawColor(renderer, 0xAF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(renderer);

		SDL_RenderSetViewport(csRenderer, &screenViewport);
		startButton.render();

		//Render fred to screen
		SDL_RenderSetViewport(csRenderer, &characterViewport);
		csTextures2[TEXTURE_CS_FRED]->render(characterViewport.w / 2 - csTextures2[TEXTURE_CS_FRED]->getWidth() / 2, 
			characterViewport.h / 2 - csTextures2[TEXTURE_CS_FRED]->getHeight() / 2);

		// render the textures
		SDL_RenderSetViewport(csRenderer, &choiceViewport);
		tempText.render(0, 0);

		SDL_RenderSetViewport(csRenderer, &topViewport);
		tempText2.render(topViewport.w / 2 - tempText2.getWidth() / 2, 
			topViewport.h / 2 - tempText2.getHeight() / 2);

		//Render start button
		//std::cout << "\n START BUTTON 
		//SDL_RenderSetViewport(csRenderer, &screenViewport);
		
		
		//Update screen
		SDL_RenderPresent(renderer);

	} // while (!quit)

	return 0;
}
