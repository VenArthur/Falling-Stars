#pragma once
#include <SDL.h>
#include "Texture.h"
#include "Stars.h"

class Player
{
public:
	//Player dimensions
	static const int PLAYER_WIDTH = 200;
	static const int PLAYER_HEIGHT = 200;

	Player();
	void render(SDL_Renderer* renderer, Texture& playerTexture);
	void handleEvent(SDL_Event evnt, Texture& playerTexture, Texture &standingLeft, Texture &standingRight, Texture &runningLeft1, Texture &runningRight);

	//Position getters
	int getPosX() { return floor(m_PosX); }
	int getPosY() { return floor(m_PosY); }

	//Gets collision circle
	Circle& getCollider();

	//Moves the collision circle relative to the star's offset
	void shiftColliders();

private:
	//X offset
	float m_PosX;
	//Y offset - Y is const because the player only moves left and right
	const float m_PosY = 600;

	//Player speed
	float m_PlayerSpeed = 15;

	//The player's collision circle
	Circle m_Collider;

	//Circle-Circle collision detector
	bool checkCollision(Circle& a, Circle& b);

	//Calculates distanced squared between 2 points
	double distanceSquared(int x1, int y1, int x2, int y2);
};

