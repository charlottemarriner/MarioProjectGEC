#pragma once
#include <SDL.h>
#include "GameScreen.h"
#include "GameScreenManager.h"
#include "Commons.h"
#include "Texture2D.h"

class Texture2D;
class GameScreenManager;

class GameScreenMenu : GameScreen
{
public:
	GameScreenMenu(SDL_Renderer* renderer);
	~GameScreenMenu();

	bool startGame;
	bool twoPlayers;

	void Render();
	void Update(float deltaTime, SDL_Event e);

private:
	Texture2D* mBackgroundTexture;

	bool SetUpLevel();
};

