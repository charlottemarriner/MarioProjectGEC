#pragma once
#include <SDL.h>
#include <string>
#include <iostream>
#include "Character.h"
#include "Constants.h"
#include "Commons.h"
#include "Texture2D.h"

class CharacterKrab : public Character
{
public:
	CharacterKrab(SDL_Renderer* renderer, string imagePath, LevelMap* map, Vector2D startPosition, FACING facing, float paramSpeed);
	~CharacterKrab();
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

	bool krabHit;
	bool mInjured;
	float mInjuredTime;
	void FlipRightWayUp();

	bool leftPipe;
};

