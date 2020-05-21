#include "SoundEffect.h"
#include <SDL_mixer.h>
#include <iostream>

SoundEffect::SoundEffect(Mix_Chunk* gEffect)
{
	gSoundEffect = gEffect;
	gSoundEffect = NULL;
}

SoundEffect::~SoundEffect()
{
	FreeSound();
	gSoundEffect = NULL;
	Mix_Quit();
}

void SoundEffect::FreeSound()
{
	if (gSoundEffect != NULL)
	{
		Mix_FreeChunk(gSoundEffect);
		gSoundEffect = NULL;
	}
}

void SoundEffect::CoinSound()
{
	Mix_Chunk* fx = Mix_LoadWAV("Music/mb_coin.wav");
	if (fx == NULL)
	{
		cout << "Coin no soundy" << endl;
	}
	else
	{
		Mix_PlayChannel(-1, fx, 0);
	}
}

void SoundEffect::JumpSound()
{
	Mix_Chunk* fx = Mix_LoadWAV("Music/mb_jump.wav");
	if (fx == NULL)
	{
		cout << "Jump no soundy" << endl;
	}
	else
	{
		Mix_PlayChannel(-1, fx, 0);
	}
}

void SoundEffect::EnemySound()
{
	Mix_Chunk* fx = Mix_LoadWAV("Music/mb_sc.wav");
	if (fx == NULL)
	{
		cout << "Enemy no soundy" << endl;
	}
	else
	{
		Mix_PlayChannel(-1, fx, 0);
	}
}

void SoundEffect::DeathSound()
{
	Mix_Chunk* fx = Mix_LoadWAV("Music/mb_die.wav");
	if (fx == NULL)
	{
		cout << "Die no soundy" << endl;
	}
	else
	{
		Mix_PlayChannel(-1, fx, 0);
	}
}


