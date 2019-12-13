
#include "Lib.hpp"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <direct.h>
#include <functional>
#include <Box2D/Box2D.h>
#include "JeuTank.hpp"
#include "Entity.hpp"
#include "Entity.hpp"
#include <SFML/Graphics.hpp>

using namespace sf;

static Vector2f mousePos[4] = {
	Vector2f(0,0),
	Vector2f(100,100),
	Vector2f(200,200),
	Vector2f(300,800)
};

float lerp(float a, float b, float r) {
	return a + (b - a) * r;
}

float rd() {
	return 1.0 * rand() / RAND_MAX;
}

//// hue: 0-360°; sat: 0.f-1.f; val: 0.f-1.f 
sf::Color hsv(int hue, float sat, float val)
{
	hue %= 360;
	while (hue < 0) hue += 360;

	if (sat < 0.f) sat = 0.f;
	if (sat > 1.f) sat = 1.f;

	if (val < 0.f) val = 0.f;
	if (val > 1.f) val = 1.f;

	int h = hue / 60;
	float f = float(hue) / 60 - h;
	float p = val * (1.f - sat);
	float q = val * (1.f - sat * f);
	float t = val * (1.f - sat * (1 - f));

	switch (h)
	{
	default:
	case 0:
	case 6: return sf::Color(val * 255, t * 255, p * 255);
	case 1: return sf::Color(q * 255, val * 255, p * 255);
	case 2: return sf::Color(p * 255, val * 255, t * 255);
	case 3: return sf::Color(p * 255, q * 255, val * 255);
	case 4: return sf::Color(t * 255, p * 255, val * 255);
	case 5: return sf::Color(val * 255, p * 255, q * 255);
	}
}

void drawCurve(sf::RenderWindow &win, float now) {
	sf::VertexArray va(sf::LineStrip);
	sf::Color red = sf::Color::Red;
	sf::Color blue = sf::Color::Blue;
	int nb = 320;
	float stride = 1280.0 / nb;

	int ofsX = 0;
	if (fmodf(now, 2.0) <= 1) {
		red.a = 0;
	}

	for (int i = 0; i < nb + 1; ++i) {
		double ratio = 1.0 * i / nb;
		double x = ofsX + stride * i;
		double y = 400;

		sf::Color c = hsv(ratio * 360, 0.8, 0.8);

		sf::Vertex vertex(Vector2f(x, y), c);
		va.append(vertex);
	}
	win.draw(va);
}

static void drawBox2D(sf::RenderWindow &win)
{
	b2Vec2 vertices[4];
	vertices[0].Set(0.0f, 0.0f);
	vertices[1].Set(1.0f, 0.0f);
	vertices[2].Set(0.0f, 1.0f);
	vertices[3].Set(1.0f, 1.0f);
	int32 count = 4;
	b2PolygonShape LimitsPolygon;
	LimitsPolygon.Set(vertices, count);
}

static RectangleShape	* sh = nullptr;
static Vector2f			shPos;
static Vector2f			shDir;

static RectangleShape	* shTarget = nullptr;

static int shSize = 48;

static void initMovingSquare() {
	sh = new RectangleShape(Vector2f(shSize, shSize));
	sh->setPosition(shPos.x = 100, shPos.y = 400);
	sh->setOrigin(shSize >> 1, shSize >> 1);
	sh->setFillColor(Color(0x48D2EBff));
	sh->setOutlineThickness(2);
	sh->setOutlineColor(Color(0x3FEB53ff));

	shTarget = new RectangleShape(Vector2f(shSize *1.5, 4));
	shTarget->setOrigin(0, 2);
	shTarget->setFillColor(Color(0xB34139ff));
}

static void drawMovingSquare(sf::RenderWindow& win) {
	win.draw(*sh);

	Vector2i mpos = sf::Mouse::getPosition(win);
	shTarget->setPosition(sh->getPosition());
	float angle = atan2f(mpos.y - sh->getPosition().y, mpos.x - sh->getPosition().x) / (2 * 3.141569) * 360;
	shTarget->setRotation(angle);

	shDir.x = mpos.x - sh->getPosition().x;
	shDir.y = mpos.y - sh->getPosition().y;

	win.draw(*shTarget);
}

