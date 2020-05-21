#pragma once
#include "Window.h"
#include <SDL.h>
#include <string>
#include "Commons.h"
#include "Constants.h"
#include "GameScreen.h"
#include "GameScreenMenu.h"
#include "GameScreenLevel1TwoPlayer.h"
#include "GameScreenLevel1OnePlayer.h"
#include "GameScreenLevel2TwoPlayer.h"
#include "GameScreenLevel2OnePlayer.h"
#include "GameScreenLevel3TwoPlayer.h"
#include "GameScreenLevel3OnePlayer.h"
#include "GameScreenLevel4TwoPlayer.h"
#include "GameScreenLevel4OnePlayer.h"
#include "GameScreenLevel5TwoPlayer.h"
#include "GameScreenLevel5OnePlayer.h"
#include "GameOverScreen.h"
#include "Text.h"


class GameScreen;

class GameScreenManager
{
private:
	SDL_Renderer* mRenderer;
	GameScreen* mCurrentScreen;

public:
	GameScreenManager(SDL_Renderer* renderer, SCREENS startScreen);
	~GameScreenManager();

	static GameScreenManager* Instance;
	Text* mTexthandler;

	int multiplierCount;
	float multiplierCountDown;

	bool gameStart;
	bool twoPlayers;
	
	bool level1;
	bool level2;
	bool level3;
	bool level4;
	bool level5;

	int luigiLives;
	int marioLives;
	int score;
	string y;
	

	void UpdateScore();

	void Render();
	void Update(float deltaTime, SDL_Event e);

	void ChangeScreen(SCREENS newScreen);
};
