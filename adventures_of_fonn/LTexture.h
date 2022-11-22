#ifndef LTEXTURE_H_INCLUDED
#define LTEXTURE_H_INCLUDED

#include<SDL.h>
#include<string>
#include<SDL_image.h>
#include<SDL_ttf.h>

class LTexture
{
	public:
		//Initializes variables
		LTexture(); //this one should not be used 
		LTexture(SDL_Renderer* renderer);

		//Deallocates memory
		~LTexture();

		//Loads image at specified path
		bool loadFromFile(std::string path);

		//Creates image from font string
        bool loadFromRenderedText(std::string textureText, SDL_Color textColor, TTF_Font* font);

		//Deallocates texture
		void free();

		//Renders texture at given point
		void render(int x, int y, SDL_Rect* clip = NULL, SDL_Rect* scale = NULL);

		//Gets image dimensions
		int getWidth();
		int getHeight();

	private:
		//The actual hardware texture
		SDL_Texture* mTexture;
		
		//the renderer to use
		SDL_Renderer* mRenderer;

		//Image dimensions
		int mWidth;
		int mHeight;
};

#endif
