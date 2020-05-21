#pragma once
#include <SDL.h>
#include <string>
#include <iostream>
#include "Character.h"
#include "Constants.h"
#include "Commons.h"
#include "Texture2D.h"

class CharacterKoopa : public Character
{
public:
	CharacterKoopa(SDL_Renderer* renderer, string imagePath, LevelMap* map, Vector2D startPosition, FACING facing, float paramSpeed);
	~CharacterKoopa();

	void Update(float deltaTime, SDL_Event e);
	void Render();

	void MoveLeft(float deltaTime);
	void MoveRight(float deltaTime);
	void SwapFacing();
	void Jump();

	void RespawnAtTop();

	void TakeDamage();
	bool GetInjured() { return mInjured; };
private:
	float mSingleSpriteWidth;
	float mSingleSpriteHeight;
	float mFrameDelay;
	float mCurrentFrame;

	float mInjuredTime;
	bool mInjured;
	void FlipRightWayUp();

	bool mNotKilled;

	bool leftPipe;
};

