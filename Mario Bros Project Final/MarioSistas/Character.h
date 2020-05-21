#pragma once
#include <SDL.h>
#include <string>
#include <iostream>
#include "Commons.h"
#include "Texture2D.h"
#include "LevelMap.h"
#include "SoundEffect.h"

using namespace std;

class Texture2D;
class SoundEffect;

class Character
{
protected:
	SDL_Renderer* mRenderer;
	Vector2D mPosition;
	Texture2D* mTexture;
	SoundEffect* gSoundEffect;
	

	virtual void MoveLeft(float deltaTime);
	virtual void MoveRight(float deltaTime);

	bool mJumping;
	bool mCanJump;
	float mJumpForce;
	void Jump();
	virtual void AddGravity(float deltaTime);
	float mCollisionRadius;

	FACING mFacingDirection;
	LevelMap* mCurrentLevelMap;

	bool mMovingLeft;
	bool mMovingRight;

	bool alive;
public:
	Character(SDL_Renderer* renderer, string imagePath, Vector2D startPosition, LevelMap* map);
	~Character();

	virtual void Render();
	virtual void Update(float deltaTime, SDL_Event e);

	void SetPosition(Vector2D newPosition);
	Vector2D GetPosition();

	/*void AddGravity(float deltaTime);*/

	float GetCollisionRadius();
	bool IsJumping() { return mJumping; }
	void CancelJump() { mJumping = false; }

	Rect2D GetCollisionBox();

	//Alive states
	void SetAlive(bool isAlive);
	bool GetAlive() { return alive; };
private:
	/*LevelMap* mCurrentLevelMap;*/
	SoundEffect* mEffect;
};