#include "training.h"
#include "stuff.h"
#include "misc.h"

#include<fstream>
#include<vector>
#include<sstream>

using namespace std;

enum eTrainingTextures {
	TEXTURE_T_FRED1,
	TEXTURE_T_PLATFORMS1,
	TEXTURE_T_NILES,
	TEXTURE_T_TOTAL,
};

enum eTrainingClips {
	CLIPS_T_FRED1,
	CLIPS_T_BLOCK1,
	CLIPS_T_FREDHITBOX,
	CLIPS_T_NILES,
	CLIPS_T_TOTAL,
};

//enemy states for enemy battle scene
enum eEnemyStates {
	STATE_E_INTRO,
	STATE_E_DIALOGUE_1,
	STATE_E_DIALOGUE_2,
	STATE_E_DIALOGUE_3,
};

const int STATE_PLATFORMING = 0;
const int STATE_ENEMYATTACK = 1;

string trainingMedia[TEXTURE_T_TOTAL] = {
	"Resource Files\\Char Select\\fredbody.png",
	"Resource Files\\Training\\platforms1.png",
	"Resource Files\\Training\\niles.png"
};

// {x, y, w, h} for clips (in order of enum!!!)
int trainingClipInfo[CLIPS_T_TOTAL][4] = {
	{0, 0, 305, 340},
	{51, 44, 227, 79},
	{178, 333, 88, 225},
	{250, 28, 300, 917}
};

SDL_Renderer* tRenderer;

LTexture* trainingTextures[TEXTURE_T_TOTAL];
SDL_Rect trainingClips[CLIPS_T_TOTAL];

//global variables
bool gHitEnemy;
int gEnemyName;

bool loadMediaTraining() {
	bool success = true;

	std::cout << "LOADING MEDIA";
	// load all textures to texture-holding array
	int count = 0;
	for (std::string fileName : trainingMedia) {
		trainingTextures[count] = new LTexture(tRenderer);
		if (!trainingTextures[count]->loadFromFile(fileName)) {
			std::cout << "Failed to load image: " << fileName;
			success = false;
		}
		count++;
	}

	// create the rects for the clips
	count = 0;
	for (auto clipInfo : trainingClipInfo) {
		trainingClips[count].x = clipInfo[X];
		trainingClips[count].y = clipInfo[Y];
		trainingClips[count].w = clipInfo[W];
		trainingClips[count].h = clipInfo[H];

		count++;
	}

	return success;
}

// takes a CSV file with the information about the blocks in a map section and converts it to blocks
vector<Component> loadMapFromCSV(string fileName) {
	// order of information: xposition,yposition,width,height,textureoption
	vector<Component> blocks;

	fstream fin;
	fin.open(fileName, ios::in);

	//vector<vector<string>> blockInfo;
	vector<string> row;
	string line, word;

	getline(fin, line); //gets rid of the first line

	while (getline(fin, line)) {
		//cout << line << endl;
		row.clear();
		stringstream s(line);

		while (getline(s, word, ',')) {
			row.push_back(word);
		}

		// create the Component and add to vector
		SDL_Rect tempScale;
		tempScale.x = stoi(row[0]) * G_SCALING_FACTOR;
		tempScale.y = stoi(row[1]) * G_SCALING_FACTOR;
		tempScale.w = stoi(row[2]) * G_SCALING_FACTOR;
		tempScale.h = stoi(row[3]) * G_SCALING_FACTOR;

		int tempTexture, tempClip;
		if (stoi(row[4]) == 0) {
			tempTexture = TEXTURE_T_PLATFORMS1;
			tempClip = CLIPS_T_BLOCK1;
		} 
		else {
			cout << "\n INVALID TEXTURE CHOICE, DEFAULT CHOSEN";
			tempTexture = TEXTURE_T_PLATFORMS1;
			tempClip = CLIPS_T_BLOCK1;
		}

		Component tempBlock;
		tempBlock.setSprite(trainingTextures[tempTexture], &trainingClips[tempClip], &tempScale);
		tempBlock.setPosition(tempScale.x, tempScale.y);

		blocks.push_back(tempBlock);
	}

	return blocks;
}

