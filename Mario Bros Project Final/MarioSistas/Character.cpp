#include "Character.h"
#include "Texture2D.h"
#include "Constants.h"


Character::Character(SDL_Renderer* renderer, string imagePath, Vector2D startPosition, LevelMap* map)
{
	mRenderer = renderer;
	mPosition = startPosition;
	mTexture = new Texture2D(mRenderer);
	mTexture->LoadFrom(imagePath);
	mFacingDirection = FACING_RIGHT;
	mMovingLeft = false;
	mMovingRight = false;
	mCollisionRadius = 15.0f;
	mCurrentLevelMap = map;

	alive = true;
}

Character::~Character()
{
	delete mRenderer;
	mRenderer = NULL;
	delete mTexture;
	mTexture = NULL;
}

void Character::Render()
{
	mTexture->Render(mPosition, SDL_FLIP_NONE);

	if (mFacingDirection == FACING_RIGHT)
	{
		mTexture->Render(mPosition, SDL_FLIP_NONE);
	}
	else
	{
		mTexture->Render(mPosition, SDL_FLIP_HORIZONTAL);
	}
}

void Character::Update(float deltaTime, SDL_Event e)
{
	if (mJumping)
	{
		mPosition.y -= mJumpForce * deltaTime;

		mJumpForce -= JUMP_FORCE_DECREMENT * deltaTime;

		if (mJumpForce <= 0.0f)
		{
			mJumping = false;
		}
	}

	if (mMovingLeft)
	{
		MoveLeft(deltaTime);
	}
	else if (mMovingRight)
	{
		MoveRight(deltaTime);
	}
}

void Character::MoveLeft(float deltaTime)
{
	mPosition.x -= MOVEMENTSPEED;
	mFacingDirection = FACING_LEFT;
}

void Character::MoveRight(float deltaTime)
{
	mPosition.x += MOVEMENTSPEED;
	mFacingDirection = FACING_RIGHT;
}

void Character::Jump()
{
	if (!mJumping)
	{
		mJumpForce = INITIAL_JUMP_FORCE;
		mJumping = true;
		mCanJump = false;
	}
	gSoundEffect->JumpSound();
}

void Character::AddGravity(float deltaTime)
{
	mPosition.y += GRAVITY * deltaTime;
	mCanJump = false;
}

void Character::SetPosition(Vector2D newPosition)
{
	mPosition = newPosition;
}

Vector2D Character::GetPosition()
{
	return mPosition;
}

float Character::GetCollisionRadius()
{
	return mCollisionRadius;
}

Rect2D Character::GetCollisionBox()
{
	return Rect2D(mPosition.x, mPosition.y, (mTexture->GetWidth()/6), mTexture->GetHeight());
}

void Character::SetAlive(bool isAlive)
{
	alive = isAlive;
}
