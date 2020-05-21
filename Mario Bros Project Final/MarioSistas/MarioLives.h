#pragma once
#include "Character.h"
#include <SDL.h>
#include <iostream>
#include "Commons.h"
#include <string>
#include "Constants.h"

using namespace std;

class Texture2D;

class MarioLives : public Character
{
public:
	MarioLives(SDL_Renderer* renderer, string imagePath, LevelMap* map, Vector2D startPosition);
	~MarioLives();
	virtual void AddGravity(float deltaTime) override;
};

