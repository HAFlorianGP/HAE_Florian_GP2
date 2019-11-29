// App.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//


#include "pch.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <direct.h>
#include <functional>
#include "Lib.hpp"

using namespace sf;

class Turtle : public sf::ConvexShape {
public:
	sf::Transform m_Trs;

	Turtle() : sf::ConvexShape(3) {
		setFillColor(sf::Color(0x73BEFFff));
		setOutlineThickness(2);
		setOutlineColor(sf::Color(0xEB2420ff, 0xEB2420ff, 0xEB2420ff));

		setPoint(0, Vector2f(0, -24));
		setPoint(1, Vector2f(-16, 16));
		setPoint(2, Vector2f(16, 16));
	}

	void setTransform(sf::Transform trs) {
		m_Trs = trs;
	}

};

static sf::Shader * simpleShader = nullptr;

static sf::Texture * whiteTex = nullptr;
static sf::Texture * gradientMountain = nullptr;

static sf::Transform				s_Initial;
static std::vector<sf::Transform>	s_Trs;
static std::vector<Turtle*>			s_Turtles;

static void startTransforms() {
	s_Initial = Transform::Identity;
	s_Initial.translate(500, 500);
}

static void translateX(float dx) {
	sf::Transform res;
	res.translate(dx, 0);
	s_Trs.push_back(res);
}

static void translateY(float dy) {
	sf::Transform res;
	res.translate(0, dy);
	s_Trs.push_back(res);
}

static void rotate(float degrees) {
	sf::Transform res;
	res.rotate(degrees);
	s_Trs.push_back(res);
}

static void scaleXY(float dx, float dy) {
	sf::Transform res;
	res.scale(dx, dy);
	s_Trs.push_back(res);
}

static void scaleXY(float dxy) {
	sf::Transform res;
	res.scale(dxy, dxy);
	s_Trs.push_back(res);
}

static void scaleX(float dx) {
	sf::Transform res;
	res.scale(dx, 0);
	s_Trs.push_back(res);
}

static void scaleY(float dy) {
	sf::Transform res;
	res.scale(0, dy);
	s_Trs.push_back(res);
}


static void computeTransform(sf::Transform & result, int step = -1) {
	sf::Transform inter;

	inter.combine(s_Initial);

	if (step <= -1) {
		for (sf::Transform t : s_Trs)
			inter = inter.combine(t);
	}
	else {
		step--;
		for (sf::Transform t : s_Trs) {
			inter = inter.combine(t);
			if (step <= 0)
				break;
		}
	}
	result = inter;
}


static void plotTurtle() {
	sf::Transform cur;
	computeTransform(cur, s_Trs.size() - 1);
	Turtle* t = new Turtle();
	t->setTransform(cur);
	s_Turtles.push_back(t);
}

/*static void shift_left() {
	rotate(180);
	translateY(len);
	rotate(180);
}

static void shift_right() {
	rotate(180);
	translateY(len);
	rotate(180);
}

static void koch_n(int n, float len){
	if (n == 0)
	{
		
	}
}*/

bool startsWith(const char * s0, const char * s1) {
	if (*s0 == 0 && *s1 != 0)
		return false;
	if (*s1 == 0)
		return true;
	if (*s0 != *s1)
		return false;
	else
		return startsWith(s0 + 1, s1 + 1);
}

enum TurtleCommand {
	AV, REC, GRO, RET, L45, R45,
};

static std::vector<TurtleCommand> cmd;
static char data[1024];
static void readScript() {
	FILE * f = nullptr;
	auto err = fopen_s(&f, "res/scriptTortue.txt", "r");
	if (f != nullptr) {
		memset(data, 0, 1024);
		fread(data, 2014, 1, f);
		fclose(f);

		char * cur = data;
		bool doContinue = true;
		float currentFrame = 0;

		while (doContinue)
		{
			if (*cur == 0) break;
			else {
				if (startsWith(cur, "AV")) {
					cmd.push_back(AV);
					currentFrame++;
				}
				else if (currentFrame == 100) {
					cmd.push_back(REC);
					currentFrame++;
					break;
				}
				else if (currentFrame == 200) {
					cmd.push_back(GRO);
					currentFrame++;
					break;
				}
				else if (currentFrame == 300) {
					cmd.push_back(RET);
					currentFrame++;
					break;
				}
				else if (currentFrame == 400) {
					cmd.push_back(L45);
					currentFrame++;
					break;
				}

				else if (currentFrame += 500) {
					cmd.push_back(R45);
					currentFrame++;
					break;
				}
				cur = strstr(cur, " ");
			}			
		}
		
		if (*cur == 21) {
				printf("terminer");
				return;
		}
	}

	else {
		printf("cannot read file %s", "res/scriptTortue.txt", err);
	}
}

