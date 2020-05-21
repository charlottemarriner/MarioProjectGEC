#include "Character.h"
#include "Collisions.h"

//Initialise the instance to NULL.
Collisions* Collisions::mInstance = NULL;

Collisions::Collisions()
{
}

Collisions::~Collisions()
{
	delete mInstance;
	mInstance = NULL;
}

Collisions* Collisions::Instance()
{
	if (!mInstance)
	{
		mInstance = new Collisions;
	}
	return mInstance;
}

bool Collisions::Circle(Character* myCharacter, Character* myCharacter2)
{
	Vector2D vec = Vector2D((myCharacter->GetPosition().x - myCharacter2->GetPosition().x),
							(myCharacter->GetPosition().y - myCharacter2->GetPosition().y));

	double distance = sqrt((vec.x * vec.x) + (vec.y * vec.y));
	double combinedDistance = (myCharacter->GetCollisionRadius() + myCharacter2->GetCollisionRadius());
	return distance < combinedDistance;
}

bool Collisions::Box(Rect2D rect1, Rect2D rect2)
{
	if (rect1.x + (rect1.width / 2) > rect2.x &&
		rect1.x + rect1.width / 2 < rect2.x + rect2.width &&
		rect1.y + (rect1.height / 2) > rect2.y &&
		rect1.y + (rect1.height / 2) < rect2.y + rect2.height)
	{
		return true;
	}
		return false;
}
