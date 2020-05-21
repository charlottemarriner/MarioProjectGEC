#include "GameScreenLevel5TwoPlayer.h"
#include <iostream>
#include "Texture2D.h"
#include "Constants.h"

GameScreenLevel5TwoPlayer::GameScreenLevel5TwoPlayer(SDL_Renderer* renderer) : GameScreen(renderer)
{
	SetUpLevel();
}

GameScreenLevel5TwoPlayer::~GameScreenLevel5TwoPlayer()
{
	delete mBackgroundTexture;
	mBackgroundTexture = NULL;
	delete myCharacter;
	myCharacter = NULL;
	delete myCharacter2;
	myCharacter2 = NULL;
	delete mPowBlock;
	mPowBlock = NULL;

	delete mLevelMap;
	mLevelMap = NULL;

	mKoopa.clear();
	mCoins.clear();
	mKrab.clear();
	mMarioLives.clear();
	mLuigiLives.clear();
}

void GameScreenLevel5TwoPlayer::Update(float deltaTime, SDL_Event e)
{
	Vector2D preP1 = myCharacter->GetPosition();
	Vector2D preP2 = myCharacter2->GetPosition();

	//Check if game has been won or lost
	if ((koopaKilled == 2) && (krabKilled == 4))
	{
		GameScreenManager::Instance->ChangeScreen(SCREEN_MENU);
		GameScreenManager::Instance->gameStart = false;
	}
	else if ((GameScreenManager::Instance->marioLives == 0) && (GameScreenManager::Instance->luigiLives == 0))
	{
		GameScreenManager::Instance->ChangeScreen(SCREEN_GAMEOVER);
	}


	myCharacter->Update(deltaTime, e);
	myCharacter2->Update(deltaTime, e);

	UpdatePowBlock();
	UpdateKrab(deltaTime, e);
	UpdateKoopa(deltaTime, e);
	UpdateCoins(deltaTime, e);

	mCountdownTime -= deltaTime;


	//Respawm enemies
	if (mCountdownTime <= 0 && mCountdown == true)
	{
		if ((mKoopaSpawnNum < KOOPA_NUM_LEVEL5) || (mKrabSpawnNum < KRAB_NUM_LEVEL5))
		{
			mCountdown = false;
			CountdownRespawn();
		}
	}

	//Do the screen shake :D
	if (mScreenshake)
	{
		mScreenshakeTime -= deltaTime;
		mWobble++;
		mBackgroundYPos = sin(mWobble);
		mBackgroundYPos *= 3.0f;

		//End the shake after the duration
		if (mScreenshakeTime <= 0.0f)
		{
			mScreenshake = false;
			mBackgroundYPos = 0.0f;
		}
	}


	if (Collisions::Instance()->Box(myCharacter->GetCollisionBox(), myCharacter2->GetCollisionBox()))
	{
		myCharacter->SetPosition(preP1);
		myCharacter2->SetPosition(preP2);
	}
}

void GameScreenLevel5TwoPlayer::Render()
{
	//Draw the background texture
	mBackgroundTexture->Render(Vector2D(0, mBackgroundYPos), SDL_FLIP_NONE);

	//Call to character render function
	myCharacter->Render();
	myCharacter2->Render();
	mPowBlock->Render();

	//Draw the enemies
	for (unsigned int i = 0; i < mKoopa.size(); i++)
	{
		mKoopa[i]->Render();
	}

	//Draw the coins
	for (unsigned int i = 0; i < mCoins.size(); i++)
	{
		mCoins[i]->Render();
	}

	//Draw the Goomba
	for (unsigned int i = 0; i < mKrab.size(); i++)
	{
		mKrab[i]->Render();
	}

	//Draw the lives
	for (unsigned int i = 0; i < mMarioLives.size(); i++)
	{
		mMarioLives[i]->Render();
	}
	for (unsigned int i = 0; i < mLuigiLives.size(); i++)
	{
		mLuigiLives[i]->Render();
	}
}

