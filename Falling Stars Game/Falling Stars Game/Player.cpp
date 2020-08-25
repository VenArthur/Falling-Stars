#include "Player.h"

Player::Player()
{
	//Starting X Position
	m_PosX = 640;

	//Initialize Star Collider
	m_StarCollider.x = m_PosX;
	m_StarCollider.y = m_PosY + (PLAYER_HEIGHT / 2);
	m_StarCollider.r = 20;

	//Initialize Meteor Collider
	m_MeteorCollider.x = m_PosX + 80;
	m_MeteorCollider.y = m_PosY + 10;
	m_MeteorCollider.w = 50;
	m_MeteorCollider.h = PLAYER_HEIGHT - 70;

	//Initialize score
	score = 0;

	//Initialize hearts
	hearts = 3;

	//Initialize name
	name = "";

	shiftColliders();
}

void Player::render(SDL_Renderer* renderer, Texture& playerTexture)
{
	//Show the texture
	playerTexture.render(renderer, m_PosX - m_StarCollider.r, m_PosY - m_StarCollider.r);

	/*Testing dimensions for meteor collider
	SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
	SDL_RenderDrawRect(renderer, &m_MeteorCollider);
	*/
	
}

//Handles Player textures and movement
void Player::handleEvent(SDL_Event evnt, Texture& playerTexture, Texture &standingLeft, Texture &standingRight, Texture &runningLeft, Texture &runningRight)
{
	if (evnt.type == SDL_KEYDOWN)
	{
		if (evnt.key.keysym.sym == SDLK_a || evnt.key.keysym.sym == SDLK_LEFT)
		{ 
			m_IsGoingLeft = true;

			if (playerTexture == runningLeft)
			{
				playerTexture = standingLeft;
			}
			else
			{
				playerTexture = runningLeft;
			}

			m_PosX -= m_PlayerSpeed;

			//Don't go off screen on the left side
			if (m_PosX < 0)
			{
				m_PosX += m_PlayerSpeed;
			}

			shiftColliders();
		}
		else if (evnt.key.keysym.sym == SDLK_d || evnt.key.keysym.sym == SDLK_RIGHT)
		{
			m_IsGoingLeft = false;

			if (playerTexture == runningRight)
			{
				playerTexture = standingRight;
			}
			else
			{
				playerTexture = runningRight;
			}


			m_PosX += m_PlayerSpeed;

			//Don't go off screen on the right side 
			//(1280 is the screenwidth, yes I could have passed it in but I'm lazy), the minus 5 is because it was off by 5 to the end of the screen
			if (m_PosX > (1280 + m_StarCollider.r) - (PLAYER_WIDTH - 5))
			{
				m_PosX -= m_PlayerSpeed;
			}

			shiftColliders();
		}
	}
	else if (evnt.type == SDL_KEYUP)
	{
		if (playerTexture == runningLeft)
		{
			playerTexture = standingLeft;
		}
		else if (playerTexture == runningRight)
		{
			playerTexture = standingRight;
		}
	}
}

Circle& Player::getStarCollider()
{
	return m_StarCollider;
}

SDL_Rect& Player::getMeteorCollider()
{
	return m_MeteorCollider;
}

void Player::shiftColliders()
{
	if (m_IsGoingLeft)
	{
		m_StarCollider.x = m_PosX;
		m_StarCollider.y = m_PosY + (PLAYER_HEIGHT / 2);

		m_MeteorCollider.x = m_PosX + 80;
		m_MeteorCollider.y = m_PosY + 10;
		
	}
	else
	{
		m_StarCollider.x = m_PosX + (PLAYER_WIDTH - 50);
		m_StarCollider.y = m_PosY + (PLAYER_HEIGHT / 2);

		m_MeteorCollider.x = m_PosX + 30;
		m_MeteorCollider.y = m_PosY + 10;
	}
}


