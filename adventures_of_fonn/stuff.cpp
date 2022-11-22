#include "stuff.h"
#include<iostream>




// COMPONENT class ////////////////////////////////////////////////////////////////
Component::Component() {
	mPosition.x = 0;
	mPosition.y = 0;

	mVelocity.x = 0;
	mVelocity.y = 0;

	mHitbox.x = 0;
	mHitbox.y = 0;
	mHitbox.w = 0;
	mHitbox.h = 0;

	mTexture = NULL;

	mNormalClip.x = 0;
	mNormalClip.y = 0;
	mNormalClip.w = 0;
	mNormalClip.h = 0;

	mSize.x = 0;
	mSize.y = 0;
	mSize.w = 0;
	mSize.h = 0;

	prev = NULL;
	next = NULL;
}

SDL_Point Component::position() {
	return mPosition;
}


SDL_Point Component::velocity() {
	return mVelocity;
}

void Component::setPosition(int x, int y) {
	mPosition.x = x;
	mPosition.y = y;
}

void Component::setVelocity(int x, int y) {
	mVelocity.x = x;
	mVelocity.y = y;
}

void Component::setPrevious(Component* component) {
	prev = component;
}

void Component::setNext(Component* component) {
	next = component;
}

Component* Component::getPrevious() {
	return prev;
}

Component* Component::getNext() {
	return next;
}

int Component::height() {
	return mSize.h;
}

int Component::width() {
	return mSize.w;
}

void Component::setSprite(LTexture* texture, SDL_Rect* normalSprite, SDL_Rect* size, SDL_Rect* hitbox) {
	mTexture = texture;

	mNormalClip.x = normalSprite->x;
	mNormalClip.y = normalSprite->y;
	mNormalClip.w = normalSprite->w;
	mNormalClip.h = normalSprite->h;

	if (size == NULL) {
		mSize.x = normalSprite->x;
		mSize.y = normalSprite->y;
		mSize.w = normalSprite->w;
		mSize.h = normalSprite->h;
	}
	else {
		mSize.x = size->x;
		mSize.y = size->y;
		mSize.w = size->w;
		mSize.h = size->h;
	}

	if (hitbox == NULL) {
		mHitbox.x = mSize.x;
		mHitbox.y = mSize.y;
		mHitbox.w = mSize.w;
		mHitbox.h = mSize.h;
	}
	else {
		mHitbox.x = hitbox->x;
		mHitbox.y = hitbox->y;
		mHitbox.w = hitbox->w;
		mHitbox.h = hitbox->h;
	}

	//TODO IF YOU WANT A DIFFERENT SIZE HITBOX YOU ALSO HAVE TO CALCULATE WHERE IT IS IN RELATION
	//TO THE SPRITE POSITION ETC ETC
}

// use setPosition and setVelocity ??????
/*void Component::update(SDL_Point pos, SDL_Point vel) {
	//this is where all the velocity and position stuff gets updated
	mPosition = pos;
	mVelocity = vel;
}*/

void Component::update() {
	//this is where all the velocity and position stuff gets updated
	std::cout << "\nfor some reason we are in Component::update";
}

void Component::render() {
	//take updated position and render it ????????
	if (mTexture == NULL) std::cout << "Remember to set the texture!!!";
	else mTexture->render(mPosition.x, mPosition.y, &mNormalClip, &mSize);
}

int Component::getName() {
	std::cout << "\nin Component::getName (this should never happen pls)";
	return 0;
}



//PLAYER CLASS//////////////////////////////////////////////////////////////////////
Player::Player() {
	name = "OOGA BOOGA";

	deltaPos.x = 0;
	deltaPos.y = 0;

	deltaVel.x = 0;
	deltaVel.y = 0;

	maxJumpVel = 4; //TODO change this to const 
	maxJumpCount = 6;

	airborne = false;
	jumpCount = 0;
	mInteractionFloor = false;
	mInteractionLeft = false;
	mInteractionRight = false;

	mContactSurfaceV = 0;
	mContactSurfaceH = 0;

}

SDL_Point Player::velocity() {
	return base.velocity();
}

SDL_Point Player::position() {
	return base.position();
}

void Player::setSprite(LTexture* texture, SDL_Rect* normalSprite, SDL_Rect* size, SDL_Rect* hitbox) {
	//TODO probably have two separate functions, one to store the needed sprites and
	//one to update the used sprite
	base.setSprite(texture, normalSprite, size, hitbox);
}