int main() {

	sf::ContextSettings settings;
	settings.antialiasingLevel = 2;

	sf::RenderWindow window(sf::VideoMode(1280, 720), "SFML works!", sf::Style::Default, settings);
	window.setVerticalSyncEnabled(true);

	sf::View initialView = window.getDefaultView();
	sf::View mainView = window.getDefaultView();
	window.setView(mainView);

	sf::Clock clock;

	readScript();

	sf::Time appStart = clock.getElapsedTime();
	sf::Time frameStart = clock.getElapsedTime();
	sf::Time prevFrameStart = clock.getElapsedTime();

	float fps[4] = { 0.f,0.f,0.f,0.f };
	int step = 0;
	sf::Font * font = new sf::Font();
	if (font->loadFromFile("res/Orbitron-Bold.ttf") == false) {
		printf("no such font\n");
	}

	if (!sf::Shader::isAvailable())
		printf("no shader avail\n");

	simpleShader = new Shader();
	if (!simpleShader->loadFromFile("res/simple.vert", "res/simple.frag"))
		printf("unable to load shaders\n");

	whiteTex = new Texture();
	if (!whiteTex->create(1, 1)) printf("tex crea failed\n");
	whiteTex->setSmooth(true);
	unsigned int col = 0xffffffff;
	whiteTex->update((const sf::Uint8*)&col, 1, 1, 0, 0);
	
	sf::Text myFpsCounter;
	int every = 0;

	sf::Clock deltaClock;

	double winWidth = window.getSize().x;
	double winHeight = window.getSize().y;

	int showSegment = 0;

	startTransforms();
	plotTurtle();

	while (window.isOpen())//on passe tout le temps DEBUT DE LA FRAME 
	{
		sf::Event event;//recup les evenement clavier/pad
		frameStart = clock.getElapsedTime();
		window.setView(initialView);

		window.clear();
		while (window.pollEvent(event)) {


			Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(window));

			switch (event.type) {

			case sf::Event::Resized:
				initialView.setSize({
								   static_cast<float>(event.size.width),
								   static_cast<float>(event.size.height)
					});
				window.setView(initialView);
				break;

			case sf::Event::KeyReleased: {

				if (event.key.code == sf::Keyboard::Space) {
					startTransforms();

					sf::Transform trs;
					computeTransform(trs);

					for (Turtle * t : s_Turtles) delete t;
					s_Turtles.clear();

					Turtle* t = new Turtle();
					t->setTransform(trs);
					s_Turtles.push_back(t);
				}

				auto delta = 32;

				if (event.key.code == sf::Keyboard::Up) { translateY(-delta);	   plotTurtle(); }
				if (event.key.code == sf::Keyboard::Down) { translateY(delta);	   plotTurtle(); }

				if (event.key.code == sf::Keyboard::Left) { rotate(-45);		   plotTurtle(); }
				if (event.key.code == sf::Keyboard::Right) { rotate(45);		   plotTurtle(); }

				if (event.key.code == sf::Keyboard::Add) { scaleXY(2.0);		   plotTurtle(); }
				if (event.key.code == sf::Keyboard::Subtract) { scaleXY(0.5);	   plotTurtle(); }

				if (event.key.code == sf::Keyboard::F1) {
					for (TurtleCommand tc : cmd){
						switch (tc) {
						default:
							break;
						case AV: translateY(-delta);
							plotTurtle();
							break;
						case REC: translateY(delta);
							plotTurtle();
							break;
						case GRO: scaleXY(2.0);
							plotTurtle();
							break;
						case RET: scaleXY(0.5);
							plotTurtle();
							break;
						case L45: rotate(-45);
							plotTurtle();
							break;
						case R45: rotate(45);
							break;
						}
						plotTurtle();
					}
				}

				break;
			}

			case sf::Event::KeyPressed:

				break;

			case sf::Event::Closed:
				window.close();
				break;

			default:
				break;
			}
		}
		
		sf::Time dt = deltaClock.restart();

		myFpsCounter.setPosition(8, 8);
		myFpsCounter.setFillColor(sf::Color::Red);
		myFpsCounter.setFont(*font);

		if (every == 0) {
			myFpsCounter.setString(std::string("FPS:") + std::to_string(fps[(step - 1) % 4]));
			every = 30;
		}
		every--;

		window.draw(myFpsCounter);

		for (Turtle * t : s_Turtles) {
			RenderStates rs;
			rs.transform = t->m_Trs;
			window.draw(*t, rs);
		}
		window.display();//ca dessine et ca attend la vsync

		fps[step % 4] = 1.0f / (frameStart - prevFrameStart).asSeconds();
		prevFrameStart = frameStart;

		step++;
	}
	return 0;
}