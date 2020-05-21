#pragma once
#include <SDL.h>
#include "Commons.h"
#include "GameScreen.h"
#include "GameScreenManager.h"
#include "Texture2D.h"

class Texture2D;
class GameScreenManager;

class GameOverScreen : GameScreen
{
public:
	GameOverScreen(SDL_Renderer* renderer);
	~GameOverScreen();

	bool restartGame;

	void Render();
	void Update(float deltaTime, SDL_Event e);

private:
	Texture2D* mBackgroundTexture;

	bool SetUpLevel();
};