void Player::handleEvent(const Uint8* keystate) {


	//continuous-response keys
	if (keystate[SDL_SCANCODE_D] && !mInteractionRight) {
		deltaPos.x = 5;
		//std::cout << "\nMOVING RIGHT CONT";
	}
	if (keystate[SDL_SCANCODE_A] && !mInteractionLeft) {
		deltaPos.x = -5;
		//std::cout << "\nMOVING LEFT CONT";
	}
	if (keystate[SDL_SCANCODE_SPACE]) {
		//if the player has already jumped off the ground
		//TODO when space is released it should stop the airborne if from happening 
		if (airborne) {
			//increase upward velocity, unless the player has already reached their max. jump height
			//(since holding space should make you jump longer)
			if (jumpCount < maxJumpCount) {
				if (maxJumpCount - jumpCount > maxJumpVel) deltaVel.y -= maxJumpVel;
				else deltaVel.y -= maxJumpCount - jumpCount;
				jumpCount++;
				//std::cout << "\nEXTENDING JUMP";
			}
		}
		//if the player is grounded
		else {
			airborne = true;
			jumpCount = 0;

			if (deltaVel.y < maxJumpVel) {
				deltaVel.y -= 2;
				//std::cout << "\nJUMPING";
			}
		}
	}
	if (keystate[SDL_SCANCODE_S]) {
		deltaPos.y = 1;
		//std::cout << "\nMOVING DOWN CONT";
	}
	//single-press keys (dont need this for moving but will probably need it for something else at some point)
	/*
	switch (e->type) {
		// Look for a keypress 
		case SDL_KEYDOWN:
			//Check the SDLKey values and move change the coords 
			switch (e->key.keysym.sym) {
		
			//moving right
				case SDLK_d:
					// for now, just directly modify position
					deltaPos.x = 1;
					std::cout << "\nMOVING RIGHT";
					break;

				//moving left
				case SDLK_a:
					deltaPos.x = -1;
					std::cout << "\nMOVING LEFT";
					break;

				//jumping
				case SDLK_SPACE:
					deltaPos.y = -1;
					std::cout << "\nMOVING UP";
					break;

				//crouching (no velocity change, will change sprite in the future
				case SDLK_s:
					//TODO
					// temporary: make it move down
					deltaPos.y = 1;
					std::cout << "\nMOVING DOWN";
					break;

				default:
					break;
			}
		}*/
	
}

void Player::checkInteractions(Section section) {
	//should go thru all components in the section and see if the hitboxes are overlapping
	Component* nextBlock = section.getFirstBlock();
	bool hitV = false;
	bool hitH = false;

	int playerPosX1 = base.position().x;
	int playerPosX2 = playerPosX1 + base.width();
	int playerPosY1 = base.position().y + base.height();
	int playerPosY2 = base.position().y; //top

	int blockTop, blockBtm, blockL, blockR;

	while (nextBlock != NULL) {
		//check for interactions with that block
		//if bottom part of sprite hitbox is in the block
		
		blockTop = nextBlock->position().y;
		blockBtm = blockTop + nextBlock->height();
		blockL = nextBlock->position().x;
		blockR = blockL + nextBlock->width();


		//TODO make a function to check for overlap between two SDL_Rects and just do that or sth
		//vertical interaction
		if (playerPosY1 >= blockTop && playerPosY1 <= blockBtm && playerPosX2 > blockL && playerPosX1 < blockR) {
			// yes overlap
			hitV = true;
			mInteractionFloor = true;
			//std::cout << "\nOVERLAP DETECTED: BOTTOM";
			mContactSurfaceV = blockTop;
		} //TODO THESE ARE ALMSOT THE EXACT SAME ITS CRAZY THAT THIS WORKS AS INTENDED AT ALL
		else if (playerPosY1 > blockTop && playerPosY2 < blockBtm && playerPosX1 < blockR && playerPosX2 > blockL) {
			hitH = true;
			if (playerPosX2 < blockR) { //moving right
				mInteractionRight = true;
				mInteractionLeft = false;
				mContactSurfaceH = blockL;
				//std::cout << "\nOVERLAP DETECTED: RIGHT SIDE";
			}
			else if (playerPosX1 > blockL) { //moving left 
				mInteractionLeft = true;
				mInteractionRight = false;
				mContactSurfaceH = blockR;
				//std::cout << "\nOVERLAP DETECTED: LEFT SIDE";
			}

		}

		//get next block
		nextBlock = nextBlock->getNext();
	}

	if (!hitV) mInteractionFloor = false;
	if (!hitH) {
		mInteractionLeft = false;
		mInteractionRight = false;
		//std::cout << "\nNO HIT H";
	}


	// check enemy interactions
	Component* nextEnemy = section.getFirstEnemy();
	gHitEnemy = false;
	gEnemyName = 0;

	while (nextEnemy != NULL) {
		blockTop = nextEnemy->position().y;
		blockBtm = blockTop + nextEnemy->height();
		blockL = nextEnemy->position().x;
		blockR = blockL + nextEnemy->width();

		//if there is any overlap at all
		if (playerPosY1 >= blockTop && playerPosY1 <= blockBtm && playerPosX2 >= blockL && playerPosX1 <= blockR) {
			gHitEnemy = true;
			gEnemyName = nextEnemy->getName();
			std::cout << "\n ENEMY HIT";
			break;
		}

		nextEnemy = nextEnemy->getNext();
	}

}

