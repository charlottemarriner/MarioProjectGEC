#include "GameOverScreen.h"

GameOverScreen::GameOverScreen(SDL_Renderer* renderer) : GameScreen(renderer)
{
	restartGame = false;
	SetUpLevel();
}

GameOverScreen::~GameOverScreen()
{
	delete mBackgroundTexture;
	mBackgroundTexture = NULL;
}

void GameOverScreen::Render()
{
	//Draw the background
	mBackgroundTexture->Render(Vector2D(), SDL_FLIP_NONE);
}

void GameOverScreen::Update(float deltaTime, SDL_Event e)
{
	switch (e.type)
	{
	case SDL_KEYDOWN:
		switch (e.key.keysym.sym)
		{
		case SDLK_SPACE:
			GameScreenManager::Instance->ChangeScreen(SCREEN_MENU);
			GameScreenManager::Instance->gameStart = false;
			break;
		}
	case SDL_KEYUP:
		switch (e.key.keysym.sym)
		{
		case SDLK_SPACE:
			break;
		}
	}
}

bool GameOverScreen::SetUpLevel()
{
	//Load the background texture for one player
	mBackgroundTexture = new Texture2D(mRenderer);
	if (!mBackgroundTexture->LoadFrom("Images/Gameover.png"))
	{
		cout << "Failed to load background texture!";
		return false;
	}
}