bool runTraining(SDL_Window* window, SDL_Surface* screenSurface, SDL_Renderer* renderer) {
	bool success = true;
	int state = STATE_PLATFORMING;

	tRenderer = renderer;
	if (!loadMediaTraining()) {
		success = false;
	}
	else {
		vector<Component> blocks = loadMapFromCSV("Resource Files\\Training\\maps\\map1_training.txt");

		SDL_RenderClear(tRenderer);

		//set up fred
		

		SDL_Rect fredScale;
		fredScale.x = 0;
		fredScale.y = 0;
		fredScale.w = G_SCREEN_WIDTH_SCALED / 9;
		fredScale.h = G_SCREEN_HEIGHT_SCALED / 6;

		Player fred;
		fred.setSprite(trainingTextures[TEXTURE_T_FRED1], &trainingClips[CLIPS_T_FRED1], 
			&fredScale, &trainingClips[CLIPS_T_FREDHITBOX]);
		fred.render();

		//setting up blocks in the section (TODO: make a function for this)
		Section testSection;

		/*SDL_Rect block1Scale;
		block1Scale.x = 0;
		block1Scale.y = 0;
		block1Scale.w = G_SCREEN_WIDTH_SCALED / 10;
		block1Scale.h = G_SCREEN_HEIGHT_SCALED / 12;

		Component tempBlock;
		tempBlock.setSprite(trainingTextures[TEXTURE_T_PLATFORMS1], &trainingClips[CLIPS_T_BLOCK1], &block1Scale);
		tempBlock.setPosition(400, 600);

		Component tempBlock2;
		tempBlock2.setSprite(trainingTextures[TEXTURE_T_PLATFORMS1], &trainingClips[CLIPS_T_BLOCK1], &block1Scale);
		tempBlock2.setPosition(600, 400);

		Component tempBlock3;
		tempBlock3.setSprite(trainingTextures[TEXTURE_T_PLATFORMS1], &trainingClips[CLIPS_T_BLOCK1], &block1Scale);
		tempBlock3.setPosition(800, G_SCREEN_HEIGHT_SCALED-block1Scale.h);
		
		testSection.addComponent(&tempBlock);
		testSection.addComponent(&tempBlock2);
		testSection.addComponent(&tempBlock3);*/

		for (Component c : blocks) {
			testSection.addComponent(&c);
		}
		
		
		SDL_Rect nilesScale;
		nilesScale.x = 0;
		nilesScale.y = 0;
		nilesScale.w = 300 / 7;
		nilesScale.h = 917 / 7;

		Enemy tempEnemy;
		tempEnemy.setSprite(trainingTextures[TEXTURE_T_NILES], &trainingClips[CLIPS_T_NILES], &nilesScale);
		tempEnemy.setPosition(350, 250);
		tempEnemy.setMovePattern(MV_PLATFORM);
		
		testSection.addEnemy(&tempEnemy);

		testSection.render();

		SDL_RenderPresent(tRenderer);

		SDL_Event e;

		//timing variables
		float timeStepMs = 1000.f / GCS::G_UPDATE_FREQ;
		float timeLastMs = 0;
		float timeCurrentMs = 0;
		float timeDeltaMs = 0;
		float timeAccumMs = 0;

		bool quit = false;
		int enemyState = STATE_E_INTRO;

		const Uint8* keystate = SDL_GetKeyboardState(nullptr);
		bool temp = false;

		while (!quit) {
			//std::cout << ".";

			//loop timing
			timeLastMs = timeCurrentMs;
			timeCurrentMs = SDL_GetTicks();
			timeDeltaMs = timeCurrentMs - timeLastMs;
			timeAccumMs += timeDeltaMs;

			while (timeAccumMs >= timeStepMs && !quit && state == STATE_PLATFORMING) {
				//std::cout << "\nSTEP\n";

				SDL_GetKeyboardState(nullptr);

				while (SDL_PollEvent(&e) != 0) {
					// quit is pressed
					if (e.type == SDL_QUIT) {
						quit = true;
						std::cout << "QUITTING";
						break;
					}

					// handle events with the player
					//fred.handleEvent(&e, keystate);
					//keysHeld = 0;
				}
				fred.handleEvent(keystate);

				SDL_RenderClear(tRenderer);
				fred.checkInteractions(testSection);
				fred.update(); ///TODO move this to section update

				tempEnemy.checkInteractions(testSection);
				testSection.update();
				testSection.render();
				fred.render(); //TODO also move this, probably
				SDL_RenderPresent(tRenderer);


				if (gHitEnemy && !temp) {
					enemyState = STATE_E_INTRO;
					state = STATE_ENEMYATTACK;
					temp = true; // makes it only happen once for debugging purposes
					//despawn enemy so it is not immediately hit on return to platforming
					//TODO maybe the enemy that is hit has to be passed somewhere so it can be removed
				}
				
				timeAccumMs -= timeStepMs;
			} // while state == PLATFORMING 

			//maybe move this to previous loop and have an if statement instead
			while (timeAccumMs >= timeStepMs && !quit && state == STATE_ENEMYATTACK) {
				bool validInput = false;

 				while (SDL_PollEvent(&e) != 0) {
					//std::cout << "\n BUTTON DETECTED";
					// quit is pressed
					if (e.type == SDL_QUIT) {
						quit = true;
						std::cout << "QUITTING";
						break;
					}
					//TODO handle event here (changes states or sth
					else if (e.type == SDL_KEYDOWN && !validInput) {
						switch (enemyState) {
							std::cout << "\n in switch";
						case STATE_E_INTRO:
							std::cout << "\n INTRO";
							if (e.key.keysym.sym == SDLK_w) {
								enemyState = STATE_E_DIALOGUE_1;
								validInput = true;
							}
							//TODO figure out dialogue patterns and do this ig
							break;

						case STATE_E_DIALOGUE_1:
							if (e.key.keysym.sym == SDLK_w) {
								enemyState = STATE_E_DIALOGUE_2;
								validInput = true;
							}
							break;

						case STATE_E_DIALOGUE_2:
							if (e.key.keysym.sym == SDLK_w) {
								state = STATE_PLATFORMING;
								validInput = true;
							}
							break;

						case STATE_E_DIALOGUE_3:

							break;

						default:
							enemyState = STATE_E_INTRO;
							break;
						}
					}

					// to prevent holding down the key doing multiple times, flush event queue after valid input
					if (validInput) {
						SDL_PumpEvents();
						SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
					}
				}

				SDL_RenderClear(tRenderer);

				switch (enemyState) {
				case STATE_E_INTRO:
					std::cout << "\n 0";
					break;

				case STATE_E_DIALOGUE_1:
					std::cout << "\n 1";
					break;

				case STATE_E_DIALOGUE_2:
					std::cout << "\n 2";
					break;

				case STATE_E_DIALOGUE_3:

					break;

				default:
					break;
				}
				
				trainingTextures[TEXTURE_T_NILES]->render(0, 0, &trainingClips[TEXTURE_T_NILES]);
				trainingTextures[TEXTURE_T_NILES]->render(0, 0);
				SDL_RenderPresent(tRenderer);

				timeAccumMs -= timeStepMs;
			} //while state == ENEMYATTACK
		}
	}
	return success;
}