void Player::update() {
	SDL_Point newPos;
	SDL_Point newVel;

	SDL_Point oldPos = base.position();
	SDL_Point oldVel = base.velocity();

	//uses old vel to calc new pos
	//dont use x velocity, it is only for indicating what direction it is moving (?) since no inertia (?) horizontally
	//newPos.x = oldPos.x + ( oldVel.x / GCS::G_TIMESCALE + deltaPos.x ) * GCS::G_SPACESCALE;
	newPos.x = oldPos.x + deltaPos.x * GCS::G_SPACESCALE;
	newPos.y = oldPos.y + ( oldVel.y / GCS::G_TIMESCALE + deltaPos.y ) * GCS::G_SPACESCALE;
	

	newVel.x = oldVel.x;// +deltaVel.x * GCS::G_SPACESCALE;
	newVel.y = oldVel.y + ( deltaVel.y + GCS::G_GRAV_ACCEL / GCS::G_TIMESCALE) * GCS::G_SPACESCALE;

	//check for interactions with the surrounding environment that would override pos or vel changes
	//checking for being at screen borders (TODO change this ig)
	if (newPos.x < 0) newPos.x = 0;
	if (newPos.x > G_SCREEN_WIDTH_SCALED) newPos.x = G_SCREEN_WIDTH_SCALED;
	if (newPos.y < 0) newPos.y = 0;
	if (newPos.y > G_SCREEN_HEIGHT_SCALED - base.height()) {
		newPos.y = G_SCREEN_HEIGHT_SCALED - base.height();
		std::cout << "\nBOTTOM HIT";
	}

	if (newPos.y >= G_SCREEN_HEIGHT_SCALED - base.height() && newVel.y > 0) {
		newVel.y = 0;
		airborne = false; //TODO this should go somewhere else once platform physics!!
		//std::cout << "\nVEL SET TO 0";
	}

	//checking for block interactions vertically
	if (mInteractionFloor && newVel.y > 0) {
		newVel.y = 0;
		newPos.y = mContactSurfaceV - base.height();
		airborne = false;
		//std::cout << "\nHIT PLATFORM";
	}

	//check for block interactions horizontally
	//TODO change this so it lets you move away from the wall

	//moving to the right
	if (mInteractionRight) {
		newVel.x = 0;
		newPos.x = mContactSurfaceH - base.width();
		std::cout << "\nHIT RIGHT SIDE";
	}
	//moving to the left (right now this is redundant idk if I could just combine them)
	if (mInteractionLeft) {
		newVel.x = 0;
		newPos.x = mContactSurfaceH; 
		std::cout << "\nHIT LEFT SIDE";
	}

	//TODO calc new vel based on oldVel and inputs or acceleration ig (maybe this should happen 
	//in a handle inputs function or sth? or just have a deltaVel from inputs and also use that here
	//base.update(newPos, newVel);
	base.setPosition(newPos.x, newPos.y);
	base.setVelocity(newVel.x, newVel.y);

	deltaPos.x = 0;
	deltaPos.y = 0;

	deltaVel.x = 0;
	deltaVel.y = 0;

}

void Player::render() {
	base.render();
}


//ENEMY CLASS//////////////////////////////////////////////////////////////////////
Enemy::Enemy() {
	mName = 0;

	mInteractionFloor = false;
	mInteractionLeft = false;
	mInteractionRight = false;
	mInteractionEdge = false;

	mContactSurfaceV = 0;
	mContactSurfaceH = 0;

	mPattern = MV_STATIONARY;
}

void Enemy::setName(int name) {
	mName = name;
}

int Enemy::getName() {
	return mName;
}

