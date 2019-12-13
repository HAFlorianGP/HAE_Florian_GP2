#include "Game.hpp"
#include "Entity.hpp"
#include <SFML/Graphics.hpp>

Game* Game::me = nullptr;

static bool wasPressed[sf::Keyboard::KeyCount];

void Game::Init()
{
	RectangleShape *sh = new RectangleShape(Vector2f(12, 24));
	sh->setFillColor(sf::Color::Cyan);
	sh->setOrigin(6, 24);

	Entity * ent = new Entity(sh);
	ent->setPosPixel(100, 500);
	evec.push_back(ent);
	player = ent;
	me = this;

	memset(wasPressed, 0, sizeof(wasPressed));

	makePlatforms();
}

void Game::makePlatforms() {
	int cScreenWidth = 1280 / Entity::CELL_WIDTH;
	int cScreenHeight = 720 / Entity::CELL_WIDTH;


	platforms.push_back(Vector2i(8, cScreenHeight - 1));
	platforms.push_back(Vector2i(8, cScreenHeight - 2));
}

void Game::Controls() {
	auto lat_acc = 0.065;
	auto max_lat_speed = 0.65;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		player->dx += lat_acc;
		if (player->dx > max_lat_speed) player->dx = max_lat_speed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
		player->dx -= lat_acc;
		if (player->dx < -max_lat_speed) player->dx = -max_lat_speed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		player->dy += lat_acc;
		if (player->dy > max_lat_speed) player->dy = max_lat_speed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
		player->dy -= lat_acc;
		if (player->dy < -max_lat_speed) player->dy = -max_lat_speed;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		player->setPosPixel(100, 500);
		player->dx = player->dy = 0.0f;
		player->changeState(ES_IDLE);
	}
	wasPressed[sf::Keyboard::Up] = sf::Keyboard::isKeyPressed(sf::Keyboard::Z);


	float dx = player->dx;
	float dy = player->dy;
	float speedLen = sqrt(dx*dx + dy * dy);
	if (abs(dx) == 0 && abs(dy) == 0) {
		if (player->getState() != ES_IDLE && player->getState() != ES_COVER)
			player->changeState(ES_IDLE);
	}
	else if (speedLen <= max_lat_speed * 0.5)
		player->changeState(ES_WALKING);
	else
		player->changeState(ES_RUNNING);
}


void Game::update(double dt) {
	Controls();
	for (auto it = evec.begin(); it != evec.end();) {
		Entity * ent = *it;
		ent->update(dt);
		it++;
	}
}

void Game::draw(RenderWindow & win)
{
	for (auto it = evec.begin(); it != evec.end();) {
		Entity * ent = *it;
		ent->draw(win);
		it++;
	}

	auto CW = Entity::CELL_WIDTH;
	RectangleShape Wall;
	Wall.setFillColor(sf::Color::Red);
	Wall.setSize(Vector2f(CW, CW));
	for (Vector2i & pl : platforms) {
		Wall.setPosition(pl.x * CW, pl.y * CW);
		win.draw(Wall);
	}

	for (auto it = pvec.begin(); it != pvec.end();) {
		Particle * p = *it;
		p->update();
		if (p->killed) {
			it = pvec.erase(it);
		}
		else {
			p->draw(win);
			it++;
		}
	}
	/*RectangleShape line(Vector2f(1000, 1));
	RectangleShape line2(Vector2f(250, 1));
	RectangleShape line3(Vector2f(250, 1));
	line.setFillColor(sf::Color::Red);
	line2.setFillColor(sf::Color::Green);
	line3.setFillColor(sf::Color::Green);
	line.setPosition(100, 500);
	line2.setPosition(200, 600);
	line3.setPosition(800, 600);
	win.draw(line);
	win.draw(line2);
	win.draw(line3);*/  
	//Lignes imaginaires
}

bool Game::willCollide(Entity * end, int cx, int cy)
{
	int cScreenWidth = 1280 / Entity::CELL_WIDTH;
	int cScreenHeight = 720 / Entity::CELL_WIDTH;

	if (cx < 0) return true;
	else if (cx >= cScreenWidth) return true;

	if (cy < 0) return true;
	else if (cy >= cScreenHeight) return true;

	for (Vector2i & Wall : platforms)
		if (Wall.x == cx && Wall.y == cy)
			return true;
	return false;
}

void Game::togglePlatform(int cx, int cy)
{
	for (auto it = platforms.begin(); it != platforms.end(); it++) {
		Vector2i & pl = *it;
		if (pl.x == cx && pl.y == cy) {
			platforms.erase(it);
			return;
		}
	}
	platforms.push_back(Vector2i(cx, cy));
}