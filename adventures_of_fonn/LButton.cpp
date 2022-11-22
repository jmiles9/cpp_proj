#include "LButton.h"
#include<iostream>

LButton::LButton() {
	mPosition.x = 0;
	mPosition.y = 0;

	mCurrentSprite.x = 0;
	mCurrentSprite.y = 0;
	mCurrentSprite.w = 0;
	mCurrentSprite.h = 0;

	mPressedSprite.x = 0;
	mPressedSprite.y = 0;
	mPressedSprite.w = 0;
	mPressedSprite.h = 0;

	mHoverSprite.x = 0;
	mHoverSprite.y = 0;
	mHoverSprite.w = 0;
	mHoverSprite.h = 0;

	mClicked = UNPRESSED;
	mHovered = false;

	mTexture = NULL;
}

void LButton::setPosition(int x, int y) {
	mPosition.x = x;
	mPosition.y = y;
}

void LButton::setSprite(SDL_Rect* sprite, SDL_Rect* sprite2, SDL_Rect* hoverSprite) {
	mCurrentSprite.x = sprite->x;
	mCurrentSprite.y = sprite->y;
	mCurrentSprite.w = sprite->w;
	mCurrentSprite.h = sprite->h;

	if (sprite2 == NULL) {
		mPressedSprite.x = sprite->x;
		mPressedSprite.y = sprite->y;
		mPressedSprite.w = sprite->w;
		mPressedSprite.h = sprite->h;
	}
	else {
		mPressedSprite.x = sprite2->x;
		mPressedSprite.y = sprite2->y;
		mPressedSprite.w = sprite2->w;
		mPressedSprite.h = sprite2->h;
	}

	if (hoverSprite == NULL) {
		mHoverSprite.x = sprite->x;
		mHoverSprite.y = sprite->y;
		mHoverSprite.w = sprite->w;
		mHoverSprite.h = sprite->h;
	}
	else {
		mHoverSprite.x = hoverSprite->x;
		mHoverSprite.y = hoverSprite->y;
		mHoverSprite.w = hoverSprite->w;
		mHoverSprite.h = hoverSprite->h;
	}
}

void LButton::setTexture(LTexture* texture) {
	mTexture = texture;
}

void LButton::handleEvent(SDL_Event* e) {
	// check if the event is a mouse event
	if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP) {
		// get mouse position
		int x, y;
		SDL_GetMouseState(&x, &y);

		// check if mouse is inside button
		if (x > mPosition.x && x < mPosition.x + mCurrentSprite.w &&
			y > mPosition.y && y < mPosition.y + mCurrentSprite.h) {
			// mouse is inside
			mHovered = true;

			if (e->type == SDL_MOUSEBUTTONDOWN) {
				mClicked = PRESSED;
			}
			else if (e->type == SDL_MOUSEBUTTONUP) {
				if (mClicked == PRESSED) {
					mClicked = RELEASED;
				}
			}
		}
		else mHovered = false;
	}
}

bool LButton::clicked() {
	return mClicked == RELEASED;
}

void LButton::render(SDL_Rect* scale) {
	if (mClicked == PRESSED) {
		mTexture->render(mPosition.x, mPosition.y, &mPressedSprite, scale);
	}
	else if (mHovered) {
		mTexture->render(mPosition.x, mPosition.y, &mHoverSprite, scale);
	}
	else {
		mTexture->render(mPosition.x, mPosition.y, &mCurrentSprite, scale);
	}
}