void Enemy::setMovePattern(int pattern) {
	mPattern = pattern;

	switch (mPattern) {
	case MV_STATIONARY:
		setVelocity(0, 0);
		break;

	case MV_SIMPLE:
		setVelocity(GCS::G_ENEMY_VEL, 0);
		break;

	case MV_PLATFORM:
		setVelocity(GCS::G_ENEMY_VEL, 0);
		break;

	default:
		setVelocity(0, 0);
		break;
	}
}


void Enemy::checkInteractions(Section section) {
	//should go thru all components in the section and see if the hitboxes are overlapping
	Component* nextBlock = section.getFirstBlock();
	bool hitV = false;
	bool hitH = false;

	while (nextBlock != NULL) {
		//check for interactions with that block
		//if bottom part of sprite hitbox is in the block
		int playerPosX1 = position().x;
		int playerPosX2 = playerPosX1 + width();
		int playerPosY1 = position().y + height();
		int playerPosY2 = position().y; //top

		int blockTop = nextBlock->position().y;
		int blockBtm = blockTop + nextBlock->height();
		int blockL = nextBlock->position().x;
		int blockR = blockL + nextBlock->width();


		//TODO make a function to check for overlap between two SDL_Rects and just do that or sth
		//vertical interaction
		if (playerPosY1 >= blockTop && playerPosY1 <= blockBtm && playerPosX2 > blockL && playerPosX1 < blockR) {
			// yes overlap
			hitV = true;
			mInteractionFloor = true;
			//std::cout << "\nOVERLAP DETECTED: BOTTOM";
			mContactSurfaceV = blockTop;

			//check if it is on the edge for movement purposes
			mInteractionEdge = (playerPosX1 < blockL || playerPosX2 > blockR);
		}
		else if (playerPosY1 > blockTop && playerPosY2 < blockBtm && playerPosX1 < blockR && playerPosX2 > blockL) {
			hitH = true;
			if (playerPosX2 < blockR) { //moving right
				mInteractionRight = true;
				mInteractionLeft = false;
				mContactSurfaceH = blockL;
				//std::cout << "\nOVERLAP DETECTED: RIGHT SIDE";
			}
			else if (playerPosX1 > blockL) { //moving left 
				mInteractionLeft = true;
				mInteractionRight = false;
				mContactSurfaceH = blockR;
				//std::cout << "\nOVERLAP DETECTED: LEFT SIDE";
			}

		}

		//get next block
		nextBlock = nextBlock->getNext();
	}

	if (!hitV) mInteractionFloor = false;
	if (!hitH) {
		mInteractionLeft = false;
		mInteractionRight = false;
		//std::cout << "\nNO HIT H";
	}
}


