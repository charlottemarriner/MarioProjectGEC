#include "GameScreenMenu.h"
#include <iostream>
#include "Texture2D.h"

GameScreenMenu::GameScreenMenu(SDL_Renderer* renderer) : GameScreen(renderer)
{
	startGame = false;
	twoPlayers = false;
	SetUpLevel();
}

GameScreenMenu::~GameScreenMenu()
{
	delete mBackgroundTexture;
	mBackgroundTexture = NULL;
}

void GameScreenMenu::Render()
{
	//Draw the background
	mBackgroundTexture->Render(Vector2D(), SDL_FLIP_NONE);
}

void GameScreenMenu::Update(float deltaTime, SDL_Event e)
{
	switch (e.type)
	{
	case SDL_KEYDOWN:
		switch (e.key.keysym.sym)
		{
		case SDLK_UP:
			twoPlayers = false;
			SetUpLevel();
			break;
		case SDLK_DOWN:
			twoPlayers = true;
			SetUpLevel();
			break;
		case SDLK_SPACE:
			if (twoPlayers == false)
			{
				GameScreenManager::Instance->ChangeScreen(SCREEN_LEVEL1_ONEPLAYER);
			}
			else if (twoPlayers == true)
			{
				GameScreenManager::Instance->ChangeScreen(SCREEN_LEVEL1_TWOPLAYER);
			}
		}
	case SDL_KEYUP:
		switch (e.key.keysym.sym)
		{
		case SDLK_UP:
			break;
		case SDLK_DOWN:
			break;
		case SDLK_SPACE:
			break;
		}
	}
}

bool GameScreenMenu::SetUpLevel()
{
	if (!twoPlayers)
	{
		//Load the background texture for one player
		mBackgroundTexture = new Texture2D(mRenderer);
		if (!mBackgroundTexture->LoadFrom("Images/StartMenu.png"))
		{
			cout << "Failed to load background texture!";
			return false;
		}
	}
	else if (twoPlayers)
	{
		//Load the background texture for two players
		mBackgroundTexture = new Texture2D(mRenderer);
		if (!mBackgroundTexture->LoadFrom("Images/StartMenu2.png"))
		{
			cout << "Failed to load background texture!";
			return false;
		}
	}
}
