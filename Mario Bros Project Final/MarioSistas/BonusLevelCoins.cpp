#include "BonusLevelCoins.h"

BonusLevelCoins::BonusLevelCoins(SDL_Renderer* renderer, string imagePath, LevelMap* map, Vector2D startPosition) : Character(renderer, imagePath, startPosition, map)
{
	mPosition = startPosition;
	mCurrentFrame = 0;

	mSingleSpriteWidthCoin = mTexture->GetWidth() / 3; //3 sprites on this spritesheet in 1 row
	mSingleSpriteHeightCoin = mTexture->GetHeight();
}

BonusLevelCoins::~BonusLevelCoins()
{
}

void BonusLevelCoins::Update(float deltaTime, SDL_Event e)
{
	//To gain the benefit of the code written in the base class Update() function we need to call it
	Character::Update(deltaTime, e);

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
}

void BonusLevelCoins::Render()
{
	//Variable to hold the left position of the sprite we want to draw
	int left = 0.0f;

	//Get the portion of the spritesheet you want to draw.
	//								{XPos, YPos, WidthOfSingleSprite, HeightOfSingleSprite}
	SDL_Rect portionOfSpriteSheet = { mSingleSpriteWidthCoin * mCurrentFrame, 0, mSingleSpriteWidthCoin, mSingleSpriteHeightCoin };

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

//Don't want gravity to effect level 3 bonus coins so override gravity
void BonusLevelCoins::AddGravity(float deltaTime)
{
}
