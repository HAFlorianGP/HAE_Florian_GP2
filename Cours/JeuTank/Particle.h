#pragma once


#include <vector>
#include <functional>
#include <SFML/Graphics.hpp>

class Particle {
public:

	sf::Shape * spr;
	Vector2f speed;

	int life = 100;
	bool killed = false;

	std::function<void(Particle*)> bhv;

	Particle(sf::Shape * spr) {
		this->spr = spr;
	}

	~Particle() {
		delete(spr);
		spr = nullptr;
	}

	virtual void update() {
		if (bhv) {
			bhv(this);
		}
		life--;
		if (life == 0) killed = true;
	}

	void draw(RenderWindow & win) {
		win.draw(*spr);
	}

	void applySpeed() {
		Vector2f pos = spr->getPosition();
		spr->setPosition(pos.x + speed.x, pos.y + speed.y);
	}
};