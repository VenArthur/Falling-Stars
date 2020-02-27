#include "Texture.h"



Texture::Texture()
{
	//Initialize
	m_Texture = NULL;
	m_Width = 0;
	m_Height = 0;
}


Texture::~Texture()
{
	free();
}


bool Texture::loadFromFile(SDL_Renderer* renderer, std::string path)
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());

	if (loadedSurface == NULL)
	{
		printf("\nUnable to load image %s ! SDL_image Error: %s\n ", path.c_str(), IMG_GetError());
	}
	else
	{
		//Color key image
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);

		if (newTexture == NULL)
		{
			printf("\nUnable to create texture from %s ! SDL_Error: %s\n", path.c_str(), SDL_GetError());
		}
		else
		{
			//Get image dimensions
			m_Width = loadedSurface->w;
			m_Height = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	//Return success
	m_Texture = newTexture;
	return m_Texture != NULL;
}


bool Texture::loadFromRenderedText(SDL_Renderer* renderer, SDL_Color textColor, TTF_Font* font, std::string textureText)
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, textureText.c_str(), textColor);

	if (textSurface == NULL)
	{
		printf("\nUnable to render text surface! SDL_ttf Error %s\n", TTF_GetError());
	}
	else
	{
		//Create texture from surface pixels
		m_Texture = SDL_CreateTextureFromSurface(renderer, textSurface);

		if (m_Texture == NULL)
		{
			printf("\nUnable to create texture from rendered text! SDL_Error: %s\n", SDL_GetError());
		}
		else
		{
			//Get image dimensions
			m_Width = textSurface->w;
			m_Height = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}

	//Return success
	return m_Texture != NULL;
}


void Texture::free()
{
	//Free texture if it exists
	if (m_Texture != NULL)
	{
		SDL_DestroyTexture(m_Texture);
		m_Texture = NULL;
		m_Width = 0;
		m_Height = 0;
	}
}


void Texture::render(SDL_Renderer* renderer, int x, int y)
{
	//Set rendering space 
	SDL_Rect renderQuad = { x, y, m_Width, m_Height };

	//Render to screen
	SDL_RenderCopy(renderer, m_Texture, NULL, &renderQuad);
}

int Texture::getWidth()
{
	return m_Width;
}

int Texture::getHeight()
{
	return m_Height;
}