void Enemy::update() {
	SDL_Point newPos;
	SDL_Point newVel;

	SDL_Point oldPos = position();
	SDL_Point oldVel = velocity();

	bool hit = false;

	//uses old vel to calc new pos
	newPos.x = oldPos.x + oldVel.x / GCS::G_TIMESCALE * GCS::G_SPACESCALE;
	newPos.y = oldPos.y + oldVel.y / GCS::G_TIMESCALE * GCS::G_SPACESCALE;

	//initialize them so it doesn't complain
	newVel.y = oldVel.y;
	newVel.x = oldVel.x;


	//check for interactions with the surrounding environment that would override pos or vel changes
	//checking for being at screen borders (TODO change this ig)
	if (newPos.x < 0) {
		newPos.x = 0;
		hit = true;
	}
	if (newPos.x > G_SCREEN_WIDTH_SCALED - width()) {
		newPos.x = G_SCREEN_WIDTH_SCALED - width();
		hit = true;
	}
	if (newPos.y < 0) newPos.y = 0;
	if (newPos.y > G_SCREEN_HEIGHT_SCALED - height()) {
		newPos.y = G_SCREEN_HEIGHT_SCALED - height();
		std::cout << "\nBOTTOM HIT";
	}

	// if a condition is met to change the velocity, then change it

	//check wall interactions first
	if (mInteractionRight) {
		hit = true;
		newPos.x = mContactSurfaceH - width();
		//std::cout << "\nHIT RIGHT SIDE";
	}
	if (mInteractionLeft) {
		hit = true;
		newPos.x = mContactSurfaceH;
		//std::cout << "\nHIT RIGHT SIDE";
	}

	switch (mPattern) {
	case MV_STATIONARY:
		//velocity should always be 0, so don't do anything
		newVel.x = 0;
		//newVel.y = 0;
		break;

	case MV_SIMPLE:
		//it should pace back and forth, turning around if it hits a wall
		//if (mInteractionLeft || mInteractionRight) hit = true;

		

		//if (check wall hit) (actually probably check wall hit before switch case)
		//  switch velocity direction (newVel = -oldVel)
		std::cout <<"\n"<< hit;
		if (hit) {
			newVel.x = -oldVel.x;
			//newVel.y = oldVel.y;
		}
		break;

	case MV_PLATFORM:
		//it should pace back and forth, turning around if it hits a wall or a platform
		
		//if (about to fall off) (going to need to code this lol) 
		//   switch velocity direction
		if (hit || mInteractionEdge) {
			newVel.x = -oldVel.x;
			//move it backwards (if this is buggy add a variable like contactsurfaceH but for edges)
			newPos.x = oldPos.x - oldVel.x / GCS::G_TIMESCALE * GCS::G_SPACESCALE;
		}
		//newVel.y = 0;
		break;

	default:
		//dont move (same as MV_STATIONARY for now at least)
		newVel.x = 0;
		//newVel.y = 0;
		break;
	}

	//if it is falling, increase velocity down, else set to 0
	if (!mInteractionFloor) {
		newVel.y = oldVel.y + (GCS::G_GRAV_ACCEL / GCS::G_TIMESCALE) * GCS::G_SPACESCALE;
	}
	else newVel.y = 0;

	

	if (newPos.y >= G_SCREEN_HEIGHT_SCALED - height() && newVel.y > 0) {
		newVel.y = 0;
		//airborne = false; //TODO this should go somewhere else once platform physics!!
		// might need the airborne for MV_PLATFORM.. idk
		//std::cout << "\nVEL SET TO 0";
	}

	//checking for block interactions vertically
	//there was an && newVel.y > 0 but im not sure why so I took it out
	if (mInteractionFloor) {
		newVel.y = 0;
		newPos.y = mContactSurfaceV - height();
		//airborne = false;
		//std::cout << "\nHIT PLATFORM";
	}

	//check for block interactions horizontally
	//TODO change this so it lets you move away from the wall
/* i think this is only relevant to player class
	//moving to the right
	if (mInteractionRight) {
		newVel.x = 0;
		newPos.x = mContactSurfaceH - width();
		std::cout << "\nHIT RIGHT SIDE";
	}
	//moving to the left (right now this is redundant idk if I could just combine them)
	if (mInteractionLeft) {
		newVel.x = 0;
		newPos.x = mContactSurfaceH;
		std::cout << "\nHIT LEFT SIDE";
	}*/

	//TODO calc new vel based on oldVel and inputs or acceleration ig (maybe this should happen 
	//in a handle inputs function or sth? or just have a deltaVel from inputs and also use that here
	//Component::update(newPos, newVel);
	setPosition(newPos.x, newPos.y);
	setVelocity(newVel.x, newVel.y);


}



//SECTION CLASS/////////////////////////////////////////////////////////////////////
Section::Section() {
	name = "SECTION NAME HERE";

	mFirstBlock = NULL;
	mLastBlock = NULL;

	mFirstEnemy = NULL;
	mLastEnemy = NULL;
}

void Section::addComponent(Component* component) {
	if (mFirstBlock == NULL) {
		mFirstBlock = component;
		mLastBlock = component;
	}
	else {
		//add it to end
		mLastBlock->setNext(component);
		component->setPrevious(mLastBlock);
		mLastBlock = component;
	}
}

void Section::addEnemy(Enemy* enemy) {
	if (mFirstEnemy == NULL) {
		mFirstEnemy = enemy;
		mLastEnemy = enemy;
	}
	else { 
		//add it to end
		mLastEnemy->setNext(enemy);
		enemy->setPrevious(mLastEnemy);
		mLastEnemy = enemy;
	}
}

Component* Section::getFirstBlock() {
	return mFirstBlock;
}

Component* Section::getFirstEnemy() {
	return mFirstEnemy;
}

void Section::update() {
	//updates all enemy and block positions
	//TODO block stuff ig if wanting moving camera or sth

	//probably if updating blocks its faster to check if they need to be updated first

	//update enemies
	Component* nextEnemy = mFirstEnemy;
	while (nextEnemy != NULL) {
		nextEnemy->update();
		nextEnemy = nextEnemy->getNext();
	}
}

void Section::render() {
	//render each block by going thru linked list of blocks
	Component* nextBlock = mFirstBlock;
	while (nextBlock != NULL) {
		nextBlock->render();
		nextBlock = nextBlock->getNext();
	}

	//render enemies
	Component* nextEnemy = mFirstEnemy;
	while (nextEnemy != NULL) {
		nextEnemy->render();
		nextEnemy = nextEnemy->getNext();
	}

	//TODO render player (currently renders seperately)
}