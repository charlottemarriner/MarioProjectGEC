#include "MarioLives.h"

MarioLives::MarioLives(SDL_Renderer* renderer, string imagePath, LevelMap* map, Vector2D startPosition) : Character(renderer, imagePath, startPosition, map)
{
	mPosition = startPosition;
}

MarioLives::~MarioLives()
{
}

void MarioLives::AddGravity(float deltaTime)
{
}
