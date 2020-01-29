#pragma once
#include <math.h>
#include "Lib.hpp"

class SphereCollider {

public:
	
	Vector2i Collider1;
	Vector2i Collider2;
	Vector2i Collider3;
	Vector2i Collider4;
	Vector2i Collider5;
	Vector2i Collider6;
	Vector2i Collider7;
	Vector2i Collider8;

	SphereCollider(Vector2f Origine, float Rayon) {
		Collider1 = (Vector2i(Origine.x + Rayon, Origine.y));
		Collider2 = (Vector2i(Origine.x + ((sqrt(2) / 2) * Rayon), Origine.y - ((sqrt(2) / 2) * Rayon)));
		Collider3 = (Vector2i(Origine.x , Origine.y - Rayon));
		Collider4 = (Vector2i(Origine.x - ((sqrt(2) / 2) * Rayon), Origine.y - ((sqrt(2) / 2) * Rayon)));
		Collider5 = (Vector2i(Origine.x - Rayon, Origine.y));
		Collider6 = (Vector2i(Origine.x - ((sqrt(2) / 2) * Rayon), Origine.y + ((sqrt(2) / 2) * Rayon)));
		Collider7 = (Vector2i(Origine.x ,  Origine.y + Rayon));
		Collider8 = (Vector2i(Origine.x + ((sqrt(2) / 2) * Rayon), Origine.y + ((sqrt(2) / 2) * Rayon)));
	}

void Update(Vector2f Origine, float Rayon) {
	Collider1 = (Vector2i(Origine.x + Rayon, Origine.y));
	Collider2 = (Vector2i(Origine.x + ((sqrt(2) / 2) * Rayon), Origine.y - ((sqrt(2) / 2) * Rayon)));
	Collider3 = (Vector2i(Origine.x, Origine.y - Rayon));
	Collider4 = (Vector2i(Origine.x - ((sqrt(2) / 2) * Rayon), Origine.y - ((sqrt(2) / 2) * Rayon)));
	Collider5 = (Vector2i(Origine.x - Rayon, Origine.y));
	Collider6 = (Vector2i(Origine.x - ((sqrt(2) / 2) * Rayon), Origine.y + ((sqrt(2) / 2) * Rayon)));
	Collider7 = (Vector2i(Origine.x, Origine.y + Rayon));
	Collider8 = (Vector2i(Origine.x + ((sqrt(2) / 2) * Rayon), Origine.y + ((sqrt(2) / 2) * Rayon)));
	}
};

class SquareCollider {

public: 
	Rect<int> Collider;

	Vector2f PrevPos;

	SquareCollider(int Left, int Up, int Width, int Height )
	{
		Collider.left = Left;
		Collider.top = Up;
		Collider.width = Width;
		Collider.height = Height;
	}

void update(int Left, int Up, int Width, int Height)
{
	Collider.left = Left;
	Collider.top = Up;
	Collider.width = Width;
	Collider.height = Height;
}

bool CheckColliding(SquareCollider other) 
	{
	if (Collider.intersects(other.Collider)) return true;
	else return false;
	}

float GetAngleOfCollision(SphereCollider other2)
	{
		if (Collider.contains(other2.Collider1) || Collider.contains(other2.Collider5)) { return 0; }
		if (Collider.contains(other2.Collider3) || Collider.contains(other2.Collider7)) { return 1; }
	}

bool CheckCollidingWithSphere(SphereCollider other2)
	{
	if (Collider.contains(other2.Collider1) || Collider.contains(other2.Collider3) || Collider.contains(other2.Collider5) || Collider.contains(other2.Collider7)) { return true; }
	else return false;	
	}
};

