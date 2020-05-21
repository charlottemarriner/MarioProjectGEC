#include "GameScreenManager.h"
#include "GameScreen.h"
#include "GameScreenMenu.h"
#include <iostream>
#include <SDL.h>

GameScreenManager* GameScreenManager::Instance = nullptr;

GameScreenManager::GameScreenManager(SDL_Renderer* renderer, SCREENS startScreen)
{
	gameStart = false;
	twoPlayers = false;

	level1, level2, level3, level4, level5 = false;
	luigiLives = 3;
	marioLives = 3;
	score = 0;
	multiplierCount = 0;
	multiplierCountDown = MULTIPLIER_COUNTDOWN;

	mRenderer = renderer;
	mCurrentScreen = NULL;

	//Ensure the first screen is set up
	ChangeScreen(startScreen);
}

GameScreenManager::~GameScreenManager()
{
	mRenderer = NULL;
	delete mCurrentScreen;
	mCurrentScreen = NULL;
}

void GameScreenManager::UpdateScore()
{
	if (score > 0)
	{
		multiplierCount++;
		if (multiplierCountDown > 0)
		{
			score = score + (800 * multiplierCount);
			multiplierCountDown = MULTIPLIER_COUNTDOWN;
		}
		else if (multiplierCountDown <= 0)
		{
			multiplierCount = 0;
			multiplierCountDown = MULTIPLIER_COUNTDOWN;
		}
	}

	mTexthandler = nullptr;

	std::string y;
	y = "Score: " + std::to_string(score);
	mTexthandler = new Text(Window::gRenderer, "Images/OCRAEXT.ttf", 20, y, { 255,255,255,255 });
}

void GameScreenManager::Render()
{
	mCurrentScreen->Render();
}

void GameScreenManager::Update(float deltaTime, SDL_Event e)
{
	mCurrentScreen->Update(deltaTime, e);

	if (multiplierCount > 0)
	{
		multiplierCountDown -= deltaTime;
	}
}

void GameScreenManager::ChangeScreen(SCREENS newScreen)
{
	//Clear up old screen
  	if (mCurrentScreen != NULL)
	{
		mCurrentScreen = NULL;
		delete mCurrentScreen;
	}

	GameScreenMenu* tempScreenMenu;
	GameScreenLevel1TwoPlayer* tempScreenLevel1TwoPlayer;
	GameScreenLevel1OnePlayer* tempScreenLevel1OnePlayer;
	GameScreenLevel2TwoPlayer* tempScreenLevel2TwoPlayer;
	GameScreenLevel2OnePlayer* tempScreenLevel2OnePlayer;
	GameScreenLevel3TwoPlayer* tempScreenLevel3TwoPlayer;
	GameScreenLevel3OnePlayer* tempScreenLevel3OnePlayer;
	GameScreenLevel4TwoPlayer* tempScreenLevel4TwoPlayer;
	GameScreenLevel4OnePlayer* tempScreenLevel4OnePlayer;
	GameScreenLevel5TwoPlayer* tempScreenLevel5TwoPlayer;
	GameScreenLevel5OnePlayer* tempScreenLevel5OnePlayer;
	GameOverScreen* tempScreenGameOver;

		switch (newScreen)
		{
		case SCREEN_MENU:
			tempScreenMenu = new GameScreenMenu(mRenderer);
			mCurrentScreen = (GameScreen*)tempScreenMenu;
			tempScreenMenu = NULL;
			break;
		case SCREEN_LEVEL1_TWOPLAYER:
			tempScreenLevel1TwoPlayer = new GameScreenLevel1TwoPlayer(mRenderer);
			mCurrentScreen = (GameScreen*)tempScreenLevel1TwoPlayer;
			tempScreenLevel1TwoPlayer = NULL;
			gameStart = true;
			twoPlayers = true;
			level1 = true;
			break;
		case SCREEN_LEVEL1_ONEPLAYER:
			tempScreenLevel1OnePlayer = new GameScreenLevel1OnePlayer(mRenderer);
			mCurrentScreen = (GameScreen*)tempScreenLevel1OnePlayer;
			tempScreenLevel1OnePlayer = NULL;
			gameStart = true;
			twoPlayers = false;
			level1 = true;
			break;
		case SCREEN_LEVEL2_TWOPLAYER:
			tempScreenLevel2TwoPlayer = new GameScreenLevel2TwoPlayer(mRenderer);
			mCurrentScreen = (GameScreen*)tempScreenLevel2TwoPlayer;
			tempScreenLevel2TwoPlayer = NULL;
			level2 = true;
			level1 = false;
			break;
		case SCREEN_LEVEL2_ONEPLAYER:
			tempScreenLevel2OnePlayer = new GameScreenLevel2OnePlayer(mRenderer);
			mCurrentScreen = (GameScreen*)tempScreenLevel2OnePlayer;
			tempScreenLevel2OnePlayer = NULL;
			level2 = true;
			level1 = false;
			break;
		case SCREEN_LEVEL3_TWOPLAYER:
			tempScreenLevel3TwoPlayer = new GameScreenLevel3TwoPlayer(mRenderer);
			mCurrentScreen = (GameScreen*)tempScreenLevel3TwoPlayer;
			tempScreenLevel3TwoPlayer = NULL;
			level3 = true;
			level2 = false;
			break;
		case SCREEN_LEVEL3_ONEPLAYER:
			tempScreenLevel3OnePlayer = new GameScreenLevel3OnePlayer(mRenderer);
			mCurrentScreen = (GameScreen*)tempScreenLevel3OnePlayer;
			tempScreenLevel3OnePlayer = NULL;
			level3 = true;
			level2 = false;
			break;
		case SCREEN_LEVEL4_TWOPLAYER:
			tempScreenLevel4TwoPlayer = new GameScreenLevel4TwoPlayer(mRenderer);
			mCurrentScreen = (GameScreen*)tempScreenLevel4TwoPlayer;
			tempScreenLevel4TwoPlayer = NULL;
			level4 = true;
			level3 = false;
			break;
		case SCREEN_LEVEL4_ONEPLAYER:
			tempScreenLevel4OnePlayer = new GameScreenLevel4OnePlayer(mRenderer);
			mCurrentScreen = (GameScreen*)tempScreenLevel4OnePlayer;
			tempScreenLevel4OnePlayer = NULL;
			level4 = true;
			level3 = false;
			break;
		case SCREEN_LEVEL5_TWOPLAYER:
			tempScreenLevel5TwoPlayer = new GameScreenLevel5TwoPlayer(mRenderer);
			mCurrentScreen = (GameScreen*)tempScreenLevel5TwoPlayer;
			tempScreenLevel5TwoPlayer = NULL;
			level5 = true;
			level4 = false;
			break;
		case SCREEN_LEVEL5_ONEPLAYER:
			tempScreenLevel5OnePlayer = new GameScreenLevel5OnePlayer(mRenderer);
			mCurrentScreen = (GameScreen*)tempScreenLevel5OnePlayer;
			tempScreenLevel5OnePlayer = NULL;
			level5 = true;
			level4 = false;
			break;
		case SCREEN_GAMEOVER:
			tempScreenGameOver = new GameOverScreen(mRenderer);
			mCurrentScreen = (GameScreen*)tempScreenGameOver;
			tempScreenGameOver = NULL;
			gameStart = false;
			level5 = false;
			break;
		}
}

