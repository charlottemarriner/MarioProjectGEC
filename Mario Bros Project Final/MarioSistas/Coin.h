#pragma once
#include <SDL.h>
#include <string>
#include <iostream>
#include "Character.h"
#include "Constants.h"
#include "Commons.h"
#include "Texture2D.h"

class Coin : public Character
{
public:
	Coin(SDL_Renderer* renderer, string imagePath, LevelMap* map, Vector2D startPosition, FACING facing, float paramSpeed);
	~Coin();

	void Update(float deltaTime, SDL_Event e);
	void Render();

	void MoveLeft(float deltaTime);
	void MoveRight(float deltaTime);
	void SwapFacing();

private:
	float mSingleSpriteWidthCoin;
	float mSingleSpriteHeightCoin;

	float mFrameDelay;
	float mCurrentFrame;
};