bool GameScreenLevel5TwoPlayer::SetUpLevel()
{
	SetLevelMap();

	//Set up the player character
	myCharacter = new CharacterMario(mRenderer, "Images/Mario.png", Vector2D(64, 330), mLevelMap);
	myCharacter2 = new CharacterLuigi(mRenderer, "Images/Luigi.png", Vector2D(416, 330), mLevelMap);

	//Load the background texture
	mBackgroundTexture = new Texture2D(mRenderer);

	//Set up Pow Block
	mPowBlock = new PowBlock(mRenderer, mLevelMap);

	GameScreenManager::Instance->UpdateScore();
	GameScreenManager::Instance->multiplierCount = 0;

	mScreenshake = false;
	mCountdownTime = ENEMYSPAWNCOUNTDOWN;
	mCountdown = true;
	mBackgroundYPos = 0.0f;
	koopaKilled = 0;
	krabKilled = 0;
	playerOneDeath = false;
	playerTwoDeath = false;
	mKoopaSpawnNum = 0;
	mKrabSpawnNum = 0;
	mCurrentKoopaNum = 0;
	mCurrentKrabNum = 0;
	spawnSwapCounter = false;
	mCurrentCoinNum = 0;

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

	//Set up Luigi lives
	if (GameScreenManager::Instance->luigiLives == 3)
	{
		CreateLuigiLives(Vector2D(380, 10));
		CreateLuigiLives(Vector2D(400, 10));
		CreateLuigiLives(Vector2D(420, 10));
	}
	else if (GameScreenManager::Instance->luigiLives == 2)
	{
		CreateLuigiLives(Vector2D(380, 10));
		CreateLuigiLives(Vector2D(400, 10));
	}
	else if (GameScreenManager::Instance->luigiLives == 1)
	{
		CreateLuigiLives(Vector2D(380, 10));
	}

	//Set up 2 bad bois
	CreateKrab(Vector2D(60, 32), FACING_RIGHT, KOOPA_SPEEP);
	CreateKrab(Vector2D(420, 32), FACING_LEFT, KOOPA_SPEEP);

	//Check if background texture loads
	if (!mBackgroundTexture->LoadFrom("Images/Background5.png"))
	{
		cout << "Failed to load background texture!";
		return false;
	}
	else
		return true;
}

void GameScreenLevel5TwoPlayer::SetLevelMap()
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

