#include "LTexture.h"
#include<iostream>

LTexture::LTexture()
{
	//Initialize
	mRenderer = NULL;
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
	std::cout << "DEFAULT CONSTRUCTOR USED";
}

LTexture::LTexture(SDL_Renderer* renderer)
{
	//Initialize
	mRenderer = renderer;
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

bool LTexture::loadFromFile(std::string path)
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Color key image
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(mRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

bool LTexture::loadFromRenderedText(std::string textureText, SDL_Color textColor, TTF_Font* font){
    //Get rid of preexisting texture
    free();

    //Render text surface
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, textureText.c_str(), textColor);
    if( textSurface == NULL ){
        printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
    }
    else{
        //Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface(mRenderer, textSurface);
        if(mTexture == NULL){
            printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
        }
        else{
            //Get image dimensions
            mWidth = textSurface->w;
            mHeight = textSurface->h;
        }

        //Get rid of old surface
        SDL_FreeSurface( textSurface );
    }
    
    //Return success
    return mTexture != NULL;
}

void LTexture::free()
{
	//Free texture if it exists
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::render(int x, int y, SDL_Rect* clip, SDL_Rect* scale)
{
	if (mRenderer == NULL) {
		std::cout << "cannot render with a NULL renderer! don't use the default constructor!! (LTexture.render)";
	}
	else {
		//Set rendering space and render to screen
		SDL_Rect renderQuad = { x, y, mWidth, mHeight };

		if (scale != NULL) {
			renderQuad.w = scale->w;
			renderQuad.h = scale->h;
		}
		else if (clip != NULL) {
			renderQuad.w = clip->w;
			renderQuad.h = clip->h;
		}
		
		SDL_RenderCopy(mRenderer, mTexture, clip, &renderQuad);
		//std::cout << "rendering at x=" << renderQuad.x;
		//std::cout << " ...at y=" << renderQuad.y;
		//std::cout << " ...at w=" << renderQuad.w;
		//std::cout << " ...at h=" << renderQuad.h;
	}
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}