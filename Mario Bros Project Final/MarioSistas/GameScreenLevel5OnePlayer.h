#pragma once
#include <SDL.h>
#include <vector>
#include <string>
#include "Commons.h"
#include "GameScreen.h"
#include "GameScreenManager.h"
#include "LevelMap.h"
#include "Collisions.h"
#include "PowBlock.h"
#include "Character.h"
#include "CharacterMario.h"
#include "CharacterLuigi.h"
#include "MarioLives.h"
#include "CharacterKoopa.h"
#include "CharacterKrab.h"
#include "Coin.h"

class Character;
class Texture2D;
class PowBlock;

class GameScreenLevel5OnePlayer : GameScreen
{
private:
	Texture2D* mBackgroundTexture;
	CharacterMario* myCharacter;
	LevelMap* mLevelMap;
	PowBlock* mPowBlock;
	SoundEffect* gSoundEffect;

	bool SetUpLevel();
	void SetLevelMap();

	bool mScreenshake;
	float mScreenshakeTime;
	void DoScreenshake();
	float mWobble;
	float mBackgroundYPos;

	void UpdateKoopa(float deltaTime, SDL_Event e);
	void CreateKoopa(Vector2D position, FACING direction, float speed);
	vector<CharacterKoopa*> mKoopa;
	int mKoopaSpawnNum;
	int mCurrentKoopaNum;

	void UpdateKrab(float deltaTime, SDL_Event e);
	void CreateKrab(Vector2D position, FACING direction, float speed);
	vector<CharacterKrab*> mKrab;
	int mKrabSpawnNum;
	int mCurrentKrabNum;

	void UpdateCoins(float deltaTime, SDL_Event e);
	void CreateCoin(Vector2D position, FACING direction, float speed);
	vector<Coin*> mCoins;
	int mCurrentCoinNum;

	void CreateMarioLives(Vector2D position);
	vector<MarioLives*> mMarioLives;
	void UpdateLives();

	bool spawnSwapCounter;

	int koopaKilled;
	int krabKilled;

public:
	GameScreenLevel5OnePlayer(SDL_Renderer* renderer);
	~GameScreenLevel5OnePlayer();

	void Render();
	void Update(float deltaTime, SDL_Event e);
	void UpdatePowBlock();

	bool mCountdown;
	float mCountdownTime;
	void CountdownRespawn();
};