void GameScreenLevel5TwoPlayer::UpdateKoopa(float deltaTime, SDL_Event e)
{
	if (!mKoopa.empty())
	{
		int enemyIndexToDelete = -1;
		for (unsigned int i = 0; i < mKoopa.size(); i++)
		{
			//Code to wrap enemies around screen
			//Top two pipes
			if (((mKoopa[i]->GetPosition().x < 50) && (mKoopa[i]->GetPosition().y < 50)) || ((mKoopa[i]->GetPosition().x > 420) && (mKoopa[i]->GetPosition().y < 50)))
			{
				mKoopa[i]->SwapFacing();
			}
			//bottom two pipes - kill when reach bottom two pipes
			else if (((mKoopa[i]->GetPosition().x < 50) && (mKoopa[i]->GetPosition().y > 320)) || ((mKoopa[i]->GetPosition().x > 420) && (mKoopa[i]->GetPosition().y > 320)))
			{
				mKoopa[i]->RespawnAtTop();
			}
			//middle area
			else if ((((mKoopa[i]->GetPosition().x < -20) && (22 <= (mKoopa[i]->GetPosition().y) <= 340))) || ((mKoopa[i]->GetPosition().x > 490) && (22 <= (mKoopa[i]->GetPosition().y) <= 340)))
			{
				mKoopa[i]->SwapFacing();
			}

			//COMMENTED OUT FOR NOW AS CODE ABOVE WORKS BETTER FOR PURPOSE
			////check if the enemy is on the bottom row of tiles
			//if (mEnemies[i]->GetPosition().y > 300.0f)
			//{
			//	//is the enemy off screen to the left / right?
			//	if (mEnemies[i]->GetPosition().x < (float)(-mEnemies[i]->GetCollisionBox().width * 0.5f) || mEnemies[i]->GetPosition().x > SCREEN_WIDTH - (float)(mEnemies[i]->GetCollisionBox().width * 0.55f))
			//	{
			//		
			//		mEnemies[i]->SetAlive(false);
			//	}
			//}


			//now do the update

			mKoopa[i]->Update(deltaTime, e);

			//check to see if enemy collides with player
			if ((mKoopa[i]->GetPosition().y > 300.0f || mKoopa[i]->GetPosition().y <= 64.0f) && (mKoopa[i]->GetPosition().x < 64.0f || mKoopa[i]->GetPosition().x > SCREEN_WIDTH - 96.0f))
			{
				//ignore collsions if behind pipe
			}
			else
			{
				if (Collisions::Instance()->Circle(mKoopa[i], myCharacter))
				{
					if (mKoopa[i]->GetInjured())
					{
						mKoopa[i]->SetAlive(false);
					}
					else
					{
						myCharacter->Death();
						GameScreenManager::Instance->marioLives--;
						playerOneDeath = true;
						UpdateLives();
					}
				}
				else if (Collisions::Instance()->Circle(mKoopa[i], myCharacter2))
				{
					if (mKoopa[i]->GetInjured())
					{
						mKoopa[i]->SetAlive(false);
					}
					else
					{
						myCharacter2->Death();
						GameScreenManager::Instance->luigiLives--;
						playerTwoDeath = true;
						UpdateLives();
					}
				}
			}

			//Check for collisions between Koopas
			if (mCurrentKoopaNum == 2)
			{
				if (Collisions::Instance()->Circle(mKoopa[0], mKoopa[1]))
				{
					mKoopa[0]->SwapFacing();
					mKoopa[1]->SwapFacing();
					mKoopa[0]->Jump();

				}
			}

			//Check for collision between koopas and krabs
			if (mCurrentKoopaNum == 1)
			{
				for (unsigned int i = 0; i < mKrab.size(); i++)
				{
					if (Collisions::Instance()->Circle(mKoopa[0], mKrab[i]))
					{
						mKoopa[0]->SwapFacing();
						mKrab[i]->SwapFacing();
						mKoopa[0]->Jump();
					}
				}
			}
			else if (mCurrentKoopaNum == 2)
			{
				for (unsigned int i = 0; i < mKrab.size(); i++)
				{
					if (Collisions::Instance()->Circle(mKoopa[0], mKrab[i]))
					{
						mKoopa[0]->SwapFacing();
						mKrab[i]->SwapFacing();
						mKoopa[0]->Jump();
					}
					else if (Collisions::Instance()->Circle(mKoopa[1], mKrab[i]))
					{
						mKoopa[1]->SwapFacing();
						mKrab[i]->SwapFacing();
						mKoopa[1]->Jump();
					}
				}
			}

			//if the enemy is no longer alive then schedule it for deletion
			if (!mKoopa[i]->GetAlive())
			{
				enemyIndexToDelete = i;
			}
		}

		//remove dead enemies -1 each update

		if (enemyIndexToDelete != -1)
		{
			mKoopa.erase(mKoopa.begin() + enemyIndexToDelete);
			mCurrentKoopaNum--;
			koopaKilled++;
			GameScreenManager::Instance->score = GameScreenManager::Instance->score + 800;
			GameScreenManager::Instance->UpdateScore();
			if (spawnSwapCounter)
			{
				CreateCoin(Vector2D(60, 32), FACING_LEFT, COIN_SPEED);
				spawnSwapCounter = false;
			}
			else if (!spawnSwapCounter)
			{
				CreateCoin(Vector2D(420, 32), FACING_RIGHT, COIN_SPEED);
				spawnSwapCounter = true;
			}
		}
	}
}

void GameScreenLevel5TwoPlayer::CreateKoopa(Vector2D position, FACING direction, float speed)
{
	CharacterKoopa* koopaCharacter = new CharacterKoopa(mRenderer, "Images/Koopa.png", mLevelMap, position, direction, speed);
	mKoopa.push_back(koopaCharacter);
	mCurrentKoopaNum++;
	mKoopaSpawnNum++;
	gSoundEffect->EnemySound();
}

