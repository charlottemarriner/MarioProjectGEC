#include "CharacterMario.h"
#include <iostream>

CharacterMario::CharacterMario(SDL_Renderer* renderer, string imagePath, Vector2D startPosition, LevelMap* map) : Character( renderer, imagePath, startPosition, map)
{
	mPosition = startPosition;
	mCurrentFrame = 0;
}

CharacterMario::~CharacterMario()
{
}

void CharacterMario::Render()
{
	//Variable to hold the left position of the sprite we want to draw
	int left = 0.0f;

	//Get the portion of the spritesheet you want to draw.
	//								{XPos, YPos, WidthOfSingleSprite, HeightOfSingleSprite}
	SDL_Rect portionOfSpriteSheet = { mSingleSpriteWidth * mCurrentFrame, 0, mSingleSpriteWidth, mSingleSpriteHeight };

	//Determine where you want it drawn
	SDL_Rect destRect = { (int)(mPosition.x), (int)(mPosition.y), mSingleSpriteWidth, mSingleSpriteHeight };

	//Then draw it facing the correct direction
	if (mFacingDirection == FACING_LEFT)
	{
		mTexture->Render(portionOfSpriteSheet, destRect, SDL_FLIP_NONE);
	}
	else
	{
		mTexture->Render(portionOfSpriteSheet, destRect, SDL_FLIP_HORIZONTAL);
	}
}

void CharacterMario::Update(float deltaTime, SDL_Event e)
{
		Character::Update(deltaTime, e);

		//Setup Mario Spritesheet
		mSingleSpriteWidth = mTexture->GetWidth() / 6; //6 sprites on this spritesheet in 1 row
		mSingleSpriteHeight = mTexture->GetHeight();

		//Collision position variables
		int centralXPosition = (int)(mPosition.x + (mSingleSpriteWidth * 0.5f)) / TILE_WIDTH;
		int footPosition = (int)(mPosition.y + mSingleSpriteHeight) / TILE_HEIGHT;
		int headPosition = (int)(mPosition.y) / TILE_HEIGHT;

		//Deal with Gravity
		if (mCurrentLevelMap->GetTileAt(footPosition, centralXPosition) == 0)
		{
			AddGravity(deltaTime);
		}
		else
		{
			//Collided with ground so we can jump again
			mCanJump = true;
		}

		if (mCurrentLevelMap->GetTileAt(headPosition, centralXPosition) == 1)
		{
			mJumping = false;
		}



		mFrameDelay -= deltaTime;

		switch (e.type)
		{
		case SDL_KEYDOWN:
			switch (e.key.keysym.sym)
			{
			case SDLK_a:
				mMovingLeft = true;
				if (mFrameDelay <= 0.0f)
				{
					//Reset frame delay count
					mFrameDelay = ANIMATION_DELAY;

					//Move frame on
					mCurrentFrame++;

					//Loop frame around if it goes beyond the number of frames
					if (mCurrentFrame > 3)
					{
						mCurrentFrame = 1;
					}
				}
				break;
			case SDLK_d:
				mMovingRight = true;
				if (mFrameDelay <= 0.0f)
				{
					//Reset frame delay count
					mFrameDelay = ANIMATION_DELAY;

					//Move frame on
					mCurrentFrame++;

					//Loop frame around if it goes beyond the number of frames
					if (mCurrentFrame > 3)
					{
						mCurrentFrame = 1;
					}
				}
				break;
			case SDLK_w:
				mCurrentFrame = 4;
				Jump();
			}
			break;
		case SDL_KEYUP:
			switch (e.key.keysym.sym)
			{
			case SDLK_a:
				mMovingLeft = false;
				mCurrentFrame = 0;
				break;
			case SDLK_d:
				mMovingRight = false;
				mCurrentFrame = 0;
				break;
			}
		}
}

void CharacterMario::Death()
{
	mPosition.x = 64;
	mPosition.y = 330;
	gSoundEffect->DeathSound();
	//Reset frame delay count
	mFrameDelay = ANIMATION_DELAY;
	mCurrentFrame = 5;
}

void CharacterMario::MoveLeft(float deltaTime)
{
	mPosition.x -= MOVEMENTSPEED;
	mFacingDirection = FACING_LEFT;
}

void CharacterMario::MoveRight(float deltaTime)
{
	mPosition.x += MOVEMENTSPEED;
	mFacingDirection = FACING_RIGHT;
}
