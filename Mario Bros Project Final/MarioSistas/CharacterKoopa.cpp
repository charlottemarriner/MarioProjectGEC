#include "CharacterKoopa.h"

CharacterKoopa::CharacterKoopa(SDL_Renderer* renderer, string imagePath, LevelMap* map, Vector2D startPosition, FACING facing, float paramSpeed) : Character(renderer, imagePath, startPosition, map)
{
	mFacingDirection = facing;
	paramSpeed = paramSpeed;
	mPosition = startPosition;
	leftPipe = true;
	mCurrentFrame = 0;

	mInjured = false;
	mNotKilled = false;
}

CharacterKoopa::~CharacterKoopa()
{
}

void CharacterKoopa::Update(float deltaTime, SDL_Event e)
{
	//To gain the benefit of the code written in the base class Update() function we need to call it
	Character::Update(deltaTime, e);
	
	//Setup the Koopa Spritesheet
	mSingleSpriteWidth = mTexture->GetWidth() / 6; //6 sprites on this spritesheet in 1 row
	mSingleSpriteHeight = mTexture->GetHeight();

	//Collision position variables
	int centralXPosition = (int)(mPosition.x + (mSingleSpriteWidth * 0.5f)) / TILE_WIDTH;
	int footPosition = (int)(mPosition.y + mSingleSpriteHeight) / TILE_HEIGHT;
	int headPosition = (int)(mPosition.y) / TILE_HEIGHT;

	mFrameDelay -= deltaTime;

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

	if (!mInjured)
	{
		if (mFrameDelay <= 0.0f)
		{
			//Reset frame delay count
			mFrameDelay = ANIMATION_DELAY;

			//Move frame on
			mCurrentFrame++;

			//Loop frame around if it goes beyond the number of frames
			if (mCurrentFrame > 4)
			{
				mCurrentFrame = 0;
			}
		}
		//We are not injured so move
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
	else
	{
		if (mFrameDelay <= 0.0f)
		{
			//Reset frame delay count
			mFrameDelay = ANIMATION_DELAY;

			mCurrentFrame = 5;
		}
		//We should not be moving when injured
		mMovingRight = false;
		mMovingLeft = false;

		//Count down the injured time
		mInjuredTime -= deltaTime;

		if (mInjuredTime <= 0.0)
		{
			FlipRightWayUp();
		}
	}
	
}

void CharacterKoopa::Render()
{
	//Variable to hold the left position of the sprite we want to draw
	int left = 0.0f;

	//Get the portion of the spritesheet you want to draw.
	//								{XPos, YPos, WidthOfSingleSprite, HeightOfSingleSprite}
	SDL_Rect portionOfSpriteSheet = { mSingleSpriteWidth * mCurrentFrame, 0, mSingleSpriteWidth, mSingleSpriteHeight };

	//Determine where you want it drawn
	SDL_Rect destRect = { (int)(mPosition.x), (int)(mPosition.y), mSingleSpriteWidth, mSingleSpriteHeight };

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

void CharacterKoopa::TakeDamage()
{
	mInjured = true;
	mInjuredTime = INJURED_TIME;
	Jump();
}

void CharacterKoopa::Jump()
{
	if (!mJumping)
	{
		mJumpForce = INITIAL_JUMP_FORCE;

		mJumping = true;
		mCanJump = false;
	}
}

void CharacterKoopa::RespawnAtTop()
{
	if (leftPipe)
	{
		mMovingRight = true;
		mMovingLeft = false;
		mPosition.x = 60;
		mPosition.y = 22;
		leftPipe = false;
	}
	else if (!leftPipe)
	{
		mMovingLeft = true;
		mMovingRight = false;
		mPosition.x = 420; //blaze it
		mPosition.y = 22;
		leftPipe = true;
	}
}

void CharacterKoopa::MoveLeft(float deltaTime)
{
	mPosition.x -= KOOPA_SPEEP;
	mFacingDirection = FACING_LEFT;
}

void CharacterKoopa::MoveRight(float deltaTime)
{
	mPosition.x += KOOPA_SPEEP;
	mFacingDirection = FACING_RIGHT;
}

void CharacterKoopa::SwapFacing()
{
	if (mFacingDirection == FACING_LEFT)
	{
		mMovingLeft = false;
		mMovingRight = true;

	}
	else if (mFacingDirection == FACING_RIGHT)
	{
		mMovingRight = false;
		mMovingLeft = true;

	}
}

void CharacterKoopa::FlipRightWayUp()
{
	SwapFacing();
	mInjured = false;
	Jump();
}