void GameScreenLevel5TwoPlayer::UpdateKrab(float deltaTime, SDL_Event e)
{
	if (!mKrab.empty())
	{
		int enemyIndexToDelete = -1;
		for (unsigned int i = 0; i < mKrab.size(); i++)
		{
			//Code to wrap enemies around screen
			//Top two pipes
			if (((mKrab[i]->GetPosition().x < 50) && (mKrab[i]->GetPosition().y < 50)) || ((mKrab[i]->GetPosition().x > 420) && (mKrab[i]->GetPosition().y < 50)))
			{
				mKrab[i]->SwapFacing();
			}
			//bottom two pipes - kill when reach bottom two pipes
			else if (((mKrab[i]->GetPosition().x < 50) && (mKrab[i]->GetPosition().y > 320)) || ((mKrab[i]->GetPosition().x > 420) && (mKrab[i]->GetPosition().y > 320)))
			{
				mKrab[i]->RespawnAtTop();
			}
			//middle area
			else if ((((mKrab[i]->GetPosition().x < -20) && (22 <= (mKrab[i]->GetPosition().y) <= 340))) || ((mKrab[i]->GetPosition().x > 480) && (22 <= (mKrab[i]->GetPosition().y) <= 340)))
			{
				mKrab[i]->SwapFacing();
			}

			//now do the update

			mKrab[i]->Update(deltaTime, e);

			//check to see if enemy collides with player
			if ((mKrab[i]->GetPosition().y > 300.0f || mKrab[i]->GetPosition().y <= 64.0f) && (mKrab[i]->GetPosition().x < 64.0f || mKrab[i]->GetPosition().x > SCREEN_WIDTH - 96.0f))
			{
				//ignore collsions if behind pipe
			}
			else
			{
				if (Collisions::Instance()->Circle(mKrab[i], myCharacter))
				{
					if (mKrab[i]->GetInjured())
					{
						mKrab[i]->SetAlive(false);
					}
					else
					{
						myCharacter->Death();
						GameScreenManager::Instance->marioLives--;
						playerOneDeath = true;
						UpdateLives();
					}
				}
				else if (Collisions::Instance()->Circle(mKrab[i], myCharacter2))
				{
					if (mKrab[i]->GetInjured())
					{
						mKrab[i]->SetAlive(false);
					}
					else
					{
						myCharacter2->Death();
						GameScreenManager::Instance->luigiLives--;
						playerTwoDeath = true;
						UpdateLives();
					}
				}
			}


			//Inter-krab collision checks
			if (mCurrentKrabNum == 2)
			{
				if (Collisions::Instance()->Circle(mKrab[0], mKrab[1]))
				{
					mKrab[0]->SwapFacing();
					mKrab[1]->SwapFacing();
					mKrab[0]->Jump();

				}
			}
			else if (mCurrentKrabNum == 3)
			{
				if (Collisions::Instance()->Circle(mKrab[0], mKrab[1]))
				{
					mKrab[0]->SwapFacing();
					mKrab[1]->SwapFacing();
					mKrab[0]->Jump();

				}
				else if (Collisions::Instance()->Circle(mKrab[0], mKrab[2]))
				{
					mKrab[0]->SwapFacing();
					mKrab[2]->SwapFacing();
					mKrab[0]->Jump();

				}
				else if (Collisions::Instance()->Circle(mKrab[1], mKrab[2]))
				{
					mKrab[1]->SwapFacing();
					mKrab[2]->SwapFacing();
					mKrab[1]->Jump();

				}
			}
			else if (mCurrentKrabNum == 4)
			{
				if (Collisions::Instance()->Circle(mKrab[0], mKrab[1]))
				{
					mKrab[0]->SwapFacing();
					mKrab[1]->SwapFacing();
					mKrab[0]->Jump();

				}
				else if (Collisions::Instance()->Circle(mKrab[0], mKrab[2]))
				{
					mKrab[0]->SwapFacing();
					mKrab[2]->SwapFacing();
					mKrab[0]->Jump();

				}
				else if (Collisions::Instance()->Circle(mKrab[1], mKrab[2]))
				{
					mKrab[1]->SwapFacing();
					mKrab[2]->SwapFacing();
					mKrab[1]->Jump();

				}
				else if (Collisions::Instance()->Circle(mKrab[0], mKrab[3]))
				{
					mKrab[0]->SwapFacing();
					mKrab[3]->SwapFacing();
					mKrab[0]->Jump();

				}
				else if (Collisions::Instance()->Circle(mKrab[1], mKrab[3]))
				{
					mKrab[1]->SwapFacing();
					mKrab[3]->SwapFacing();
					mKrab[1]->Jump();

				}
				else if (Collisions::Instance()->Circle(mKrab[2], mKrab[3]))
				{
					mKrab[2]->SwapFacing();
					mKrab[3]->SwapFacing();
					mKrab[2]->Jump();

				}
			}

			//if the enemy is no longer alive then schedule it for deletion
			if (!mKrab[i]->GetAlive())
			{
				enemyIndexToDelete = i;
			}
		}

		//remove dead enemies -1 each update

		if (enemyIndexToDelete != -1)
		{
			mKrab.erase(mKrab.begin() + enemyIndexToDelete);
			mCurrentKrabNum--;
			krabKilled++;
			GameScreenManager::Instance->score = GameScreenManager::Instance->score + 800;
			GameScreenManager::Instance->UpdateScore();
			if (spawnSwapCounter)
			{
				CreateCoin(Vector2D(60, 32), FACING_LEFT, COIN_SPEED);
				spawnSwapCounter = false;
			}
			else if (!spawnSwapCounter)
			{
				CreateCoin(Vector2D(420, 32), FACING_RIGHT, COIN_SPEED);
				spawnSwapCounter = true;
			}
		}
	}
}

