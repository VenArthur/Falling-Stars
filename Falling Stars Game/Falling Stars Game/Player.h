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
	//Render the player
	void render(SDL_Renderer* renderer, Texture& playerTexture);
	//Handles Player textures and movement
	void handleEvent(SDL_Event evnt, Texture& playerTexture, Texture &standingLeft, Texture &standingRight, Texture &runningLeft1, Texture &runningRight);

	//Position getters
	int getPosX() { return floor(m_PosX); }
	int getPosY() { return floor(m_PosY); }

	//Gets collision circle
	Circle& getStarCollider();

	//Gets collision rectangle
	SDL_Rect& getMeteorCollider();

	//Moves the collision circle relative to the player's offset
	void shiftColliders();

	int score;

private:
	//X offset
	float m_PosX;
	//Y offset - Y is const because the player only moves left and right
	const float m_PosY = 460;

	//Player speed
	float m_PlayerSpeed = 15;

	//The player's collision circle
	Circle m_StarCollider;

	//The player's collision rectangle
	SDL_Rect m_MeteorCollider;

	//For collision, whether or not the player is facing left or right
	bool m_IsGoingLeft;
	
};

