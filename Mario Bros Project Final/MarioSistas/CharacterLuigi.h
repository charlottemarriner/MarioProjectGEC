#pragma once
#include "Character.h"
#include <SDL.h>
#include <iostream>
#include "Commons.h"
#include <string>
#include "Constants.h"

using namespace std;

class Texture2D;

class CharacterLuigi : public Character
{
public:
	CharacterLuigi(SDL_Renderer* renderer, string imagePath, Vector2D startPosition, LevelMap* map);
	~CharacterLuigi();

	void Update(float deltaTime, SDL_Event e);
	void Death();
	void Render();
	
protected:
	virtual void MoveLeft(float deltaTime) override;
	virtual void MoveRight(float deltaTime) override;
private:
	float mSingleSpriteWidth;
	float mSingleSpriteHeight;
	float mFrameDelay;
	float mCurrentFrame;

	int dead;
};