void GameScreenLevel5TwoPlayer::CreateKrab(Vector2D position, FACING direction, float speed)
{
	CharacterKrab* krabCharacter = new CharacterKrab(mRenderer, "Images/MrKrabs.png", mLevelMap, position, direction, speed);
	mKrab.push_back(krabCharacter);
	mCurrentKrabNum++;
	mKrabSpawnNum++;
	gSoundEffect->EnemySound();
}

void GameScreenLevel5TwoPlayer::UpdateCoins(float deltaTime, SDL_Event e)
{
	if (!mCoins.empty())
	{
		int enemyIndexToDelete = -1;
		for (unsigned int i = 0; i < mCoins.size(); i++)
		{
			//Code to wrap enemies around screen
			//Top two pipes
			if (((mCoins[i]->GetPosition().x < 50) && (mCoins[i]->GetPosition().y < 50)) || ((mCoins[i]->GetPosition().x > 420) && (mCoins[i]->GetPosition().y < 50)))
			{
				mCoins[i]->SwapFacing();
			}
			//bottom two pipes - kill when reach bottom two pipes
			else if (((mCoins[i]->GetPosition().x < 50) && (mCoins[i]->GetPosition().y > 320)) || ((mCoins[i]->GetPosition().x > 420) && (mCoins[i]->GetPosition().y > 320)))
			{
				mCoins[i]->SetAlive(false);
			}
			//middle area
			else if (((mCoins[i]->GetPosition().x < -20) && (22 <= (mCoins[i]->GetPosition().y) <= 340)) || ((mCoins[i]->GetPosition().x > 480) && (22 <= (mCoins[i]->GetPosition().y) <= 340)))
			{
				mCoins[i]->SwapFacing();
			}

			//now do the update

			mCoins[i]->Update(deltaTime, e);

			//check to see if enemy collides with player
			if ((mCoins[i]->GetPosition().y > 300.0f || mCoins[i]->GetPosition().y <= 64.0f) && (mCoins[i]->GetPosition().x < 64.0f || mCoins[i]->GetPosition().x > SCREEN_WIDTH - 96.0f))
			{
				//ignore collsions if behind pipe
			}
			else
			{
				if (Collisions::Instance()->Circle(mCoins[i], myCharacter))
				{
					mCoins[i]->SetAlive(false);
					gSoundEffect->CoinSound();
				}
				else if (Collisions::Instance()->Circle(mCoins[i], myCharacter2))
				{
					mCoins[i]->SetAlive(false);
					gSoundEffect->CoinSound();
				}
			}

			//Coin Collisions with krabs and koopas
			if (mCurrentCoinNum == 1)
			{
				for (unsigned int i = 0; i < mKoopa.size(); i++)
				{
					if (Collisions::Instance()->Circle(mCoins[0], mKoopa[i]))
					{
						mCoins[0]->SwapFacing();
					}
				}
				for (unsigned int i = 0; i < mKrab.size(); i++)
				{
					if (Collisions::Instance()->Circle(mCoins[0], mKrab[i]))
					{
						mCoins[0]->SwapFacing();
					}
				}
			}
			else if (mCurrentCoinNum == 2)
			{
				for (unsigned int i = 0; i < mKoopa.size(); i++)
				{
					if (Collisions::Instance()->Circle(mCoins[0], mKoopa[i]))
					{
						mCoins[0]->SwapFacing();
					}
					else if (Collisions::Instance()->Circle(mCoins[1], mKoopa[i]))
					{
						mCoins[1]->SwapFacing();
					}
				}
				for (unsigned int i = 0; i < mKrab.size(); i++)
				{
					if (Collisions::Instance()->Circle(mCoins[0], mKrab[i]))
					{
						mCoins[0]->SwapFacing();
					}
					else if (Collisions::Instance()->Circle(mCoins[1], mKrab[i]))
					{
						mCoins[1]->SwapFacing();
					}
				}
			}
			else if (mCurrentCoinNum == 3)
			{
				for (unsigned int i = 0; i < mKoopa.size(); i++)
				{
					if (Collisions::Instance()->Circle(mCoins[0], mKoopa[i]))
					{
						mCoins[0]->SwapFacing();
					}
					else if (Collisions::Instance()->Circle(mCoins[1], mKoopa[i]))
					{
						mCoins[1]->SwapFacing();
					}
					else if (Collisions::Instance()->Circle(mCoins[2], mKoopa[i]))
					{
						mCoins[2]->SwapFacing();
					}
				}
				for (unsigned int i = 0; i < mKrab.size(); i++)
				{
					if (Collisions::Instance()->Circle(mCoins[0], mKrab[i]))
					{
						mCoins[0]->SwapFacing();
					}
					else if (Collisions::Instance()->Circle(mCoins[1], mKrab[i]))
					{
						mCoins[1]->SwapFacing();
					}
					else if (Collisions::Instance()->Circle(mCoins[2], mKrab[i]))
					{
						mCoins[2]->SwapFacing();
					}
				}
			}
			else if (mCurrentCoinNum == 4)
			{
				for (unsigned int i = 0; i < mKoopa.size(); i++)
				{
					if (Collisions::Instance()->Circle(mCoins[0], mKoopa[i]))
					{
						mCoins[0]->SwapFacing();
					}
					else if (Collisions::Instance()->Circle(mCoins[1], mKoopa[i]))
					{
						mCoins[1]->SwapFacing();
					}
					else if (Collisions::Instance()->Circle(mCoins[2], mKoopa[i]))
					{
						mCoins[2]->SwapFacing();
					}
					else if (Collisions::Instance()->Circle(mCoins[3], mKoopa[i]))
					{
						mCoins[3]->SwapFacing();
					}
				}
				for (unsigned int i = 0; i < mKrab.size(); i++)
				{
					if (Collisions::Instance()->Circle(mCoins[0], mKrab[i]))
					{
						mCoins[0]->SwapFacing();
					}
					else if (Collisions::Instance()->Circle(mCoins[1], mKrab[i]))
					{
						mCoins[1]->SwapFacing();
					}
					else if (Collisions::Instance()->Circle(mCoins[2], mKrab[i]))
					{
						mCoins[2]->SwapFacing();
					}
					else if (Collisions::Instance()->Circle(mCoins[3], mKrab[i]))
					{
						mCoins[3]->SwapFacing();
					}
				}
			}
			else if (mCurrentCoinNum == 5)
			{
				for (unsigned int i = 0; i < mKoopa.size(); i++)
				{
					if (Collisions::Instance()->Circle(mCoins[0], mKoopa[i]))
					{
						mCoins[0]->SwapFacing();
					}
					else if (Collisions::Instance()->Circle(mCoins[1], mKoopa[i]))
					{
						mCoins[1]->SwapFacing();
					}
					else if (Collisions::Instance()->Circle(mCoins[2], mKoopa[i]))
					{
						mCoins[2]->SwapFacing();
					}
					else if (Collisions::Instance()->Circle(mCoins[3], mKoopa[i]))
					{
						mCoins[3]->SwapFacing();
					}
					else if (Collisions::Instance()->Circle(mCoins[4], mKoopa[i]))
					{
						mCoins[4]->SwapFacing();
					}
				}
				for (unsigned int i = 0; i < mKrab.size(); i++)
				{
					if (Collisions::Instance()->Circle(mCoins[0], mKrab[i]))
					{
						mCoins[0]->SwapFacing();
					}
					else if (Collisions::Instance()->Circle(mCoins[1], mKrab[i]))
					{
						mCoins[1]->SwapFacing();
					}
					else if (Collisions::Instance()->Circle(mCoins[2], mKrab[i]))
					{
						mCoins[2]->SwapFacing();
					}
					else if (Collisions::Instance()->Circle(mCoins[3], mKrab[i]))
					{
						mCoins[3]->SwapFacing();
					}
					else if (Collisions::Instance()->Circle(mCoins[4], mKrab[i]))
					{
						mCoins[4]->SwapFacing();
					}
				}
			}
			else if (mCurrentCoinNum == 6)
			{
				for (unsigned int i = 0; i < mKoopa.size(); i++)
				{
					if (Collisions::Instance()->Circle(mCoins[0], mKoopa[i]))
					{
						mCoins[0]->SwapFacing();
					}
					else if (Collisions::Instance()->Circle(mCoins[1], mKoopa[i]))
					{
						mCoins[1]->SwapFacing();
					}
					else if (Collisions::Instance()->Circle(mCoins[2], mKoopa[i]))
					{
						mCoins[2]->SwapFacing();
					}
					else if (Collisions::Instance()->Circle(mCoins[3], mKoopa[i]))
					{
						mCoins[3]->SwapFacing();
					}
					else if (Collisions::Instance()->Circle(mCoins[4], mKoopa[i]))
					{
						mCoins[4]->SwapFacing();
					}
					else if (Collisions::Instance()->Circle(mCoins[5], mKoopa[i]))
					{
						mCoins[5]->SwapFacing();
					}
				}
				for (unsigned int i = 0; i < mKrab.size(); i++)
				{
					if (Collisions::Instance()->Circle(mCoins[0], mKrab[i]))
					{
						mCoins[0]->SwapFacing();
					}
					else if (Collisions::Instance()->Circle(mCoins[1], mKrab[i]))
					{
						mCoins[1]->SwapFacing();
					}
					else if (Collisions::Instance()->Circle(mCoins[2], mKrab[i]))
					{
						mCoins[2]->SwapFacing();
					}
					else if (Collisions::Instance()->Circle(mCoins[3], mKrab[i]))
					{
						mCoins[3]->SwapFacing();
					}
					else if (Collisions::Instance()->Circle(mCoins[4], mKrab[i]))
					{
						mCoins[4]->SwapFacing();
					}
					else if (Collisions::Instance()->Circle(mCoins[5], mKrab[i]))
					{
						mCoins[5]->SwapFacing();
					}
				}
			}

			//if the enemy is no longer alive then schedule it for deletion
			if (!mCoins[i]->GetAlive())
			{
				enemyIndexToDelete = i;
			}
		}

		//remove dead enemies -1 each update

		if (enemyIndexToDelete != -1)
		{
			mCoins.erase(mCoins.begin() + enemyIndexToDelete);
			mCurrentCoinNum--;
			GameScreenManager::Instance->score = GameScreenManager::Instance->score + 800;
			GameScreenManager::Instance->UpdateScore();
		}
	}
}

