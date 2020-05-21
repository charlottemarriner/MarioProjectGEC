#pragma once
#include "Character.h"
#include "Constants.h"
#include "Commons.h"
#include <SDL.h>
#include <iostream>
#include <string>

using namespace std;

class Texture2D;

class CharacterMario : public Character
{
public:
	CharacterMario(SDL_Renderer* renderer, string imagePath, Vector2D startPosition, LevelMap* map);
	~CharacterMario();

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
};
