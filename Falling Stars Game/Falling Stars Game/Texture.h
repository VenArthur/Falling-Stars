#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>
class Texture
{
public:
	//Initialize variables
	Texture();

	//Deallocates memory
	~Texture();

	//Loads image at specified path
	bool loadFromFile(SDL_Renderer* renderer, std::string path);

	//Creates image from font string
	bool loadFromRenderedText(SDL_Renderer* renderer, SDL_Color textColor, TTF_Font* font, std::string textureText);

	//Deallocates texture
	void free();

	//Renders the texture at a given point
	void render(SDL_Renderer* renderer, int x, int y);

	//Get image dimensions
	int getWidth();
	int getHeight();

	friend bool operator== (const Texture &tex1, const Texture &tex2);

private:
	//The texture
	SDL_Texture* m_Texture;

	//Image dimensions
	int m_Width;
	int m_Height;
};