#pragma once

struct Vector2D
{
	float x;
	float y;

	Vector2D()
	{
		x = 0.0f;
		y = 0.0f;
	}

	Vector2D(float xx, float yy)
	{
		x = xx;
		y = yy;
	}
};

enum SCREENS
{
	SCREEN_MENU = 0,
	SCREEN_LEVEL1_ONEPLAYER,
	SCREEN_LEVEL1_TWOPLAYER,
	SCREEN_LEVEL2_ONEPLAYER,
	SCREEN_LEVEL2_TWOPLAYER,
	SCREEN_LEVEL3_ONEPLAYER,
	SCREEN_LEVEL3_TWOPLAYER,
	SCREEN_LEVEL4_ONEPLAYER,
	SCREEN_LEVEL4_TWOPLAYER,
	SCREEN_LEVEL5_ONEPLAYER,
	SCREEN_LEVEL5_TWOPLAYER,
	SCREEN_GAMEOVER,
	SCREEN_HIGHSCORES
};

enum FACING
{
	FACING_LEFT = 0,
	FACING_RIGHT
};

struct Rect2D
{
	float x;
	float y;
	float width;
	float height;

	Rect2D(float ix, float iy, float width, float height)
	{
		x = ix;
		y = iy;
		this->width = width;
		this->height = height;
	}
};

/*
vector2D example = Vector2D(34.4f, 12.5f);

alternativly

Vector2D example;
example.x = 23.32f;
example.y = 6543.32f;
*/