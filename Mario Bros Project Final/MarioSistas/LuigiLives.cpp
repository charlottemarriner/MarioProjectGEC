#include "LuigiLives.h"

LuigiLives::LuigiLives(SDL_Renderer* renderer, string imagePath, LevelMap* map, Vector2D startPosition) : Character(renderer, imagePath, startPosition, map)
{
	mPosition = startPosition;
}

LuigiLives::~LuigiLives()
{
}

void LuigiLives::AddGravity(float deltaTime)
{
}
