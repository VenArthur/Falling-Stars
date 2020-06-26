#include "Stars.h"

Stars::Stars(float x, float y, float speed)
{
	//Initialize the offsets
	m_PosX = x;
	m_PosY = y;

	//Initialize Speed
	m_StarSpeed = speed;

	m_Collider.r = STAR_WIDTH / 2;

	//Initialize the velocity
	m_VelY = 0;

	//Do NOT Render flag for collision is set to false
	m_DoNOTRender = false;

	//Move colliders relative to the circle
	shiftColliders();
}

void Stars::move(Circle &playerCollider) //Circle& collisionCircle as a parameter when implementing catching the stars
{
	m_VelY += m_StarSpeed; //This is first because otherwise the stars will instantly move - will use properly for a starting button of the game
	m_PosY += m_VelY;

	shiftColliders();

	if (checkCollision(m_Collider, playerCollider))
	{
		//If collided do not render flag goes here
		m_DoNOTRender = true;
	}
}

void Stars::render(SDL_Renderer* renderer, Texture& starsTexture)
{
	//Show the texture
	if (!m_DoNOTRender)
	{
		starsTexture.render(renderer, m_PosX - m_Collider.r, m_PosY - m_Collider.r);
	}
}

Circle& Stars::getCollider()
{
	return m_Collider;
}

void Stars::shiftColliders()
{
	m_Collider.x = m_PosX;
	m_Collider.y = m_PosY;
}


bool Stars::checkCollision(Circle& a, Circle& b)
{
	//Calculate total radius squared
	int totalRadiusSquared = a.r + b.r;
	totalRadiusSquared = totalRadiusSquared * totalRadiusSquared;

	//If the distance between the centers of the circles is less than the sum of their radii
	if (distanceSquared(a.x, a.y, b.x, b.y) < (totalRadiusSquared))
	{
		//The circles have collided
		return true;
	}

	//If not
	return false;
}

double Stars::distanceSquared(int x1, int y1, int x2, int y2)
{
	int deltaX = x2 - x1;
	int deltaY = y2 - y1;

	return deltaX * deltaX + deltaY * deltaY;
}
