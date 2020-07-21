#include "Meteors.h"

Meteors::Meteors(float x, float y, float speed)
{
	//Initialize the offsets
	m_PosX = x;
	m_PosY = y;

	if (m_PosX > 0)
	{
		m_InRightCorner = true;
	}

	//Initialize Speed
	m_MeteorSpeed = speed;

	m_Collider.r = METEOR_WIDTH / 2;

	//Initialize the velocity
	m_VelX = 0;
	m_VelY = 0;

	//Do NOT Render flag for collision is set to false
	m_DoNOTRender = false;

	//Move colliders relative to the circle
	shiftColliders();
}

void Meteors::move(SDL_Rect& playerCollider, Mix_Chunk* soundEffect, int& playerHearts)
{
	m_VelX += m_MeteorSpeed;
	m_VelY += m_MeteorSpeed; //This is first because otherwise the meteors will instantly move - will use properly for a starting button of the game

	if (m_InRightCorner == true)
	{
		m_PosX -= m_VelX;
		m_PosY += m_VelY;
	}
	else
	{
		m_PosX += m_VelX;
		m_PosY += m_VelY;
	}

	shiftColliders();

	if (checkCollision(m_Collider, playerCollider))
	{
		//If collided do not render flag goes here
		m_DoNOTRender = true;

		//If the meteor hasn't registered that the player got hit yet, but there is collision, 
		//the number of hearts goes down by one, a sound plays, and the meteor registers that the player got hit
		if (m_HeartDecrease == false)
		{
			playerHearts -= 1;
			Mix_PlayChannel(-1, soundEffect, 0);
			m_HeartDecrease = true;
		}
	}
}

void Meteors::render(SDL_Renderer* renderer, Texture& meteorTexture)
{
	//Show the texture
	if (!m_DoNOTRender)
	{
		meteorTexture.render(renderer, m_PosX - m_Collider.r, m_PosY - m_Collider.r);
	}
}

void Meteors::shiftColliders()
{
	m_Collider.x = m_PosX;
	m_Collider.y = m_PosY;
}

bool Meteors::checkCollision(Circle& a, SDL_Rect& b)
{
	//Closest point on collision box
	int cX, cY;

	//Find the closest x offset
	if (a.x < b.x)
	{
		cX = b.x;
	}
	else if (a.x > b.x + b.w)
	{
		cX = b.x + b.w;
	}
	else
	{
		cX = a.x;
	}

	//Find the closest y offset
	if (a.y < b.y)
	{
		cY = b.y;
	}
	else if (a.y > b.y + b.h)
	{
		cY = b.y + b.h;
	}
	else
	{
		cY = a.y;
	}

	//If the closest point is inside the circle
	if (distanceSquared(a.x, a.y, cX, cY) < a.r * a.r)
	{
		//This box and circle have collided
		return true;
	}

	//If the shapes have not collided
	return false;
}

double Meteors::distanceSquared(int x1, int y1, int x2, int y2)
{
	int deltaX = x2 - x1;
	int deltaY = y2 - y1;

	return deltaX * deltaX + deltaY * deltaY;
}