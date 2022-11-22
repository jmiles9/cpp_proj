#ifndef LBUTTON_H_INCLUDED
#define LBUTTON_H_INCLUDED

#include<SDL.h>
#include "LTexture.h"

class LButton
{
    public:
        //Initializes internal variables
        LButton();

        //Sets top left position
        void setPosition(int x, int y);

        //Sets the current sprite 
        void setSprite(SDL_Rect* sprite, SDL_Rect* sprite2 = NULL, SDL_Rect* hoverSprite = NULL);

        //Sets texture to take sprite from
        void setTexture(LTexture* texture);

        //Handles mouse event
        void handleEvent(SDL_Event* e);

        //Check if button has been clicked
        bool clicked();

        //Shows button sprite (button clicking zone does NOT scale with 'scale'!!!!! lol)
        void render(SDL_Rect* scale = NULL);

    private:
        enum mEButtonState {UNPRESSED, PRESSED, RELEASED};

        //Top left position
        SDL_Point mPosition;

        //if it has been clicked
        int mClicked;
        bool mHovered;

        //sprites (one for pressed and one for not pressed and one for hovered)
        SDL_Rect mCurrentSprite;
        SDL_Rect mPressedSprite;
        SDL_Rect mHoverSprite;


        LTexture* mTexture;
};

#endif