void GameScreenLevel5TwoPlayer::CreateCoin(Vector2D position, FACING direction, float speed)
{
	Coin* Coins = new Coin(mRenderer, "Images/Coin.png", mLevelMap, position, direction, speed);
	mCoins.push_back(Coins);
	mCurrentCoinNum++;
}

void GameScreenLevel5TwoPlayer::CreateMarioLives(Vector2D position)
{
	MarioLives* Lives = new MarioLives(mRenderer, "Images/MarioLives.png", mLevelMap, position);
	mMarioLives.push_back(Lives);
}

void GameScreenLevel5TwoPlayer::CreateLuigiLives(Vector2D position)
{
	LuigiLives* Lives2 = new LuigiLives(mRenderer, "Images/LuigiLives.png", mLevelMap, position);
	mLuigiLives.push_back(Lives2);
}

void GameScreenLevel5TwoPlayer::UpdateLives()
{
	if (playerOneDeath == true)
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
	else if (playerTwoDeath == true)
	{
		//Check Luigi Lives
		if (GameScreenManager::Instance->luigiLives == 2)
		{
			mLuigiLives.erase(mLuigiLives.begin() + 2);
		}
		else if (GameScreenManager::Instance->luigiLives == 1)
		{
			mLuigiLives.erase(mLuigiLives.begin() + 1);
		}
		else if (GameScreenManager::Instance->luigiLives == 0)
		{
			mLuigiLives.erase(mLuigiLives.begin());
		}
	}
	playerOneDeath = false;
	playerTwoDeath = false;
}

