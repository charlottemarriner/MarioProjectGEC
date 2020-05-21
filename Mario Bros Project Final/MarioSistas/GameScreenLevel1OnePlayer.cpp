#include "GameScreenLevel1OnePlayer.h"
#include <iostream>
#include "Texture2D.h"
#include "Constants.h"

GameScreenLevel1OnePlayer::GameScreenLevel1OnePlayer(SDL_Renderer* renderer) : GameScreen(renderer)
{
	SetUpLevel();
}

GameScreenLevel1OnePlayer::~GameScreenLevel1OnePlayer()
{
	delete mBackgroundTexture;
	mBackgroundTexture = NULL;
	delete myCharacter;
	myCharacter = NULL;
	
	delete mPowBlock;
	mPowBlock = NULL;

	delete mLevelMap;
	mLevelMap = NULL;

	mKoopa.clear();
	mCoins.clear();
	mMarioLives.clear();
}

void GameScreenLevel1OnePlayer::Update(float deltaTime, SDL_Event e)
{
	//Check if game has been won or lost
	if (koopaKilled == 3)
	{
		GameScreenManager::Instance->ChangeScreen(SCREEN_LEVEL2_ONEPLAYER);
	}
	else if (GameScreenManager::Instance->marioLives == 0)
	{
		GameScreenManager::Instance->ChangeScreen(SCREEN_GAMEOVER);
	}


	myCharacter->Update(deltaTime, e);

	UpdatePowBlock();
	UpdateKoopa(deltaTime, e);
	UpdateCoins(deltaTime, e);

	mCountdownTime -= deltaTime;

	//Respawm enemies
	if (mCountdownTime <= 0 && mCountdown == true && mKoopaSpawnNum < KOOPA_NUM_LEVEL1)
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

void GameScreenLevel1OnePlayer::Render()
{
	//Draw the background texture
	mBackgroundTexture->Render(Vector2D(0, mBackgroundYPos), SDL_FLIP_NONE);

	/*mTextHandler->display(SCREEN_WIDTH / 2, 20, mRenderer);*/

	//Call to character render function
	myCharacter->Render();
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

	//Draw Mario Lives
	for (unsigned int i = 0; i < mMarioLives.size(); i++)
	{
		mMarioLives[i]->Render();
	}
}

bool GameScreenLevel1OnePlayer::SetUpLevel()
{
	SetLevelMap();

	//Set up the player character
	myCharacter = new CharacterMario(mRenderer, "Images/Mario.png", Vector2D(64, 330), mLevelMap);

	//Load the background texture
	mBackgroundTexture = new Texture2D(mRenderer);

	//Set up Pow Block
	mPowBlock = new PowBlock(mRenderer, mLevelMap);

	GameScreenManager::Instance->luigiLives = 3;
	GameScreenManager::Instance->marioLives = 3;
	GameScreenManager::Instance->score = 0;
	GameScreenManager::Instance->UpdateScore();

	mCurrentKoopaNum = 0;
	mCurrentCoinNum = 0;
	mScreenshake = false;
	mCountdownTime = ENEMYSPAWNCOUNTDOWN;
	mCountdown = true;
	mBackgroundYPos = 0.0f;
	koopaKilled = 0;
	mKoopaSpawnNum = 0;
	spawnSwapCounter = false;

	//Set up lives
	CreateMarioLives(Vector2D(200, 10));
	CreateMarioLives(Vector2D(220, 10));
	CreateMarioLives(Vector2D(240, 10));

	//Set up 2 bad bois
	CreateKoopa(Vector2D(60, 32), FACING_RIGHT, KOOPA_SPEEP);
	CreateKoopa(Vector2D(420, 32), FACING_LEFT, KOOPA_SPEEP);

	//Check if background texture loads
	if (!mBackgroundTexture->LoadFrom("Images/BackgroundMB.png"))
	{
		cout << "Failed to load background texture!";
		return false;
	}
	else
		return true;
}

void GameScreenLevel1OnePlayer::SetLevelMap()
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
										{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 } };;

	//Clear up any old map
	if (mLevelMap != NULL)
	{
		delete mLevelMap;
		//delete mPowBlock;
	}

	//Set up the new one
	mLevelMap = new LevelMap(map);
}

