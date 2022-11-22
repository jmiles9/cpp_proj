#pragma once
#include<SDL.h>
#include "LTexture.h"
#include "misc.h"


class Component;
class Player;
class Section;


//movement patterns for enemies (?)
enum eMovementPatterns {
	MV_STATIONARY,	//does not move
	MV_SIMPLE,		//moves left and right, switching when it hits a wall
	MV_PLATFORM,	//moves left and right, switching at edge of platform
};

extern bool gHitEnemy;
extern int gEnemyName;

//this is the base of all the stuff that will render on the screen
//the "Player" class and "Block" class will build off of this probably 
class Component {
public:

	Component();

	//gets the velocity
	SDL_Point velocity();

	//gets the position
	SDL_Point position();

	//sets the position
	void setPosition(int x, int y);

	//sets the velocity
	void setVelocity(int x, int y);

	//sets the previous component
	void setPrevious(Component* component);

	//sets the next component
	void setNext(Component* component);

	//gets the previous component
	Component* getPrevious();

	//gets the next component
	Component* getNext();

	//get height and width
	int height();

	int width();

	//sets the texture and the clip to use
	void setSprite(LTexture* texture, SDL_Rect* normalSprite, SDL_Rect* size = NULL, SDL_Rect* hitbox = NULL);

	//updates stuff
	virtual void update(); 

	void render();

	virtual int getName();

	SDL_Rect mHitbox; //public bc i dont want another million get functions but should probably be private

protected:
	SDL_Point mPosition;
	SDL_Point mVelocity;

	LTexture* mTexture;
	SDL_Rect mNormalClip;

	SDL_Rect mSize;

	//these are for the purpose of rendering everything, so its a linked list
	Component* prev;
	Component* next;
};

//this will control the player character movement etc etc
class Player {
public:
	Player();

	//gets the velocity
	SDL_Point velocity();

	//gets the position
	SDL_Point position();

	//sets the texture and the clip to use
	void setSprite(LTexture* texture, SDL_Rect* normalSprite, SDL_Rect* size = NULL, SDL_Rect* hitbox = NULL);

	void handleEvent(const Uint8* keystate);

	//check for interactions with surrounding environment
	void checkInteractions(Section section);

	//this will take into account all the stuff and update pos and vel accordingly
	void update();

	void render();

private:
	Component base;

	std::string name;
	//probably will put extra sprites and stuff here but for now only one

	//these will be used to store user input to change in pos and vel
	SDL_Point deltaPos;
	SDL_Point deltaVel;

	//"constants" (these might be modified if items are picked up etc)
	int maxJumpVel;
	int maxJumpCount;

	//state variables 
	bool airborne;
	int jumpCount;

	//right and left from player perspective (on character model)
	bool mInteractionFloor;
	bool mInteractionLeft;
	bool mInteractionRight;

	int mContactSurfaceV;
	int mContactSurfaceH;

	//bool mHitEnemy; i have made these global but put them back here if decide not global
	//int mEnemyName;
};


//this will be for the enemies
class Enemy : public Component {
public:

	Enemy();

	void setName(int name);

	virtual int getName();

	void setMovePattern(int pattern);

	//check for interactions with surrounding environment
	void checkInteractions(Section section);

	//this will take into account all the stuff and update pos and vel accordingly
	virtual void update();


private:

	//this is an int because I want to use yet another enum for enemy names probably
	int mName;
	//probably will put extra sprites and stuff here but for now only one

	bool mInteractionFloor;
	bool mInteractionRight;
	bool mInteractionLeft;
	bool mInteractionEdge;

	int mContactSurfaceV;
	int mContactSurfaceH;


	//this will be the movement pattern (maybe setting velocities or whatever)
	//TODO maybe make this not an int. idk
	//for now just have a preset list to choose from ie "stationary", "between walls", etc
	int mPattern;

};


//this will hold all the blocks and stuff in a section of platforming (like a part of the map)
class Section {
public:
	Section();

	//adds a component to the section
	void addComponent(Component* component);

	void addEnemy(Enemy* enemy);

	//gets first block
	Component* getFirstBlock();

	Component* getFirstEnemy();

	//updates all component locations etc
	void update();

	//renders all components in section
	void render();

private:
	//pointer to first block in section (really its a linked list of all blocks)
	Component* mFirstBlock;
	Component* mLastBlock;

	Component* mFirstEnemy;
	Component* mLastEnemy;

	//name of section
	std::string name;
};