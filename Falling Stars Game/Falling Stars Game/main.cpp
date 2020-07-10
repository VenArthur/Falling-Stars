#include <cmath>
#include <ctime>
#include <iostream>
#include <random>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "Texture.h"
#include "Player.h"
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
Texture g_PlayerTexture; //This is the current player texture
Texture g_PlayerStandingLeft;
Texture g_PlayerStandingRight;
Texture g_PlayerRunningLeft;
Texture g_PlayerRunningRight;
Texture g_ScoreTextTexture;

//Font
TTF_Font* g_Font = NULL;

//Sound Effects
Mix_Chunk* g_StarSoundEffect = NULL;

//Stars Random Starting Position for the X-coordinate
std::random_device dev;
std::mt19937 rng(dev());
std::uniform_int_distribution<std::mt19937::result_type> randomPosX(50, SCREEN_WIDTH - 100);

//Random Star Speed
std::uniform_int_distribution<std::mt19937::result_type> randomSpeed(20, 100);


//Meteor Starting Position for the X-coordinate
//A meteor can start at only 1 of 2 possible positions. The top left corner or the top right corner
int GetMeteorStartX(int const a = 0, int const b = SCREEN_WIDTH)
{
	std::uniform_int_distribution<std::mt19937::result_type> meteorPoint(1, 2);
	return (meteorPoint(rng) == 1) ? a : b;
}


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

	//Create the player
	Player player;

	//Text in memory for player score
	std::stringstream playerScoreText;

	//Color for player score
	SDL_Color textColor{ 255, 255, 255, 255 };

	//Create stars
	std::vector<Stars*> stars;
	for (int i = 0; i < 10; i++)
	{
		stars.push_back(new Stars(randomPosX(rng), -50, 1.0 / randomSpeed(rng)));
	}

	//Max amount of stars that are currently falling 
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
			//Starting texture
			g_PlayerTexture = g_PlayerStandingLeft;

			//Main loop
			while (!quit)
			{
				while (SDL_PollEvent(&evnt) != 0)
				{
					if (evnt.type == SDL_QUIT)
					{
						quit = true;
					}

					player.handleEvent(evnt, g_PlayerTexture, g_PlayerStandingLeft, g_PlayerStandingRight, g_PlayerRunningLeft, g_PlayerRunningRight);
				}
			
				//Clear Screen
				SDL_SetRenderDrawColor(g_Renderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(g_Renderer);

				//Render background
				g_BackgroundTexture.render(g_Renderer, 0, 0);

				//Rendering the stars that are moving
				for (int s = 0; s < starsFalling; s++)
				{
					stars[s]->move(player.getStarCollider(), g_StarSoundEffect, player.score);
					stars[s]->render(g_Renderer, g_StarsTexture);
				}

				//Load score texture
				playerScoreText.str("");
				playerScoreText << player.score;

				if (!g_ScoreTextTexture.loadFromRenderedText(g_Renderer, textColor, g_Font, playerScoreText.str().c_str()))
				{
					printf("\nUnable to load score text texture!\n");
				}

				//Render score texture
				g_ScoreTextTexture.render(g_Renderer, 25, 20);
				

				//Render the player
				player.render(g_Renderer, g_PlayerTexture);

				//Update screen
				SDL_RenderPresent(g_Renderer);


				counter += 0.015;

				//A new star is ready to appear, set counter back to 0. Also add another star to the vector
				if (counter >= 1)
				{
					starsFalling += 1;
					counter = 0;

					stars.push_back(new Stars(randomPosX(rng), -50, 1.0 / randomSpeed(rng)));

					//The size of the vector will never be greater than 20 to avoid an overflow
					if (stars.size() % 21 == 0)
					{
						//Erase the stars no longer on the screen
						stars.erase(stars.begin() + 0, stars.begin() + 6);
						starsFalling = 6;
					}

					//std::cout << starsFalling << std::endl;
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

				//Initialize SDL_mixer
				if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
				{
					printf("\nSDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
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
		printf("\nFailed to load background texture image! \n");
		loadMediaSuccess = false;
	}

	//Load star texture
	if (!g_StarsTexture.loadFromFile(g_Renderer, "Stars.png"))
	{
		printf("\nFailed to load star texture image! \n");
		loadMediaSuccess = false;
	}

	//Load player standing left texture
	if (!g_PlayerStandingLeft.loadFromFile(g_Renderer, "PlayerStandingLeft.png"))
	{
		printf("\nFailed to load PlayerStandingLeft texture image! \n");
		loadMediaSuccess = false;
	}

	//Load player standing right texture
	if (!g_PlayerStandingRight.loadFromFile(g_Renderer, "PlayerStandingRight.png"))
	{
		printf("\nFailed to load PlayerStandingRight texture image! \n");
		loadMediaSuccess = false;
	}

	//Load player running left 1 texture
	if (!g_PlayerRunningLeft.loadFromFile(g_Renderer, "PlayerRunningLeft1.png"))
	{
		printf("\nFailed to load PlayerRunningLeft texture image! \n");
		loadMediaSuccess = false;
	}

	//Load player running right 1 texture
	if (!g_PlayerRunningRight.loadFromFile(g_Renderer, "PlayerRunningRight1.png"))
	{
		printf("\nFailed to load PlayerRunningRight texture image! \n");
		loadMediaSuccess = false;
	}

	//Open the font
	g_Font = TTF_OpenFont("Orbitron-SemiBold.ttf", 22);
	if (g_Font == NULL)
	{
		printf("\nFailed to load the score font (g_Font)! SDL_ttf Error: %s\n", TTF_GetError());
		loadMediaSuccess = false;
	}

	//Load star sound effect
	g_StarSoundEffect = Mix_LoadWAV("StarSoundEffect.wav");
	if (g_StarSoundEffect == NULL)
	{
		printf("\nFailed to load star sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		loadMediaSuccess = false;
	}

	return loadMediaSuccess;
}


void close()
{
	//Free textures
	g_BackgroundTexture.free();
	g_StarsTexture.free();
	g_PlayerTexture.free();
	g_PlayerStandingLeft.free();
	g_PlayerStandingRight.free();
	g_PlayerRunningLeft.free();
	g_PlayerRunningRight.free();
	g_ScoreTextTexture.free();

	//Close the font
	TTF_CloseFont(g_Font);
	g_Font = NULL;

	//Free Sound Effects
	Mix_FreeChunk(g_StarSoundEffect);

	//Destroy window
	SDL_DestroyRenderer(g_Renderer);
	SDL_DestroyWindow(g_Window);
	g_Window = NULL;
	g_Renderer = NULL;

	//Quit SDL
	IMG_Quit();
	SDL_Quit();
}