#pragma once
#include <SDL.h>
#include "Commons.h"
#include "GameScreen.h"
#include "GameScreenManager.h"
#include "Character.h"
#include "CharacterMario.h"
#include "CharacterLuigi.h"
#include "Collisions.h"
#include "LevelMap.h"
#include "PowBlock.h"
#include "BonusLevelCoins.h"
#include <vector>
#include <string>
#include "LuigiLives.h"
#include "MarioLives.h"

class Character;
class CharacterMario;
class CharacterLuigi;
class Texture2D;

class GameScreenLevel3OnePlayer : GameScreen
{
private:
	Texture2D* mBackgroundTexture;
	CharacterMario* myCharacter;
	LevelMap* mLevelMap;
	SoundEffect* gSoundEffect;

	bool SetUpLevel();
	void SetLevelMap();

	float mBackgroundYPos;

	void UpdateCoins(float deltaTime, SDL_Event e);
	void CreateCoin(Vector2D position);
	vector<BonusLevelCoins*> mBonusLevelCoins;

	void CreateMarioLives(Vector2D position);
	vector<MarioLives*> mMarioLives;
	void UpdateLives();

	bool spawnSwapCounter;

public:
	GameScreenLevel3OnePlayer(SDL_Renderer* renderer);
	~GameScreenLevel3OnePlayer();

	void Render();
	void Update(float deltaTime, SDL_Event e);

	bool mCountdown;
	float mCountdownTime;
};

