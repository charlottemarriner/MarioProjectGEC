#include "Coin.h"

Coin::Coin(SDL_Renderer* renderer, string imagePath, LevelMap* map, Vector2D startPosition, FACING facing, float paramSpeed) : Character(renderer, imagePath, startPosition, map)
{
	mPosition = startPosition;
	mCurrentFrame = 0;
}

Coin::~Coin()
{
}

void Coin::Update(float deltaTime, SDL_Event e)
{
	//To gain the benefit of the code written in the base class Update() function we need to call it
	Character::Update(deltaTime, e);

	//Setup coin spritesheet
	mSingleSpriteWidthCoin = mTexture->GetWidth() / 3; //3 sprites on this spritesheet in 1 row
	mSingleSpriteHeightCoin = mTexture->GetHeight();

	//Collision position variables
	int centralXPosition = (int)(mPosition.x + (mSingleSpriteWidthCoin * 0.5f)) / TILE_WIDTH;
	int footPosition = (int)(mPosition.y + mSingleSpriteHeightCoin) / TILE_HEIGHT;
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

	//Change frame? 
	mFrameDelay -= deltaTime;
	if (mFrameDelay <= 0.0f)
	{
		//Reset frame delay count
		mFrameDelay = ANIMATION_DELAY;

		//Move frame on
		mCurrentFrame++;

		//Loop frame around if it goes beyond the number of frames
		if (mCurrentFrame > 2)
		{
			mCurrentFrame = 0;
		}
	}

	//move
	if (mFacingDirection == FACING_LEFT)
	{
		mMovingLeft = true;
		mMovingRight = false;

	}
	else if (mFacingDirection == FACING_RIGHT)
	{
		mMovingRight = true;
		mMovingLeft = false;

	}
}

void Coin::MoveLeft(float deltaTime)
{
	mPosition.x -= COIN_SPEED;
	mFacingDirection = FACING_LEFT;
}

void Coin::MoveRight(float deltaTime)
{
	mPosition.x += COIN_SPEED;
	mFacingDirection = FACING_RIGHT;
}

void Coin::Render()
{
	//Variable to hold the left position of the sprite we want to draw
	int left = 0.0f;

	//Get the portion of the spritesheet you want to draw.
	//								{XPos, YPos, WidthOfSingleSprite, HeightOfSingleSprite}
	SDL_Rect portionOfSpriteSheet = { mSingleSpriteWidthCoin*mCurrentFrame, 0, mSingleSpriteWidthCoin, mSingleSpriteHeightCoin };

	//Determine where you want it drawn
	SDL_Rect destRect = { (int)(mPosition.x), (int)(mPosition.y), mSingleSpriteWidthCoin, mSingleSpriteHeightCoin };

	//Then draw it facing the correct direction
	if (mFacingDirection == FACING_RIGHT)
	{
		mTexture->Render(portionOfSpriteSheet, destRect, SDL_FLIP_NONE);
	}
	else
	{
		mTexture->Render(portionOfSpriteSheet, destRect, SDL_FLIP_HORIZONTAL);
	}
}

void Coin::SwapFacing()
{
	if (mFacingDirection == FACING_LEFT)
	{
		mMovingLeft = false;
		mMovingRight = true;
		mFacingDirection = FACING_RIGHT;
	}
	else if (mFacingDirection == FACING_RIGHT)
	{
		mMovingRight = false;
		mMovingLeft = true;
		mFacingDirection = FACING_LEFT;
	}
}