void GameScreenLevel1OnePlayer::UpdateKoopa(float deltaTime, SDL_Event e)
{
	if (!mKoopa.empty())
	{
		int enemyIndexToDelete = -1;
		for (unsigned int i = 0; i < mKoopa.size(); i++)
		{
			//Code to wrap enemies around screen
			//Top two pipes
			if (((mKoopa[i]->GetPosition().x < 50) && (mKoopa[i]->GetPosition().y < 50)) || ((mKoopa[i]->GetPosition().x > 430) && (mKoopa[i]->GetPosition().y < 50)))
			{
				mKoopa[i]->SwapFacing();
			}
			//bottom two pipes - kill when reach bottom two pipes
			else if (((mKoopa[i]->GetPosition().x < 50) && (mKoopa[i]->GetPosition().y > 320)) || ((mKoopa[i]->GetPosition().x > 430) && (mKoopa[i]->GetPosition().y > 320)))
			{
				mKoopa[i]->RespawnAtTop();
			}
			//middle area
			else if ((((mKoopa[i]->GetPosition().x < -20) && (22 <= (mKoopa[i]->GetPosition().y) <= 340))) || ((mKoopa[i]->GetPosition().x > 500) && (22 <= (mKoopa[i]->GetPosition().y) <= 340)))
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
						UpdateLives();
					}
				}
			}

			//Check collisions between Koopas and Krabs
			if (mCurrentKoopaNum == 2)
			{
				if (Collisions::Instance()->Circle(mKoopa[0], mKoopa[1]))
				{
					mKoopa[0]->SwapFacing();
					mKoopa[1]->SwapFacing();
					mKoopa[0]->Jump();

				}
			}
			else if (mCurrentKoopaNum == 3)
			{
				if (Collisions::Instance()->Circle(mKoopa[0], mKoopa[1]))
				{
					mKoopa[0]->SwapFacing();
					mKoopa[1]->SwapFacing();
					mKoopa[0]->Jump();

				}
				else if (Collisions::Instance()->Circle(mKoopa[0], mKoopa[2]))
				{
					mKoopa[0]->SwapFacing();
					mKoopa[2]->SwapFacing();
					mKoopa[0]->Jump();

				}
				else if (Collisions::Instance()->Circle(mKoopa[1], mKoopa[2]))
				{
					mKoopa[1]->SwapFacing();
					mKoopa[2]->SwapFacing();
					mKoopa[1]->Jump();

				}
			}
			else if (mCurrentKoopaNum == 4)
			{
				if (Collisions::Instance()->Circle(mKoopa[0], mKoopa[1]))
				{
					mKoopa[0]->SwapFacing();
					mKoopa[1]->SwapFacing();
					mKoopa[0]->Jump();

				}
				else if (Collisions::Instance()->Circle(mKoopa[0], mKoopa[2]))
				{
					mKoopa[0]->SwapFacing();
					mKoopa[2]->SwapFacing();
					mKoopa[0]->Jump();

				}
				else if (Collisions::Instance()->Circle(mKoopa[1], mKoopa[2]))
				{
					mKoopa[1]->SwapFacing();
					mKoopa[2]->SwapFacing();
					mKoopa[1]->Jump();

				}
				else if (Collisions::Instance()->Circle(mKoopa[0], mKoopa[3]))
				{
					mKoopa[0]->SwapFacing();
					mKoopa[3]->SwapFacing();
					mKoopa[0]->Jump();

				}
				else if (Collisions::Instance()->Circle(mKoopa[1], mKoopa[3]))
				{
					mKoopa[1]->SwapFacing();
					mKoopa[3]->SwapFacing();
					mKoopa[1]->Jump();

				}
				else if (Collisions::Instance()->Circle(mKoopa[2], mKoopa[3]))
				{
					mKoopa[2]->SwapFacing();
					mKoopa[3]->SwapFacing();
					mKoopa[2]->Jump();

				}
			}

			//if the enemy is no longer alive then schedule it for deletion
			if (!mKoopa[i]->GetAlive())
			{
				enemyIndexToDelete = i;
				if (koopaKilled < 3 && mKoopaSpawnNum < 3)
				{
					if (spawnSwapCounter)
					{
						CreateKoopa(Vector2D(60, 32), FACING_RIGHT, KOOPA_SPEEP);
						spawnSwapCounter = false;
					}
					else if (!spawnSwapCounter)
					{
						CreateKoopa(Vector2D(420, 32), FACING_LEFT, KOOPA_SPEEP);
						spawnSwapCounter = true;
					}
				}
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

void GameScreenLevel1OnePlayer::CreateKoopa(Vector2D position, FACING direction, float speed)
{
	CharacterKoopa* koopaCharacter = new CharacterKoopa(mRenderer, "Images/Koopa.png", mLevelMap, position, direction, speed);
	mKoopa.push_back(koopaCharacter);
	mCurrentKoopaNum++;
	mKoopaSpawnNum++;
	gSoundEffect->EnemySound();
}

void GameScreenLevel1OnePlayer::UpdateCoins(float deltaTime, SDL_Event e)
{
	if (!mCoins.empty())
	{
		int enemyIndexToDelete = -1;
		for (unsigned int i = 0; i < mCoins.size(); i++)
		{
			//Code to wrap enemies around screen
			//Top two pipes
			if (((mCoins[i]->GetPosition().x < 50) && (mCoins[i]->GetPosition().y < 50)) || ((mCoins[i]->GetPosition().x > 430) && (mCoins[i]->GetPosition().y < 50)))
			{
				mCoins[i]->SwapFacing();
			}
			//bottom two pipes - kill when reach bottom two pipes
			else if (((mCoins[i]->GetPosition().x < 50) && (mCoins[i]->GetPosition().y > 320)) || ((mCoins[i]->GetPosition().x > 430) && (mCoins[i]->GetPosition().y > 320)))
			{
				mCoins[i]->SetAlive(false);
			}
			//middle area
			else if (((mCoins[i]->GetPosition().x < -20) && (22 <= (mCoins[i]->GetPosition().y) <= 340)) || ((mCoins[i]->GetPosition().x > 500) && (22 <= (mCoins[i]->GetPosition().y) <= 340)))
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

			//Coin Collisions
			if (mCurrentCoinNum == 1)
			{
				for (unsigned int i = 0; i < mKoopa.size(); i++)
				{
					if (Collisions::Instance()->Circle(mCoins[0], mKoopa[i]))
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

void GameScreenLevel1OnePlayer::CreateCoin(Vector2D position, FACING direction, float speed)
{
	Coin* Coins = new Coin(mRenderer, "Images/Coin.png", mLevelMap, position, direction, speed);
	mCoins.push_back(Coins);
	mCurrentCoinNum++;
}

void GameScreenLevel1OnePlayer::UpdateLives()
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

void GameScreenLevel1OnePlayer::CreateMarioLives(Vector2D position)
{
	MarioLives* Lives = new MarioLives(mRenderer, "Images/MarioLives.png", mLevelMap, position);
	mMarioLives.push_back(Lives);
}


void GameScreenLevel1OnePlayer::UpdatePowBlock()
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

//void GameScreenLevel1OnePlayer::UpdateScore()
//{
//	Rect2D* scoreRect = new Rect2D(SCREEN_WIDTH / 2, 0, 90, 70);
//	string stringText = std::to_string(score);
//
//	mTextHandler = new Text(mRenderer, "Images/OCRAEXT.ttf", 70, stringText, { 255,255,255,0 }, scoreRect);
//}

void GameScreenLevel1OnePlayer::CountdownRespawn()
{
	if (mKoopaSpawnNum < 2)
	{
		CreateKoopa(Vector2D(60, 32), FACING_RIGHT, KOOPA_SPEEP);
		CreateKoopa(Vector2D(420, 32), FACING_LEFT, KOOPA_SPEEP);
	}
	else if (spawnSwapCounter)
	{
		CreateKoopa(Vector2D(60, 32), FACING_RIGHT, KOOPA_SPEEP);
		spawnSwapCounter = false;
	}
	else if (!spawnSwapCounter)
	{
		CreateKoopa(Vector2D(420, 32), FACING_LEFT, KOOPA_SPEEP);
		spawnSwapCounter = true;
	}

	mCountdownTime = ENEMYSPAWNCOUNTDOWN;
	mCountdown = true;
}

void GameScreenLevel1OnePlayer::DoScreenshake()
{
	mScreenshake = true;
	mScreenshakeTime = SCREENSHAKE_DURATION;
	mWobble = 0.0f;

	for (int i = 0; i < mKoopa.size(); i++)
	{
		mKoopa[i]->TakeDamage();
	}
}
