#pragma once
#include <SDL.h>
#include <string>
#include <iostream>
#include "Character.h"
#include "Constants.h"
#include "Commons.h"
#include "Texture2D.h"

class BonusLevelCoins : public Character
{
public:
	BonusLevelCoins(SDL_Renderer* renderer, string imagePath, LevelMap* map, Vector2D startPosition);
	~BonusLevelCoins();

	void Update(float deltaTime, SDL_Event e);
	void Render();

	virtual void AddGravity(float deltaTime) override;

private:
	float mSingleSpriteWidthCoin;
	float mSingleSpriteHeightCoin;

	float mFrameDelay;
	float mCurrentFrame;

};

