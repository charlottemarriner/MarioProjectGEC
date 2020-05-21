#pragma once
#include "Commons.h"


class Character;

class Collisions
{
public:
	~Collisions();
	
	static Collisions* Instance();

	bool Circle(Character* myCharacter, Character* myCharacter2);
	bool Box(Rect2D rect1, Rect2D rect2);
private:
	Collisions();

	static Collisions* mInstance;
};