void GameScreenLevel5TwoPlayer::UpdatePowBlock()
{
	//Update powblock for both Mario and Luigi upon collisions
	if (Collisions::Instance()->Box(mPowBlock->GetCollisionBox(), myCharacter->GetCollisionBox()))
	{
		if (mPowBlock->IsAvailable())
		{
			if (myCharacter->IsJumping())
			{
				DoScreenshake();
				mPowBlock->TakeAHit();
				myCharacter->CancelJump();
			}
		}
	}
	else if (Collisions::Instance()->Box(mPowBlock->GetCollisionBox(), myCharacter2->GetCollisionBox()))
	{
		if (mPowBlock->IsAvailable())
		{
			if (myCharacter2->IsJumping())
			{
				DoScreenshake();
				mPowBlock->TakeAHit();
				myCharacter2->CancelJump();
			}
		}
	}
}



void GameScreenLevel5TwoPlayer::CountdownRespawn()
{
	if (mKoopaSpawnNum < KOOPA_NUM_LEVEL5)
	{
		CreateKoopa(Vector2D(60, 32), FACING_RIGHT, KOOPA_SPEEP);
		CreateKoopa(Vector2D(420, 32), FACING_LEFT, KOOPA_SPEEP);
	}
	else if (mKrabSpawnNum < KRAB_NUM_LEVEL5)
	{
		if (spawnSwapCounter)
		{
			CreateKrab(Vector2D(60, 32), FACING_RIGHT, KOOPA_SPEEP);
			spawnSwapCounter = false;
		}
		else if (!spawnSwapCounter)
		{
			CreateKrab(Vector2D(420, 32), FACING_LEFT, KOOPA_SPEEP);
			spawnSwapCounter = true;
		}
	}

	mCountdownTime = ENEMYSPAWNCOUNTDOWN;
	mCountdown = true;
}

void GameScreenLevel5TwoPlayer::DoScreenshake()
{
	mScreenshake = true;
	mScreenshakeTime = SCREENSHAKE_DURATION;
	mWobble = 0.0f;

	for (int i = 0; i < mKoopa.size(); i++)
	{
		mKoopa[i]->TakeDamage();
	}

	for (int i = 0; i < mKrab.size(); i++)
	{
		mKrab[i]->TakeDamage();
	}
}