#pragma once
#include <SDL.h>
#include <SDL_mixer.h>
#include "Commons.h"
#include <string>

using namespace std;

class SoundEffect
{
public:
	SoundEffect(Mix_Chunk* gEffect);
	~SoundEffect();

	void FreeSound();
	void CoinSound();
	void JumpSound();
	void EnemySound();
	void DeathSound();

private:
	Mix_Chunk* gSoundEffect;
};