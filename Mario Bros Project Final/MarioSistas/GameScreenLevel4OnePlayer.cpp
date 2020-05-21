#include "GameScreenLevel4OnePlayer.h"
#include <iostream>
#include "Texture2D.h"
#include "Constants.h"

GameScreenLevel4OnePlayer::GameScreenLevel4OnePlayer(SDL_Renderer* renderer) : GameScreen(renderer)
{
	SetUpLevel();
}

GameScreenLevel4OnePlayer::~GameScreenLevel4OnePlayer()
{
	delete mBackgroundTexture;
	mBackgroundTexture = NULL;
	delete myCharacter;
	myCharacter = NULL;

	delete mPowBlock;
	mPowBlock = NULL;

	delete mLevelMap;
	mLevelMap = NULL;

	mCoins.clear();
	mKrab.clear();
	mMarioLives.clear();
}

void GameScreenLevel4OnePlayer::Update(float deltaTime, SDL_Event e)
{
	//Check if game has been won or lost
	if (krabKilled == 4)
	{
		GameScreenManager::Instance->ChangeScreen(SCREEN_LEVEL5_ONEPLAYER);
	}
	else if (GameScreenManager::Instance->marioLives == 0)
	{
		GameScreenManager::Instance->ChangeScreen(SCREEN_GAMEOVER);
	}


	myCharacter->Update(deltaTime, e);

	UpdatePowBlock();
	UpdateKrab(deltaTime, e);
	UpdateCoins(deltaTime, e);

	mCountdownTime -= deltaTime;

	//Respawm enemies
	if (mCountdownTime <= 0 && mCountdown == true && mKrabSpawnNum < KRAB_NUM_LEVEL4)
	{
		mCountdown = false;
		CountdownRespawn();
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
}

void GameScreenLevel4OnePlayer::Render()
{
	//Draw the background texture
	mBackgroundTexture->Render(Vector2D(0, mBackgroundYPos), SDL_FLIP_NONE);

	//Call to character render function
	myCharacter->Render();
	mPowBlock->Render();

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

	//Draw Mario Lives
	for (unsigned int i = 0; i < mMarioLives.size(); i++)
	{
		mMarioLives[i]->Render();
	}
}

bool GameScreenLevel4OnePlayer::SetUpLevel()
{
	SetLevelMap();

	//Set up the player character
	myCharacter = new CharacterMario(mRenderer, "Images/Mario.png", Vector2D(64, 330), mLevelMap);

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
	krabKilled = 0;
	mKrabSpawnNum = 0;
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

	//Set up 2 bad Mr Krabbies
	CreateKrab(Vector2D(60, 32), FACING_RIGHT, KOOPA_SPEEP);
	CreateKrab(Vector2D(420, 32), FACING_LEFT, KOOPA_SPEEP);

	//Check if background texture loads
	if (!mBackgroundTexture->LoadFrom("Images/Background4.png"))
	{
		cout << "Failed to load background texture!";
		return false;
	}
	else
		return true;
}

void GameScreenLevel4OnePlayer::SetLevelMap()
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

void GameScreenLevel4OnePlayer::UpdateKrab(float deltaTime, SDL_Event e)
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

void GameScreenLevel4OnePlayer::CreateKrab(Vector2D position, FACING direction, float speed)
{
	CharacterKrab* krabCharacter = new CharacterKrab(mRenderer, "Images/MrKrabs.png", mLevelMap, position, direction, speed);
	mKrab.push_back(krabCharacter);
	mCurrentKrabNum++;
	mKrabSpawnNum++;
	gSoundEffect->EnemySound();
}

void GameScreenLevel4OnePlayer::UpdateCoins(float deltaTime, SDL_Event e)
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
			}

			//Coin Collisions with krabs
			if (mCurrentCoinNum == 1)
			{
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

void GameScreenLevel4OnePlayer::CreateCoin(Vector2D position, FACING direction, float speed)
{
	Coin* Coins = new Coin(mRenderer, "Images/Coin.png", mLevelMap, position, direction, speed);
	mCoins.push_back(Coins);
	mCurrentCoinNum++;
}

void GameScreenLevel4OnePlayer::CreateMarioLives(Vector2D position)
{
	MarioLives* Lives = new MarioLives(mRenderer, "Images/MarioLives.png", mLevelMap, position);
	mMarioLives.push_back(Lives);
}

void GameScreenLevel4OnePlayer::UpdateLives()
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


void GameScreenLevel4OnePlayer::UpdatePowBlock()
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
}



void GameScreenLevel4OnePlayer::CountdownRespawn()
{
	if (mKrabSpawnNum < KRAB_NUM_LEVEL4)
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

void GameScreenLevel4OnePlayer::DoScreenshake()
{
	mScreenshake = true;
	mScreenshakeTime = SCREENSHAKE_DURATION;
	mWobble = 0.0f;

	for (int i = 0; i < mKrab.size(); i++)
	{
		mKrab[i]->TakeDamage();
	}
}

