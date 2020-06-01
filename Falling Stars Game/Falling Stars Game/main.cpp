#include <cmath>
#include <ctime>
#include <iostream>
#include <random>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Texture.h"
#include "Stars.h"

//Window dimensions 
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

//Window to be rendered to
SDL_Window* g_Window = NULL;

//The surface contained by the window
SDL_Surface* g_ScreenSurface = NULL;

//Window Renderer
SDL_Renderer* g_Renderer = NULL;

//Textures
Texture g_BackgroundTexture;
Texture g_StarsTexture;

//Stars
//Random Starting Position for the X-coordinate
std::random_device dev;
std::mt19937 rng(dev());
std::uniform_int_distribution<std::mt19937::result_type> randomPosX(50, SCREEN_WIDTH - 100);

//Random Star Speed
std::uniform_int_distribution<std::mt19937::result_type> randomSpeed(20, 100);

//Stars star(randomPosX(rng), -50, starSpeed);

//Initialize SDL and create the window
bool init();

//Image to load 
SDL_Surface* loadSurface(std::string path);

//Load media
bool loadMedia();

//Destroy window and close SDL
void close();


int main(int argc, char* args[])
{
	//Main loop flag
	bool quit = false;

	//Event handler
	SDL_Event evnt;

	//Create stars
	std::vector<Stars*> stars;
	for (int i = 0; i < 10; i++)
	{
		stars.push_back(new Stars(randomPosX(rng), -50, 1.0 / randomSpeed(rng)));
	}

	//Stars that are currently on screen
	int starsFalling = 1;
	//When to add the next star
	float counter = 0;

	//Initialize
	if (!init())
	{
		printf("\nFailed to initialize!\n");
	}
	else
	{
		//Load media
		if (!loadMedia())
		{
			printf("\nFailed to load media!\n");
		}
		else
		{

			//Main loop
			while (!quit)
			{
				while (SDL_PollEvent(&evnt) != 0)
				{
					if (evnt.type == SDL_QUIT)
					{
						quit = true;
					}
				}

				//The max amount of stars falling is the amount in the vector
				if (starsFalling > stars.size())
				{
					starsFalling = stars.size();
				}

				//star.move();
				for (int m = 0; m < starsFalling; m++)
				{
					stars[m]->move();
				}

				//Clear Screen
				SDL_SetRenderDrawColor(g_Renderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(g_Renderer);

				//Render background
				g_BackgroundTexture.render(g_Renderer, 0, 0);

				//star.render(g_Renderer, g_StarsTexture);
				for (int r = 0; r < starsFalling; r++)
				{
					stars[r]->render(g_Renderer, g_StarsTexture);
				}

				//Update screen
				SDL_RenderPresent(g_Renderer);


				counter += 0.005;

				//A new star is ready to appear, set counter back to 0. Also add another star to the vector
				if (counter >= 1)
				{
					starsFalling += 1;
					counter = 0;

					stars.push_back(new Stars(randomPosX(rng), -50, 1.0 / randomSpeed(rng)));
				}
			}
		}
	}

	//Free resources close SDL
	close();

	return 0;
}


bool init()
{
	//Initialization flag
	bool initSuccess = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		//SDL cannot be initialized. Print the error and success is a failure
		printf("\nSDL could not be initialized! SDL_Error: %s\n", SDL_GetError());
		initSuccess = false;
	}
	else
	{
		//Create the window
		g_Window = SDL_CreateWindow("Learning SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

		//If the window couldn't be created
		if (g_Window == NULL)
		{
			printf("\nThe window could not be created! SDL_Error: %s\n", SDL_GetError());
			initSuccess = false;
		}
		else
		{
			//Create Vsynced Renderer for Window
			g_Renderer = SDL_CreateRenderer(g_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

			if (g_Renderer == NULL)
			{
				printf("\nRenderer could not be created! SDL_Error: %s\n", SDL_GetError());
			}
			else
			{
				//Initialize Renderer color
				SDL_SetRenderDrawColor(g_Renderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG Loading
				int imgFlags = IMG_INIT_PNG;

				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("\nSDL_image could not initialize! SDL_image_error: %s\n", IMG_GetError());
					initSuccess = false;
				}

				//Initialize SDL_ttf
				if (TTF_Init() == -1)
				{
					printf("\nSDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
					initSuccess = false;
				}
			}
		}
	}

	return initSuccess;
}


SDL_Surface* loadSurface(std::string path)
{
	//The final optimized image
	SDL_Surface* optimizedSurface = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());

	if (loadedSurface == NULL)
	{
		printf("\nUnable to load image %s! SDL_Error: %s\n: ", path.c_str(), IMG_GetError());
	}
	else
	{
		//Convert image to screen format
		optimizedSurface = SDL_ConvertSurface(loadedSurface, g_ScreenSurface->format, NULL);

		if (optimizedSurface == NULL)
		{
			printf("\nUnable to optimize image %s! SDL_GetError: %s\n", path.c_str(), SDL_GetError());
		}

		//Get rid of old surface
		SDL_FreeSurface(loadedSurface);
	}

	return optimizedSurface;
}


bool loadMedia()
{
	//Loading success flag
	bool loadMediaSuccess = true;

	//Load background texture
	if (!g_BackgroundTexture.loadFromFile(g_Renderer, "Space Background.png"))
	{
		printf("\nFailed to load pride texture image! \n");
		loadMediaSuccess = false;
	}

	//Load background texture
	if (!g_StarsTexture.loadFromFile(g_Renderer, "Stars.png"))
	{
		printf("\nFailed to load pride texture image! \n");
		loadMediaSuccess = false;
	}

	return loadMediaSuccess;
}


void close()
{
	//Free textures
	g_BackgroundTexture.free();
	g_StarsTexture.free();

	//Destroy window
	SDL_DestroyRenderer(g_Renderer);
	SDL_DestroyWindow(g_Window);
	g_Window = NULL;
	g_Renderer = NULL;

	//Quit SDL
	IMG_Quit();
	SDL_Quit();
}