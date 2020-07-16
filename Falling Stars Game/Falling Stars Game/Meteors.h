#pragma once
#include <cmath>
#include <random>
#include <SDL.h>
#include <SDL_mixer.h>
#include <time.h>
#include "Texture.h"
#include "Stars.h"

class Meteors
{
public:
	//Meteor Dimensions
	static const int METEOR_WIDTH = 144;
	static const int METEOR_HEIGHT = 144;

	//Initializes the variables
	Meteors(float x, float y, float speed);

	//Moves the meteors
	void move(SDL_Rect& playerCollider, Mix_Chunk* soundEffect);

	//Renders stars to the screen
	void render(SDL_Renderer* renderer, Texture& meteorTexture);

	//Gets collision circle
	Circle& getCollider();

	//Position getters
	int getPosX() { return floor(m_PosX); }
	int getPosY() { return floor(m_PosY); }

	//Moves the collision circle relative to the meteor's offset
	void shiftColliders();

	//Circle-Box collision detector
	bool checkCollision(Circle& a, SDL_Rect& b);

	friend double Stars::distanceSquared(int x1, int y1, int x2, int y2);

private:
	//X and Y offsets
	float m_PosX, m_PosY;

	//Meteor speed
	float m_MeteorSpeed;

	//The velocity of the meteors
	float m_VelX;
	float m_VelY;

	//Do NOT render flag for meteors that hit the player
	bool m_DoNOTRender;

	//The meteor's collision circle
	Circle m_Collider;
};

