#include "GameScreenLevel3OnePlayer.h"
#include <iostream>
#include "Texture2D.h"
#include "Constants.h"

GameScreenLevel3OnePlayer::GameScreenLevel3OnePlayer(SDL_Renderer* renderer) : GameScreen(renderer)
{
	SetUpLevel();
}

GameScreenLevel3OnePlayer::~GameScreenLevel3OnePlayer()
{
	delete mBackgroundTexture;
	mBackgroundTexture = NULL;
	delete myCharacter;
	myCharacter = NULL;

	delete mLevelMap;
	mLevelMap = NULL;

	mBonusLevelCoins.clear();
	mMarioLives.clear();
}

void GameScreenLevel3OnePlayer::Update(float deltaTime, SDL_Event e)
{
	//Check if game has been won or lost
	if ((mCountdownTime <= 0))
	{
		GameScreenManager::Instance->ChangeScreen(SCREEN_LEVEL4_ONEPLAYER);
	}


	myCharacter->Update(deltaTime, e);

	UpdateCoins(deltaTime, e);

	mCountdownTime -= deltaTime;
}

void GameScreenLevel3OnePlayer::Render()
{
	//Draw the background texture
	mBackgroundTexture->Render(Vector2D(0, mBackgroundYPos), SDL_FLIP_NONE);

	//Call to character render function
	myCharacter->Render();

	//Draw the coins
	for (unsigned int i = 0; i < mBonusLevelCoins.size(); i++)
	{
		mBonusLevelCoins[i]->Render();
	}

	//Draw Mario Lives
	for (unsigned int i = 0; i < mMarioLives.size(); i++)
	{
		mMarioLives[i]->Render();
	}
}

bool GameScreenLevel3OnePlayer::SetUpLevel()
{
	SetLevelMap();

	//Set up the player character
	myCharacter = new CharacterMario(mRenderer, "Images/Mario.png", Vector2D(64, 330), mLevelMap);

	//Load the background texture
	mBackgroundTexture = new Texture2D(mRenderer);

	GameScreenManager::Instance->UpdateScore();
	GameScreenManager::Instance->multiplierCount = 0;

	mCountdownTime = ENEMYSPAWNCOUNTDOWN;
	mCountdown = true;
	mBackgroundYPos = 0.0f;
	spawnSwapCounter = false;

	//Set up Mario lives
	if (GameScreenManager::Instance->marioLives == 3)
	{
		CreateMarioLives(Vector2D(200, 10));
		CreateMarioLives(Vector2D(220, 10));
		CreateMarioLives(Vector2D(240, 10));
	}
	else if (GameScreenManager::Instance->marioLives == 2)
	{
		CreateMarioLives(Vector2D(200, 10));
		CreateMarioLives(Vector2D(220, 10));
	}
	else if (GameScreenManager::Instance->marioLives == 1)
	{
		CreateMarioLives(Vector2D(200, 10));
	}

	//Spawn all bonus coins on map
	CreateCoin(Vector2D(77, 10));
	CreateCoin(Vector2D(410, 10));
	CreateCoin(Vector2D(16, 95));
	CreateCoin(Vector2D(75, 95));
	CreateCoin(Vector2D(405, 95));
	CreateCoin(Vector2D(458, 95));
	CreateCoin(Vector2D(200, 195));
	CreateCoin(Vector2D(287, 194));
	CreateCoin(Vector2D(65, 325));
	CreateCoin(Vector2D(416, 325));


	//Check if background texture loads
	if (!mBackgroundTexture->LoadFrom("Images/Background3.png"))
	{
		cout << "Failed to load background texture!";
		return false;
	}
	else
		return true;
}

void GameScreenLevel3OnePlayer::SetLevelMap()
{
	int map[MAP_HEIGHT][MAP_WIDTH] = { { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
										{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
										{ 1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1 },
										{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
										{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
										{ 0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0 },
										{ 1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1 },
										{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
										{ 0,0,0,0,0,0,0,2,2,0,0,0,0,0,0,0 },
										{ 1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1 },
										{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
										{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
										{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 } };

	//Clear up any old map
	if (mLevelMap != NULL)
	{
		delete mLevelMap;
		//delete mPowBlock;
	}

	//Set up the new one
	mLevelMap = new LevelMap(map);
}

void GameScreenLevel3OnePlayer::UpdateCoins(float deltaTime, SDL_Event e)
{
	if (!mBonusLevelCoins.empty())
	{
		int enemyIndexToDelete = -1;
		for (unsigned int i = 0; i < mBonusLevelCoins.size(); i++)
		{
			//now do the update

			mBonusLevelCoins[i]->Update(deltaTime, e);

			//check to see if enemy collides with player
			if ((mBonusLevelCoins[i]->GetPosition().y > 300.0f || mBonusLevelCoins[i]->GetPosition().y <= 64.0f) && (mBonusLevelCoins[i]->GetPosition().x < 64.0f || mBonusLevelCoins[i]->GetPosition().x > SCREEN_WIDTH - 96.0f))
			{
				//ignore collsions if behind pipe
			}
			else
			{
				if (Collisions::Instance()->Circle(mBonusLevelCoins[i], myCharacter))
				{
					mBonusLevelCoins[i]->SetAlive(false);
					gSoundEffect->CoinSound();
				}
			}

			//if the enemy is no longer alive then schedule it for deletion
			if (!mBonusLevelCoins[i]->GetAlive())
			{
				enemyIndexToDelete = i;
			}
		}

		//remove dead enemies -1 each update

		if (enemyIndexToDelete != -1)
		{
			mBonusLevelCoins.erase(mBonusLevelCoins.begin() + enemyIndexToDelete);
			GameScreenManager::Instance->score = GameScreenManager::Instance->score + 800;
			GameScreenManager::Instance->UpdateScore();
		}
	}
	else
	{
		GameScreenManager::Instance->ChangeScreen(SCREEN_LEVEL4_ONEPLAYER);
	}
}

void GameScreenLevel3OnePlayer::CreateCoin(Vector2D position)
{
	BonusLevelCoins* Coins = new BonusLevelCoins(mRenderer, "Images/Coin.png", mLevelMap, position);
	mBonusLevelCoins.push_back(Coins);
}

void GameScreenLevel3OnePlayer::CreateMarioLives(Vector2D position)
{
	MarioLives* Lives = new MarioLives(mRenderer, "Images/MarioLives.png", mLevelMap, position);
	mMarioLives.push_back(Lives);
}

void GameScreenLevel3OnePlayer::UpdateLives()
{
	if (GameScreenManager::Instance->marioLives == 2)
	{
		mMarioLives.erase(mMarioLives.begin() + 2);
	}
	else if (GameScreenManager::Instance->marioLives == 1)
	{
		mMarioLives.erase(mMarioLives.begin() + 1);
	}
	else if (GameScreenManager::Instance->marioLives == 0)
	{
		mMarioLives.erase(mMarioLives.begin());
	}
}
