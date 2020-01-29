#include "pch.h"
#include <iostream>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System/String.hpp>
#include <sstream>
#include <SFML/System/FileInputStream.hpp>
#include <list>
#include "Lib.hpp"
#include "Collider.hpp"
#include "Proj.hpp"
#include "Particle.h"
#include <windows.h>

static sf::Shader * simpleShader = nullptr;
float timestamp2 = 0;
sf::Vector2f vec(70, 120);
sf::RectangleShape shape(vec);
float rotationP1 = -90;
float rotationP2 = -90;

bool P1IsDead = false;
bool P1Win = false;
bool P2IsDead = false;
bool P2Win = false;
sf::RectangleShape shape2(vec);
sf::RectangleShape GunP1(Vector2f(50, 100 * 1.2));
sf::RectangleShape GunP2(Vector2f(50 * 1.1, 100 * 1.4));

std::list <Bullet> Projectiles;
std::list<Explosion> ExplosionList;

bool P1CanShoot = true;
bool P2CanShoot = true;
bool Shaking = false;
bool GameIsRunning = false;
bool FlashEnabled = false;

int ScreenShaker = 0;
int TimerText = 0;
int ScoreP1 = 0;
int ScoreP2 = 0;

Vector2f Normalize(Vector2f N) {
	float length = std::sqrt(N.x * N.x + N.y * N.y); 
	N.x /= length;
	N.y /= length;
	return N;
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "Un jeu, paraît-il ...", Style::Fullscreen);
	while (window.isOpen()) {
		rotationP1 = -90;
		rotationP2 = -90;
		ScreenShaker = 0;
		timestamp2 = 0;

		P1IsDead = false;
		P2IsDead = false;
		Shaking = false;
		P1CanShoot = true;
		P2CanShoot = true;

		shape.setRotation(0);
		shape2.setRotation(0);
		GunP1.setRotation(0);
		GunP2.setRotation(0);

		shape.setFillColor(sf::Color::White);
		shape2.setFillColor(sf::Color::White);
		GunP1.setFillColor(sf::Color::White);
		GunP2.setFillColor(sf::Color::White);

		sf::RectangleShape BackgroundShape(Vector2f(1920,1080));

		sf::Texture Background;
		if (!Background.loadFromFile("Background.jpg")) {
			printf("Unable To Load Background Texture\n");
		}

		sf::Texture MiddleMap;
		if (!MiddleMap.loadFromFile("MiddleMap.png")) { 
			printf("Unable To Load MiddleMap Texture\n"); 
		}
		BackgroundShape.setTexture(&Background);

		sf::Texture ShootFX;
		if (!ShootFX.loadFromFile("SmallBombs.png")) { 
			printf("Unable To Load Bombs Texture\n"); 
		}

		sf::Texture ExplosionFX;
		if (!ExplosionFX.loadFromFile("Explosion.png")) {
			printf("Unable To Load Explosion Texture\n"); 
		}

		sf::Texture BulletTexture;
		if (!BulletTexture.loadFromFile("Fireball.png")) {
			printf("Unable To Load Bullet Texture\n"); 
		}

		if (!sf::Shader::isAvailable()) {
			printf("No Shader Available\n");
		}

		simpleShader = new Shader();
		if (!simpleShader->loadFromFile("res/simple.vert", "res/simple.frag")) {
			printf("Unable To Load Shaders\n");
		}

		std::srand(GetTickCount());
		shape.setOrigin(Vector2f(shape.getSize().x / 2, shape.getSize().y / 2 - 10));
		shape2.setOrigin(Vector2f(shape2.getSize().x / 2, shape2.getSize().y / 2 - 5));
		GunP1.setOrigin(Vector2f(GunP1.getSize().x / 2, GunP1.getSize().y / 2 + 30));
		GunP2.setOrigin(Vector2f(GunP2.getSize().x / 2, GunP2.getSize().y / 2 + 40));

		SquareCollider LeftWall(-100, 1080, 100, -2000);
		SquareCollider TopWall(0, -100, 2000, 100);
		SquareCollider RightWall(1900, 1080, 100, -2000);
		SquareCollider BottomWall(0, 1050, 2000, 100);
		SquareCollider MiddleWall(1280 / 2 - 200, 920 / 2 + 200, 400, -400);

		sf::Vector2f vec(400, 400);
		sf::RectangleShape CenterWallShape(vec);
		CenterWallShape.setFillColor(sf::Color::White);
		CenterWallShape.setTexture(&MiddleMap);
		CenterWallShape.setPosition(1280 / 2 - 200, 920 / 2 - 200);

		Vector2f offset1 = Vector2f(shape.getSize().x / 2, shape.getSize().y / 2 - 10);
		Vector2f offset2 = Vector2f(shape2.getSize().x / 2, shape2.getSize().y / 2 - 5);

		sf::Texture TankP1;
		if (!TankP1.loadFromFile("Tank1.png")) {
			printf("Tank Player 1 Loaded");
		}

		sf::Texture CanonP1;
		if (!CanonP1.loadFromFile("Tank1Canon.png")) {
			printf("Canon Player 1 Loaded");
		}

		sf::Texture TankP2;
		if (!TankP2.loadFromFile("Tank2.png")) {
			printf("Tank Player 2 Loaded");
		}

		sf::Texture CanonP2;
		if (!CanonP2.loadFromFile("Tank2Canon.png")) {
			printf("Canon Player 2 Loaded");
		}
		
		sf::Font font;
		if (!font.loadFromFile("Stars Fighters.ttf"))
		{
			printf("Font Loaded");
		}

		shape.setPosition(1600, 900);
		shape2.setPosition(150, 150);

		shape.setTexture(&TankP1);
		shape2.setTexture(&TankP2);
		GunP1.setTexture(&CanonP1);
		GunP2.setTexture(&CanonP2);

		sf::Clock TheClock;
		sf::Time appStart = TheClock.getElapsedTime();
		window.setVerticalSyncEnabled(true);

		float timestamp = 0;
		int Timer = 0;
		int Variable = 1;

		SquareCollider ColliderP1(shape.getPosition().x, shape.getPosition().y, shape.getSize().x, shape.getSize().y - 15);
		ColliderP1.PrevPos = shape.getPosition();

		SquareCollider ColliderP2(shape2.getPosition().x, shape2.getPosition().y, shape2.getSize().x, shape2.getSize().y - 15);
		ColliderP2.PrevPos = shape2.getPosition();

		sf::RectangleShape Flashing(Vector2f(1920,1080));
		Flashing.setFillColor(sf::Color(0xFF362Cff));

		Vector2f InitPos = window.getView().getCenter();

		auto Viewer = window.getView();

		int Maxx = InitPos.x + 10;
		int Minx = InitPos.x - 10;
		int Maxy = InitPos.y + 10;
		int Miny = InitPos.y - 10;

		while (window.isOpen())
		{
			FlashEnabled = false;
			if (timestamp <= 0 && Variable == -1) Variable = 1;
			if (timestamp >= 250 / 50 && Variable == 1) Variable = -1;
			timestamp += Variable * 0.05;
			timestamp2 += 0.01;

			TheClock.restart();
			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();
			}
			window.clear();

			if (Shaking)
			{
				if (ScreenShaker <= 5)
				{
					Vector2f ScreenShake(rand() % (Maxx - Minx + 2) + Minx, rand() % (Maxy - Miny + 2) + Miny);
					Viewer.setCenter(ScreenShake);
					ScreenShaker++;
				}
				else
				{
					Viewer.setCenter(InitPos);
					Shaking = false;
				}
				window.setView(Viewer);
			}

			//Mouvements
			if (GameIsRunning) {
				if (Joystick::isConnected(0) && !P1IsDead && !P2IsDead) {
					if ((sf::Joystick::getAxisPosition(0, Joystick::Y) > 45 || sf::Joystick::getAxisPosition(0, Joystick::Y) < -45) || (sf::Joystick::getAxisPosition(0, Joystick::X) > 45 || sf::Joystick::getAxisPosition(0, Joystick::X) < -45)) {
						float RotP1 = 60 * atan2(sf::Joystick::getAxisPosition(0, Joystick::X), -sf::Joystick::getAxisPosition(0, Joystick::Y)) - 90;						
						shape.setRotation(RotP1 + 90);

					}
					if (sf::Joystick::getAxisPosition(0, Joystick::Y) > -50)
					{
						shape.move(0, 4);
					}
					if (sf::Joystick::getAxisPosition(0, Joystick::Y) < 50)
					{
						shape.move(0, -4);
					}
					if (sf::Joystick::getAxisPosition(0, Joystick::X) > -50)
					{
						shape.move(4, 0);
					}
					if (sf::Joystick::getAxisPosition(0, Joystick::X) < 50)
					{
						shape.move(-4, 0);
					}
				}

				if (Joystick::isConnected(1) && !P2IsDead && !P1IsDead) {
					if ((sf::Joystick::getAxisPosition(1, Joystick::Y) > 45 || sf::Joystick::getAxisPosition(1, Joystick::Y) < -45) || (sf::Joystick::getAxisPosition(1, Joystick::X) > 45 || sf::Joystick::getAxisPosition(1, Joystick::X) < -45)) {
						float RotP2 = 60 * atan2(sf::Joystick::getAxisPosition(1, Joystick::X), -sf::Joystick::getAxisPosition(1, Joystick::Y)) - 90;
						shape2.setRotation(RotP2 + 90);

					}
					if (sf::Joystick::getAxisPosition(1, Joystick::Y) > -50)
					{
						shape2.move(0, 4);
					}
					if (sf::Joystick::getAxisPosition(1, Joystick::Y) < 50)
					{
						shape2.move(0, -4);
					}
					if (sf::Joystick::getAxisPosition(1, Joystick::X) > -50)
					{
						shape2.move(4, 0);
					}
					if (sf::Joystick::getAxisPosition(1, Joystick::X) < 50)
					{
						shape2.move(-4, 0);
					}
				}
							   
				//Targeting
							   
				if (Joystick::isConnected(0) && !P1IsDead && !P2IsDead) {

					if ((sf::Joystick::getAxisPosition(0, Joystick::U) > 45 || sf::Joystick::getAxisPosition(0, Joystick::U) < -45) || (sf::Joystick::getAxisPosition(0, Joystick::V) > 45 || sf::Joystick::getAxisPosition(0, Joystick::V) < -45)) {
						rotationP1 = 60 * atan2(sf::Joystick::getAxisPosition(0, Joystick::U), -sf::Joystick::getAxisPosition(0, Joystick::V)) - 90;
						GunP1.setRotation(rotationP1 + 90);
					}

					if (sf::Joystick::getAxisPosition(0, Joystick::Z) > 50 && P1CanShoot == true)
					{
						P1CanShoot = false;
						Vector2f point(cos(rotationP1 / 60) * 110, sin(rotationP1 / 60) * 110);
						Vector2f point1 = shape.getPosition() + point;
						Bullet CaFaitBoom(point1, point, sf::Color::Green);
						CaFaitBoom.BulletShape.setTexture(&BulletTexture);
						Projectiles.push_back(CaFaitBoom);
						Explosion BimBadaboom(point1 - Vector2f(point.x / 12, point.y / 12), &ShootFX, 24, true);
						ExplosionList.push_back(BimBadaboom);												
					}
					else if (sf::Joystick::getAxisPosition(0, Joystick::Z) < 45) P1CanShoot = true;
				}

				if (Joystick::isConnected(1) && !P2IsDead && !P2IsDead) {

					if ((sf::Joystick::getAxisPosition(1, Joystick::U) > 45 || sf::Joystick::getAxisPosition(1, Joystick::U) < -45) || (sf::Joystick::getAxisPosition(1, Joystick::V) > 45 || sf::Joystick::getAxisPosition(1, Joystick::V) < -45)) {
						rotationP2 = 57.3 * atan2(sf::Joystick::getAxisPosition(1, Joystick::U), -sf::Joystick::getAxisPosition(1, Joystick::V)) - 90;
						GunP2.setRotation(rotationP2 + 90);

					}
					if (sf::Joystick::getAxisPosition(1, Joystick::Z) > 50 && P2CanShoot == true)
					{
						P2CanShoot = false;
						Vector2f point(cos(rotationP2 / 60) * 110, sin(rotationP2 / 60) * 110);
						Vector2f point1 = shape2.getPosition() + point;
						Bullet CaFaitBoom(point1, point, sf::Color::Blue);
						CaFaitBoom.BulletShape.setTexture(&BulletTexture);
						Projectiles.push_back(CaFaitBoom);
						Explosion BimBadaboom(point1-Vector2f(point.x / 12, point.y / 12), &ShootFX, 24, true);
						ExplosionList.push_back(BimBadaboom);
					}
					else if (sf::Joystick::getAxisPosition(1, Joystick::Z) < 45) P2CanShoot = true;
				}
			}

			//Update Colliders

			ColliderP1.update(shape.getPosition().x - shape.getSize().x / 2.0f, shape.getPosition().y - shape.getSize().y / 2 + 20, shape.getSize().x, shape.getSize().y - 30);
			ColliderP2.update(shape2.getPosition().x - shape2.getSize().x / 2.0f - 10, shape2.getPosition().y - shape2.getSize().y / 2 + 15, shape2.getSize().x + 20, shape2.getSize().y - 20);

			//Collision Check

			if (TopWall.CheckColliding(ColliderP1))shape.setPosition(ColliderP1.PrevPos);
			if (BottomWall.CheckColliding(ColliderP1))shape.setPosition(ColliderP1.PrevPos);
			if (RightWall.CheckColliding(ColliderP1))shape.setPosition(ColliderP1.PrevPos);
			if (LeftWall.CheckColliding(ColliderP1))shape.setPosition(ColliderP1.PrevPos);
			if (MiddleWall.CheckColliding(ColliderP1))shape.setPosition(ColliderP1.PrevPos);

			if (TopWall.CheckColliding(ColliderP2))shape2.setPosition(ColliderP2.PrevPos);
			if (BottomWall.CheckColliding(ColliderP2))shape2.setPosition(ColliderP2.PrevPos);
			if (RightWall.CheckColliding(ColliderP2))shape2.setPosition(ColliderP2.PrevPos);
			if (LeftWall.CheckColliding(ColliderP2))shape2.setPosition(ColliderP2.PrevPos);
			if (MiddleWall.CheckColliding(ColliderP2))shape2.setPosition(ColliderP2.PrevPos);

			GunP1.setPosition(shape.getPosition());
			GunP2.setPosition(shape2.getPosition());
			for (Bullet& CaFaitBoom : Projectiles)
			{
				Vector2f VarProjectiles = CaFaitBoom.BulletShape.getPosition();
				CaFaitBoom.BulletShape.setPosition(CaFaitBoom.BulletShape.getPosition() + CaFaitBoom.Direction);
				CaFaitBoom.updatecollider();
			}		

			//BulletBounce

			for (Bullet& QuandTonCoeurFaitBoom : Projectiles)
			{
				if (QuandTonCoeurFaitBoom.timer == 0) {

					if (TopWall.CheckCollidingWithSphere(QuandTonCoeurFaitBoom.BulletCollision)) {
						if (QuandTonCoeurFaitBoom.Life == 1)
						{
							Explosion OnVaFaireCaCommeDansLesLivres(QuandTonCoeurFaitBoom.BulletShape.getPosition(), &ShootFX, 30,false);
							ExplosionList.push_back(OnVaFaireCaCommeDansLesLivres);
							Projectiles.remove(QuandTonCoeurFaitBoom);
							ScreenShaker = 0;
							Shaking = true;
							Flashing.setFillColor(sf::Color(255, 255, 255, 60));
							FlashEnabled = true;
							Maxx = InitPos.x + 5;
							Minx = InitPos.x - 5;
							Maxy = InitPos.y + 5;
							Miny = InitPos.y - 5;
							break;
						}
						else if (QuandTonCoeurFaitBoom.Life >= 2)
						{
							QuandTonCoeurFaitBoom.timer = 1;
							QuandTonCoeurFaitBoom.Life -= 1;
							float y = 0;
							float x = 1;
							Vector2f VarVar(y, x);
							if (TopWall.GetAngleOfCollision(QuandTonCoeurFaitBoom.BulletCollision) == 0)VarVar = Vector2f(x, y);
							Vector2f VarA = (((QuandTonCoeurFaitBoom.Direction.x* VarVar.x) + (QuandTonCoeurFaitBoom.Direction.y* VarVar.y)) / ((VarVar.x* VarVar.x) + (VarVar.y* VarVar.y))) * VarVar;
							Vector2f VarB = QuandTonCoeurFaitBoom.Direction - VarA;
							QuandTonCoeurFaitBoom.Direction = VarB - VarA;
						}
					}

					if (BottomWall.CheckCollidingWithSphere(QuandTonCoeurFaitBoom.BulletCollision)) {
						if (QuandTonCoeurFaitBoom.Life == 1)
						{
							Explosion BimBadaboom(QuandTonCoeurFaitBoom.BulletShape.getPosition(), &ShootFX, 30, false);
							ExplosionList.push_back(BimBadaboom);
							Projectiles.remove(QuandTonCoeurFaitBoom);
							ScreenShaker = 0;
							Shaking = true;
							Flashing.setFillColor(sf::Color(255, 255, 255, 60));
							FlashEnabled = true;
							Maxx = InitPos.x + 5;
							Minx = InitPos.x - 5;
							Maxy = InitPos.y + 5;
							Miny = InitPos.y - 5;
							break;
						}
						else if (QuandTonCoeurFaitBoom.Life >= 2)
						{
							QuandTonCoeurFaitBoom.timer = 1;
							QuandTonCoeurFaitBoom.Life -= 1;
							float y = 0;
							float x = 1;
							Vector2f n(y, x);
							if (BottomWall.GetAngleOfCollision(QuandTonCoeurFaitBoom.BulletCollision) == 0)n = Vector2f(x, y);
							Vector2f u = (((QuandTonCoeurFaitBoom.Direction.x* n.x) + (QuandTonCoeurFaitBoom.Direction.y* n.y)) / ((n.x* n.x) + (n.y* n.y))) * n;
							Vector2f w = QuandTonCoeurFaitBoom.Direction - u;
							QuandTonCoeurFaitBoom.Direction = w - u;
						}
					}

					if (RightWall.CheckCollidingWithSphere(QuandTonCoeurFaitBoom.BulletCollision)) {
						if (QuandTonCoeurFaitBoom.Life == 1)
						{
							Explosion BimBadaboom(QuandTonCoeurFaitBoom.BulletShape.getPosition(), &ShootFX, 30, false);
							ExplosionList.push_back(BimBadaboom);
							Projectiles.remove(QuandTonCoeurFaitBoom);
							ScreenShaker = 0;
							Shaking = true;
							Flashing.setFillColor(sf::Color(255, 255, 255, 60));
							FlashEnabled = true;
							Maxx = InitPos.x + 5;
							Minx = InitPos.x - 5;
							Maxy = InitPos.y + 5;
							Miny = InitPos.y - 5;
							break;
						}
						else if (QuandTonCoeurFaitBoom.Life >= 2)
						{
							QuandTonCoeurFaitBoom.timer = 1;
							QuandTonCoeurFaitBoom.Life -= 1;
							float y = 0;
							float x = 1;
							Vector2f n(y, x);
							if (RightWall.GetAngleOfCollision(QuandTonCoeurFaitBoom.BulletCollision) == 0) n = Vector2f(x, y);
							Vector2f u = (((QuandTonCoeurFaitBoom.Direction.x* n.x) + (QuandTonCoeurFaitBoom.Direction.y* n.y)) / ((n.x* n.x) + (n.y* n.y))) * n;
							Vector2f w = QuandTonCoeurFaitBoom.Direction - u;
							QuandTonCoeurFaitBoom.Direction = w - u;
						}
					}

					if (LeftWall.CheckCollidingWithSphere(QuandTonCoeurFaitBoom.BulletCollision)) {
						if (QuandTonCoeurFaitBoom.Life == 1)
						{
							Explosion BimBadaboom(QuandTonCoeurFaitBoom.BulletShape.getPosition(), &ShootFX, 30, false);
							ExplosionList.push_back(BimBadaboom);
							Projectiles.remove(QuandTonCoeurFaitBoom);
							ScreenShaker = 0;
							Shaking = true;
							Flashing.setFillColor(sf::Color(255, 255, 255, 60));
							FlashEnabled = true;
							Maxx = InitPos.x + 5;
							Minx = InitPos.x - 5;
							Maxy = InitPos.y + 5;
							Miny = InitPos.y - 5;
							break;
						}
						else if (QuandTonCoeurFaitBoom.Life >= 2)
						{
							QuandTonCoeurFaitBoom.timer = 1;
							QuandTonCoeurFaitBoom.Life -= 1;
							float y = 0;
							float x = 1;
							Vector2f n(y, x);
							if (LeftWall.GetAngleOfCollision(QuandTonCoeurFaitBoom.BulletCollision) == 0)n = Vector2f(x, y);
							Vector2f u = (((QuandTonCoeurFaitBoom.Direction.x* n.x) + (QuandTonCoeurFaitBoom.Direction.y* n.y)) / ((n.x* n.x) + (n.y* n.y))) * n;
							Vector2f w = QuandTonCoeurFaitBoom.Direction - u;
							QuandTonCoeurFaitBoom.Direction = w - u;
						}
					}

					if (MiddleWall.CheckCollidingWithSphere(QuandTonCoeurFaitBoom.BulletCollision)) {
						if (QuandTonCoeurFaitBoom.Life == 1)
						{
							Explosion BimBadaboom(QuandTonCoeurFaitBoom.BulletShape.getPosition(), &ShootFX, 30, false);
							ExplosionList.push_back(BimBadaboom);
							Projectiles.remove(QuandTonCoeurFaitBoom);
							ScreenShaker = 0;
							Shaking = true;
							Flashing.setFillColor(sf::Color(255, 255, 255, 60));
							FlashEnabled = true;
							Maxx = InitPos.x + 5;
							Minx = InitPos.x - 5;
							Maxy = InitPos.y + 5;
							Miny = InitPos.y - 5;
							break;
						}
						else if (QuandTonCoeurFaitBoom.Life >= 2)
						{
							QuandTonCoeurFaitBoom.timer = 1;
							QuandTonCoeurFaitBoom.Life -= 1;
							float y = 0;
							float x = 1;
							Vector2f n(y, x);
							if (MiddleWall.GetAngleOfCollision(QuandTonCoeurFaitBoom.BulletCollision) == 0)n = Vector2f(x, y);
							Vector2f u = (((QuandTonCoeurFaitBoom.Direction.x* n.x) + (QuandTonCoeurFaitBoom.Direction.y* n.y)) / ((n.x* n.x) + (n.y* n.y))) * n;
							Vector2f w = QuandTonCoeurFaitBoom.Direction - u;
							QuandTonCoeurFaitBoom.Direction = w - u;
						}
					}
				}
				else QuandTonCoeurFaitBoom.timer = QuandTonCoeurFaitBoom.timer + 1;

				if (ColliderP1.CheckCollidingWithSphere(QuandTonCoeurFaitBoom.BulletCollision) && !P1IsDead) {
					Explosion BimBadaboom(shape.getPosition(), &ExplosionFX, 80, false);
					ExplosionList.push_back(BimBadaboom);
					Explosion AlorsOnDanse(shape.getPosition(), &ShootFX, 80, false);
					ExplosionList.push_back(AlorsOnDanse);
					Flashing.setFillColor(sf::Color(255, 255, 255, 170));
					FlashEnabled = true;
					shape.setFillColor(sf::Color::Transparent);
					GunP1.setFillColor(sf::Color::Transparent);
					P1IsDead = true;
					if (!P2IsDead) { P2Win = true;  ScoreP2 += 1; }
					Projectiles.remove(QuandTonCoeurFaitBoom);
					ScreenShaker = -50;
					Shaking = true;
					Maxx = InitPos.x + 20;
					Minx = InitPos.x - 20;
					Maxy = InitPos.y + 20;
					Miny = InitPos.y - 20;
					break;
				}

				if (ColliderP2.CheckCollidingWithSphere(QuandTonCoeurFaitBoom.BulletCollision) && !P2IsDead) {
					Explosion BimBadaboom(shape2.getPosition(), &ExplosionFX, 80, false);
					ExplosionList.push_back(BimBadaboom);
					Explosion AlorsOnDanse(shape2.getPosition(), &ShootFX, 80, false);
					ExplosionList.push_back(AlorsOnDanse);
					Flashing.setFillColor(sf::Color(255, 255, 255, 170));
					FlashEnabled = true;
					shape2.setFillColor(sf::Color::Transparent);
					GunP2.setFillColor(sf::Color::Transparent);
					P2IsDead = true;
					if (!P1IsDead) { P1Win = true; ScoreP1 += 1; }
					Projectiles.remove(QuandTonCoeurFaitBoom);
					ScreenShaker = -50;
					Shaking = true;
					Maxx = InitPos.x + 20;
					Minx = InitPos.x - 20;
					Maxy = InitPos.y + 20;
					Miny = InitPos.y - 20;
					break;
				}
				if (QuandTonCoeurFaitBoom.timer > 0) QuandTonCoeurFaitBoom.timer = 0;
			}

			//Particles

			for (Explosion& CaFaitBoom : ExplosionList)
			{
				if (CaFaitBoom.Timer <= CaFaitBoom.MaxTimer - 30) {
					ExplosionList.remove(CaFaitBoom);
					break;
				}
				else {
					CaFaitBoom.UpdateParticle();
				}
			}
					   
			//Update PrevPos

			ColliderP1.PrevPos = shape.getPosition();
			ColliderP2.PrevPos = shape2.getPosition(); 

			//Render
			
			window.draw(BackgroundShape);

			for (Bullet QuandTonCoeurFaitBoom : Projectiles)
			{
				window.draw(QuandTonCoeurFaitBoom.BulletShape);
			}

			window.draw(shape);
			window.draw(shape2);
			window.draw(GunP1);
			window.draw(GunP2);
			window.draw(CenterWallShape);

			for (Explosion QuandTonCoeurFaitBoom : ExplosionList) {
				window.draw(QuandTonCoeurFaitBoom.ExplosionShape);
			}

			if(FlashEnabled)window.draw(Flashing);

			sf::Text text;
			text.setOutlineColor(sf::Color(0x3200FAff));
			text.setOutlineThickness(3);
			text.setFont(font);
			text.setFillColor(sf::Color(0xFF362Cff));
			text.setPosition(525, 800);
			text.setString("Press A To Start Playing");
			text.setCharacterSize(30);

			sf::Text Score1;
			Score1.setOutlineColor(sf::Color(0x3200FAff));
			Score1.setOutlineThickness(3);
			Score1.setFont(font);
			Score1.setFillColor(sf::Color(0xF08820ff));
			Score1.setPosition(30, 20);
			Score1.setString("P1 Score : "+ std::to_string(ScoreP1));
			Score1.setCharacterSize(20);

			sf::Text Score2;
			Score2.setOutlineColor(sf::Color(0x3200FAff));
			Score2.setOutlineThickness(3);
			Score2.setFont(font);
			Score2.setFillColor(sf::Color(0xF08820ff));
			Score2.setPosition(30, 70);
			Score2.setString("P2 Score : " + std::to_string(ScoreP2));
			Score2.setCharacterSize(20);

			if (P1Win)
			{
				text.setPosition(550, 800);
				text.setString("Press Start To Restart \n \n Press Select To Quit");
			}
			else if (P2Win)
			{
				text.setPosition(550, 800);
				text.setString("Press Start To Restart \n \n Press Select To Quit");
			}

			if (!P1IsDead && !P2IsDead)
			{

			}
			window.draw(Score1);
			window.draw(Score2);

			if (!Joystick::isConnected(1))
			{

			}
			if ((!GameIsRunning || (P1IsDead || P2IsDead) )&& TimerText >= 0) window.draw(text);
			if (!GameIsRunning && sf::Joystick::isButtonPressed(0, 0)) GameIsRunning = true;

			TimerText++;

			if (TimerText > 15) TimerText = -16;
			window.display();

			if (GameIsRunning && sf::Joystick::isButtonPressed(0, 7)) { 
				P1Win = false;
				P2Win = false; 
				ExplosionList.clear(); 
				Projectiles.clear();
				GameIsRunning = false;
				break; 
			}
			if (GameIsRunning && sf::Joystick::isButtonPressed(0, 6)) {
				window.close();
			}
		}
	}	
	return 0;
}
