#pragma once
#include <math.h>
#include "Lib.hpp"
#include "Collider.hpp"
#include <SFML/Graphics.hpp>

class Bullet {

public:
	Vector2f Direction;
	int timer = 0;
	int Life;
	Vector2f Position;
	CircleShape BulletShape;
	SphereCollider BulletCollision = SphereCollider(Vector2f(0,0),1);

	bool operator == (const Bullet& s) const { return Position == s.Position; }
	bool operator != (const Bullet& s) const { return !operator==(s); }

	Bullet(Vector2f Coordonnées, Vector2f Forces, sf::Color color) 
	{
		Life = 2;
		BulletCollision.Update(Coordonnées, 10);
		BulletShape.setRadius(10);

		BulletShape.setPosition(Coordonnées);

		Position = Coordonnées;
		Direction = Vector2f(Forces.x/10, Forces.y/10);
	}

	~Bullet() 
	{
		printf("Tirez!");
	}

	void updatecollider() 
	{
		Position = BulletShape.getPosition();
		BulletCollision.Update(BulletShape.getPosition(), 10);
	}
};
