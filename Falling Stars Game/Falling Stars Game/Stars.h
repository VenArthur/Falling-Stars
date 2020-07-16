#pragma once
#include <cmath>
#include <random>
#include <SDL.h>
#include <SDL_mixer.h>
#include <time.h>
#include "Texture.h"

struct Circle
{
	int x, y;
	int r;
};

class Stars
{
public:
	//Star Dimensions
	static const int STAR_WIDTH = 50;
	static const int STAR_HEIGHT = 50;

	//Initializes the variables
	Stars(float x, float y, float speed);

	//Moves the stars
	void move(Circle& playerCollider, Mix_Chunk *soundEffect, int &playerScore);

	//Renders stars to the screen
	void render(SDL_Renderer *renderer, Texture &starsTexture);

	//Gets collision circle
	Circle &getCollider();

	//Position getters
	int getPosX() { return floor(m_PosX); }
	int getPosY() { return floor(m_PosY); }

	//Moves the collision circle relative to the star's offset
	void shiftColliders();

	//Circle-Circle collision detector
	bool checkCollision(Circle& a, Circle& b);

	//Calculates distanced squared between 2 points
	double distanceSquared(int x1, int y1, int x2, int y2);

private:
	//X and Y offsets
	float m_PosX, m_PosY;

	//Star speed
	float m_StarSpeed;

	//The velocity of the stars (only need Y velocity since the stars only move down)
	float m_VelY;

	//Do NOT render flag for the caught stars
	bool m_DoNOTRender;

	bool scored = false;

	//The star's collision circle
	Circle m_Collider;
};