bool Game::willCollide(Entity * end, int cx, int cy)
{
	int cScreenWidth = 1280;
	int cScreenHeight = 720;

	if (cx < 0) return true;
	else if (cx >= cScreenWidth) return true;

	if (cy < 0) return true;
	else if (cy >= cScreenHeight) return true;

	for (Vector2i & cell : platforms)
		if (cell.x == cx && cell.y == cy)
			return true;

	return false;
}

static Vector2f p0;
static Vector2f p1;

static RectangleShape shp;

static RectangleShape walls[4];

int main()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 2;


	sf::RenderWindow window(sf::VideoMode(1280, 720), "SFML works!", sf::Style::Default, settings);
	window.setVerticalSyncEnabled(true);

	/*sf::CircleShape shape(100.f, (int)(2 * 3.141569 * 100));
	shape.setPosition(30, 30);
	shape.setFillColor(sf::Color(0xE884D4ff));
	shape.setOutlineThickness(4);
	shape.setOutlineColor(sf::Color(0xFF8A70ff));*/

	sf::Clock clock;

	sf::Time appStart = clock.getElapsedTime();
	sf::Time frameStart = clock.getElapsedTime();
	sf::Time prevFrameStart = clock.getElapsedTime();

	float fps[4] = { 0.f,0.f,0.f,0.f };
	int step = 0;
	sf::Font * font = new sf::Font();
	if (font->loadFromFile("res/Orbitron-Bold.ttf") == false) {
		printf("no such font\n");
	}
	double len = Lib::v2Len(p1 - p0);
	sf::RectangleShape sh(Vector2f(1, len));
	sh.setFillColor(sf::Color(255, 0, 127, 255));
	sh.setOrigin(0.5, 0.0);
	sh.setPosition(p0.x, p0.y);
	double angle = atan2(p1.y - p0.y, p1.x - p0.x);
	angle -= Lib::PI * 0.5;
	sh.setRotation(angle / (2.0 * 3.14156) * 360.0);

	Vector2f speed = p1 - p0;
	b2Vec2 inter;
	b2Vec2 normal;
	if (Lib::willCollide(p0, speed, &shp, inter, normal)) {
		auto dotSize = 8;
		sf::RectangleShape lp;
		lp.setOrigin(dotSize*0.5, dotSize*0.5);
		lp.setSize(Vector2f(dotSize, dotSize));
		lp.setFillColor(sf::Color(0, 255, 0, 255));
		lp.setPosition(inter.x, inter.y);
		window.draw(lp);
		{
			sf::VertexArray line(sf::LineStrip);
			line.append(sf::Vertex(Vector2f(inter.x, inter.y), sf::Color(255, 0, 0, 255)));
			line.append(sf::Vertex(Vector2f(inter.x + normal.x * 30, inter.y + normal.y * 30), sf::Color(255, 0, 0, 255)));
			window.draw(line);
		}
		b2Vec2 startToInter = inter - b2Vec2(p0.x, p0.y);
		b2Vec2 refl = startToInter - 2 * Lib::dot(startToInter, normal) * normal;
		b2Vec2 endRefl = inter + refl;
		{
			sf::VertexArray line(sf::LineStrip);
			line.append(sf::Vertex(Vector2f(inter.x, inter.y), sf::Color(255, 255, 0, 255)));
			line.append(sf::Vertex(Vector2f(endRefl.x, endRefl.y), sf::Color(255, 255, 0, 255)));
			window.draw(line);
		}
	}

	std::vector< Particle * > vec;

	initMovingSquare();

	sf::Text myFpsCounter;
	int every = 0;
	while (window.isOpen())//on passe tout le temps DEBUT DE LA FRAME 
	{
		//collision();
		sf::Event event;//recup les evenement clavier/pad
		frameStart = clock.getElapsedTime();
		while (window.pollEvent(event)) {
			switch (event.type) {
			case sf::Event::KeyReleased:
				if (event.key.code == sf::Keyboard::I)
					printf("instant fps %f\n", fps[(step - 1) % 4]);
				if (event.key.code == sf::Keyboard::F)
					printf("fps %f\n", 0.25f*(fps[0] + fps[1] + fps[2] + fps[3]));
				break;

				/*case sf::Event::KeyPressed:
				{
					if (event.key.code == sf::Keyboard::F1) mousePos[0] = sf::Vector2f(sf::Mouse::getPosition(window));
					if (event.key.code == sf::Keyboard::F2) mousePos[1] = sf::Vector2f(sf::Mouse::getPosition(window));
					if (event.key.code == sf::Keyboard::F3) mousePos[2] = sf::Vector2f(sf::Mouse::getPosition(window));
					if (event.key.code == sf::Keyboard::F4) mousePos[3] = sf::Vector2f(sf::Mouse::getPosition(window));
				}*/
				break;

			case sf::Event::Closed:
				window.close();
				break;

			default:
				break;
			}
		}

		const int squareSpeed = 5;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
			shPos.x -= squareSpeed;
			shDir.x = -1;
			shDir.y = 0;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			shPos.x += squareSpeed;
			shDir.x = 1;
			shDir.y = 0;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
			shPos.y -= squareSpeed;
			shDir.x = 0;
			shDir.y = -1;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			shPos.y += squareSpeed;
			shDir.x = 0;
			shDir.y = 1;
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			if (shDir.x == 0 && shDir.y == 0) shDir.y = 1;

			RectangleShape * pr = new RectangleShape(Vector2f(8, 8));
			pr->setFillColor(Color::Blue);
			pr->setOrigin(4, 4);
			//pr->setPosition(sh->getPosition());
			Particle * p = new Particle(pr);

			float shDirLen = sqrt(shDir.x*shDir.x + shDir.y*shDir.y);
			p->dir.x = shDir.x / shDirLen;
			p->dir.y = shDir.y / shDirLen;
			p->bhv = [](Particle * part) {
				Vector2f ppos = part->spr->getPosition();
				ppos.x += part->dir.x * 6;
				ppos.y += part->dir.y * 6;
				part->spr->setPosition(ppos);
			};
			vec.push_back(p);
		}
	//	sh->setPosition(shPos);


		myFpsCounter.setPosition(8, 8);
		myFpsCounter.setFillColor(sf::Color::Red);
		myFpsCounter.setFont(*font);

		if (every == 0) {
			myFpsCounter.setString(std::string("FPS:") + std::to_string(fps[(step - 1) % 4]));
			every = 30;
		}
		every--;

		window.clear(sf::Color(0x9EFFC6ff));//nettoie la frame

		drawMovingSquare(window);

		//drawCurve(window, clock.getElapsedTime().asSeconds() );
		//drawCatmull(window, clock.getElapsedTime().asSeconds());

		for (int i = 0; i < 4; ++i) {
			window.draw(walls[i]);
		}

		//window.draw(shape);//on demande le dessin d' une forme
		window.draw(myFpsCounter);

		for (int k = 0; k < (int)vec.size(); k++) {
			Particle * p = vec[vec.size() - k - 1];
			p->update();
			if (p->killed) {
				vec.erase(vec.begin() + k);
			}
			else {
				p->draw(window);
			}
		}

		window.display();//ca dessine et ca attend la vsync

		fps[step % 4] = 1.0f / (frameStart - prevFrameStart).asSeconds();
		prevFrameStart = frameStart;

		step++;

		double winWidth = window.getSize().x;
		double winHeight = window.getSize().y;

		walls[0].setFillColor(sf::Color::Yellow);
		walls[0].setPosition(-15, 0);
		walls[0].setSize(Vector2f(16, winHeight));

		walls[1].setFillColor(sf::Color::Yellow);
		walls[1].setPosition(winWidth - 1, 0);
		walls[1].setSize(Vector2f(16, winHeight));

		walls[2].setFillColor(sf::Color::Yellow);
		walls[2].setPosition(0, -15);
		walls[2].setSize(Vector2f(winWidth, 16));

		walls[3].setFillColor(sf::Color::Yellow);
		walls[3].setPosition(0, winHeight - 1);
		walls[3].setSize(Vector2f(winWidth, 16));

		drawBox2D(window);
	}

	/*if (sf::Joystick::isConnected(0))
	{
		float x = sf::Joystick::getAxisPosition(0, sf::Joystick::X);
		printf("axe x trouvé");
		float y = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);
		printf("axe y trouvé");
		sh->move(x, y);
		printf("bouge");
	}*/

	return 0;
}

