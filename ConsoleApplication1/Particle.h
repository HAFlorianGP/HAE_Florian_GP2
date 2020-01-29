#pragma once
#include <math.h>
#include "Lib.hpp"
#include <SFML/Graphics.hpp>

class Explosion {

public:
	int Timer;
	int MaxTimer;
	CircleShape ExplosionShape;
	Vector2f PositionExplosion;
	bool BoolGun;
	
	bool operator == (const Explosion& s) const {
		return (PositionExplosion == s.PositionExplosion && Timer == s.Timer); 
	}
	bool operator != (const Explosion& s) const {
		return !operator==(s);
	}

	Explosion(Vector2f pos, sf::Texture * texture, int size, bool gun) {
		PositionExplosion = pos;
		
		ExplosionShape.setRadius(rand() % ((size + 2) - (size - 2) + 1) + (size - 2));
		
		ExplosionShape.setTexture(texture);

			ExplosionShape.setRotation(rand() % ((180) - (-180) + 1) + (-180));
			ExplosionShape.setOrigin(ExplosionShape.getRadius() , ExplosionShape.getRadius() );
			ExplosionShape.setPosition(pos);
			IntRect Rect(Vector2i(512/4, 0), Vector2i(125, 125));
			
			ExplosionShape.setTextureRect(Rect);

		MaxTimer = rand() % (6*size - 4*size + 1) + 4*size;
		Timer = MaxTimer;
		BoolGun = gun;
	}

	void UpdateParticle() {

		if (BoolGun) {
			if (Timer == MaxTimer - 1)
			{
				IntRect Rect(Vector2i(512 / 2, 0), Vector2i(125, 125));

				ExplosionShape.setTextureRect(Rect);
			}

			else if (Timer == MaxTimer - 2)
			{
				IntRect Rect(Vector2i(0, 512 / 4), Vector2i(125, 125));

				ExplosionShape.setTextureRect(Rect);
			}

			else if (Timer == MaxTimer - 3)
			{
				IntRect Rect(Vector2i(512 / 2, 512 / 4), Vector2i(125, 125));

				ExplosionShape.setTextureRect(Rect);
			}

			else if (Timer == MaxTimer - 4)
			{
				IntRect Rect(Vector2i(0, 512 / 2), Vector2i(125, 125));

				ExplosionShape.setTextureRect(Rect);
			}

			else if (Timer == MaxTimer - 5)
			{
				IntRect Rect(Vector2i(512 / 2, 512 / 2), Vector2i(125, 125));

				ExplosionShape.setTextureRect(Rect);
			}

			else if (Timer == MaxTimer - 6)
			{
				IntRect Rect(Vector2i(0, (512 / 2) + (512 / 4)), Vector2i(125, 125));

				ExplosionShape.setTextureRect(Rect);
			}

			else if (Timer == MaxTimer - 7)
			{
				IntRect Rect(Vector2i(512 / 2, (512 / 2) + (512 / 4)), Vector2i(125, 125));

				ExplosionShape.setTextureRect(Rect);
			}
		}

		else 
		{
			if (Timer == MaxTimer - 2)
			{
				IntRect Rect(Vector2i(512 / 2, 0), Vector2i(125, 125));

				ExplosionShape.setTextureRect(Rect);
			}
			else if (Timer == MaxTimer - 4)
			{
				IntRect Rect(Vector2i((512 / 4) + (512 / 2), 0), Vector2i(125, 125));

				ExplosionShape.setTextureRect(Rect);
			}

			else if (Timer == MaxTimer - 6)
			{
				IntRect Rect(Vector2i(0, 512 / 4), Vector2i(125, 125));

				ExplosionShape.setTextureRect(Rect);
			}

			else if (Timer == MaxTimer - 8)
			{
				IntRect Rect(Vector2i(512 / 4, 512 / 4), Vector2i(125, 125));

				ExplosionShape.setTextureRect(Rect);
			}

			else if (Timer == MaxTimer - 10)
			{
				IntRect Rect(Vector2i(512 / 2, 512 / 4), Vector2i(125, 125));

				ExplosionShape.setTextureRect(Rect);
			}

			else if (Timer == MaxTimer - 12)
			{
				IntRect Rect(Vector2i((512 / 2) + (512 / 4), 512 / 4), Vector2i(125, 125));

				ExplosionShape.setTextureRect(Rect);
			}

			else if (Timer == MaxTimer - 14)
			{
				IntRect Rect(Vector2i(0, 512 / 2), Vector2i(125, 125));

				ExplosionShape.setTextureRect(Rect);
			}

			else if (Timer == MaxTimer - 16)
			{
				IntRect Rect(Vector2i(512 / 4, 512 / 2), Vector2i(125, 125));

				ExplosionShape.setTextureRect(Rect);
			}

			else if (Timer == MaxTimer - 18)
			{
				IntRect Rect(Vector2i(512 / 2, 512 / 2), Vector2i(125, 125));

				ExplosionShape.setTextureRect(Rect);
			}

			else if (Timer == MaxTimer - 20)
			{
				IntRect Rect(Vector2i((512 / 2) + (512 / 4), 512 / 2), Vector2i(125, 125));

				ExplosionShape.setTextureRect(Rect);
			}

			else if (Timer == MaxTimer - 22)
			{
				IntRect Rect(Vector2i(0, (512 / 2) + (512 / 4)), Vector2i(125, 125));

				ExplosionShape.setTextureRect(Rect);
			}

			else if (Timer == MaxTimer - 24)
			{
				IntRect Rect(Vector2i(512 / 4, (512 / 2) + (512 / 4)), Vector2i(125, 125));

				ExplosionShape.setTextureRect(Rect);
			}

			else if (Timer == MaxTimer - 26)
			{
				IntRect Rect(Vector2i(512 / 2, (512 / 2) + (512 / 4)), Vector2i(125, 125));

				ExplosionShape.setTextureRect(Rect);
			}

			else if (Timer == MaxTimer - 28)
			{
				IntRect Rect(Vector2i((512 / 2) + (512 / 4), (512 / 2) + (512 / 4)), Vector2i(125, 125));

				ExplosionShape.setTextureRect(Rect);
			}
		}
		Timer -= 1;
	}
};