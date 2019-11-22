
#include "Lib.hpp"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <direct.h>
#include <functional>
#include <Box2D/Box2D.h>

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

		//y += (ratio*ratio*1.33)* sin( ratio * now * 8.0) * 256 + rd() * 8;

		//x = 500 + cos(ratio * 2 * 3.141569) * (now * 20);
		//y = 500 + sin(ratio * 2 * 3.141569) * ( now * 20);

		//x = 500 + cos(ratio * 2 * 3.141569) * ( now * now * 20 * (0.5 + rd() * 0.5));
		//y = 500 + sin(ratio * 2 * 3.141569) * (now * now * 20 * (0.5 + rd() * 0.5));

		//y += sin(now) * 200;
		//y += ratio * ratio * sin(ratio * 8.0 + now) * 256;

		//y += sin(ratio * 8.0 + now * 1.5) * 120;

		//y += sin(ratio * 8.0 + now) * (128 * 1.0 + cos(now*16) * 4);

		/*int radius = 160;
		x = 500 + radius * cos( ratio * 2 * 3.141569) + sin(now*10 * cos(ratio)) * 8 * (1.0 + rd() * 100);
		y = 500 + radius * sin( ratio * 2 * 3.141569) + cos(now * 10 * sin(ratio)) * 10 * (1.0 + rd() * 100);*/
		/*sf::Color c = sf::Color(lerp(blue.r, red.r,ratio), lerp(blue.g, red.g, ratio), lerp(blue.b, red.b, ratio));
		//sf::Color c = i % 2 ? red : blue;*/

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

/*void drawCatmull(sf::RenderWindow &win, float now) {
	sf::VertexArray va(sf::LineStrip);
	sf::Color red = sf::Color::Red;
	sf::Color blue = sf::Color::Blue;
	int nb = 320;
	float stride = 1280.0 / nb;
	std::vector<Vector2f> points;


	points.push_back(Vector2f(0,0));
	points.push_back(Vector2f(80, 150));
	points.push_back(Vector2f(600, 300));
	points.push_back(Vector2f(100, 600));
	points.push_back(Vector2f(1280, 720));

	for (int i = 0; i < 4; i++) {
		points.push_back(mousePos[i]);
	}

	sf::CircleShape shape(16, (int)(2 * 3.141569 * 100));
	shape.setOrigin(Vector2f(16, 16));
	shape.setPosition(0, 0);
	shape.setFillColor(sf::Color(0xE884D4ff));

	for (int i = 0; i < nb + 1; ++i) {
		double ratio = 1.0 * i / nb;
		double x = 0.0;
		double y = 0.0;
		sf::Color c = hsv(ratio * 360, 0.8, 0.8);

		Vector2f pos = Lib::plot2(ratio, points);
		x = pos.x;
		y = pos.y;

		sf::Vertex vertex(Vector2f(x, y), c);
		va.append(vertex);
	}

	static float cRatio = 0.0;
	static bool autoreverse = false;
	Vector2f pos = Lib::plot2(autoreverse ? cRatio : (1 - cRatio), points);
	shape.setPosition(pos);

	cRatio += 0.001f;
	if (cRatio > 1.0) {
		cRatio = 0.0;
		autoreverse = !autoreverse;
	}

	win.draw(va);
	win.draw(shape);
}*/

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

class Particle {
public:

	sf::Shape * spr;
	Vector2f dir;

	int life = 3000;
	bool killed = false;

	std::function<void(Particle*)> bhv;

	Particle(sf::Shape * spr) {
		this->spr = spr;
		dir.y = 1;
	}

	~Particle() {
		delete(spr);
		spr = nullptr;
	}

	void update() {
		if (bhv) {
			bhv(this);
		}
		life--;
		if (life == 0) killed = true;
	}

	void draw(RenderWindow & win) {
		win.draw(*spr);
	}
};

/*static void collision()
	{
		sf::FloatRect boundingBox = sh.getGlobalBounds();
		if (boundingBox.intersects(LimitsShape)) {
			printf("hit the wall");
		}
	}*/

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
			pr->setFillColor(Color(0xEB3922ff));
			pr->setOrigin(4, 4);
			pr->setPosition(sh->getPosition());
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
		sh->setPosition(shPos);


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

		{
			int nb = 8;
			for (int i = 0; i < nb; ++i) {
				Shape* sh = new sf::CircleShape(16);
				sh->setOrigin(16, 16);
				//auto angle = Lib::rd() * 3.14156;
				auto angle = 1.0 * i / nb * 3.14156 * 2;
				float dx = cos(angle) * 180.0f;
				float dy = sin(angle) * 180.0f;
				Particle * p = new Particle(sh);
				p->life = 1000000;

				sh->setPosition(winWidth*0.5 + dx, winHeight * 0.5 + dy);
				sh->setFillColor(sf::Color(200, 0, 0, 255));

				p->bhv = [](Particle* p) {

				};
				vec.push_back(p);
			}
		}

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

