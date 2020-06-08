#include "Player.h"

Player::Player()
{
	m_PosX = 640;

	m_Collider.r = PLAYER_WIDTH / 2;

	shiftColliders();
}

void Player::render(SDL_Renderer* renderer, Texture& playerTexture)
{
	//Show the texture
	playerTexture.render(renderer, m_PosX - m_Collider.r, m_PosY - m_Collider.r);
}

void Player::handleEvent(SDL_Event evnt)
{

}

Circle& Player::getCollider()
{
	return m_Collider;
}

void Player::shiftColliders()
{
	m_Collider.x = m_PosX;
	//m_Collider.y = m_PosY;
}


bool Player::checkCollision(Circle& a, Circle& b)
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

double Player::distanceSquared(int x1, int y1, int x2, int y2)
{
	int deltaX = x2 - x1;
	int deltaY = y2 - y1;

	return deltaX * deltaX + deltaY * deltaY;
}