#include "LevelScreen.h"
#include "Player.h"
#include "Bullet.h"
#include <SFML/Graphics.hpp>
#include <string> 

LevelScreen::LevelScreen(sf::Vector2f newScreenSize)
	: bullets()
	, bulletTex("Assets/tank_bullet1.png")

	//--
	, uiFont("Assets/Quantico-Regular.ttf")
	, healthText(uiFont)
	, cityHealth(100)
	, screenSize(newScreenSize)
	//--

	, timeSinceSpawn(0.0f)
	, spawnCooldown(1.0f)

	, gunTexture("Assets/tanks_turret1.png")
	, ismyPlayerTurn(true)   // enforce start turn here
	
{


	// UI Setup
	//Ground set up 
	float groundHeight = 365.f; // You can adjust the height
	ground.setSize(sf::Vector2f(screenSize.x, groundHeight)); // Full width
	ground.setFillColor(sf::Color(0, 180, 0));       // Green color
	ground.setPosition(sf::Vector2f(0.f, screenSize.y - groundHeight));    // Align at bottom of screen

	healthText.setPosition({ 50, 50 });
	healthText.setString("Health: " + std::to_string(cityHealth));

	tankTextures[0].loadFromFile("Assets/tanks_tankDesert_body3.png");
	tankTextures[1].loadFromFile("Assets/tanks_tankNavy_body3.png");

	float bottomThirdY = screenSize.y * (2.0f / 3.0f) - tankTextures[0].getSize().y / 2.0f;

	myPlayer = new Player(
		{ 100.0f, bottomThirdY },
		this, &tankTextures[0], &gunTexture);
	// Default actor sets angle = 90, so faces right already.

	myPlayer2 = new Player(
		{ screenSize.x - 100.0f, bottomThirdY },
		this, &tankTextures[1], &gunTexture);
	// Immediately flip it to face left (–90° = left in your system)
	myPlayer2->SetAngle(-90.0f);

	myPlayer->SetActive(true);
	myPlayer2->SetActive(false);
}

	

LevelScreen::~LevelScreen()
{
	delete myPlayer;
	delete myPlayer2;
	for (int i = 0; i < bullets.size(); ++i)
	{
		

		for (int i = 0; i < bullets.size(); ++i)
		{
			delete bullets[i];
		}
	}
}


void LevelScreen::DrawTo(sf::RenderTarget& target)
{
	target.draw(ground);
	myPlayer->DrawTo(target);
	myPlayer2->DrawTo(target);
	for (int i = 0; i < bullets.size(); ++i)
	{
		bullets[i]->DrawTo(target);
	}

	

	// UI
	target.draw(healthText);
} 

void LevelScreen::Update(float frameTime)
{// ?? Update only the active player
	if (ismyPlayerTurn)
	{
		myPlayer->SetActive(true);
		myPlayer2->SetActive(false);
		myPlayer->Update(frameTime);
	}
	else
	{
		myPlayer->SetActive(false);
		myPlayer2->SetActive(true);
		myPlayer2->Update(frameTime);
	}

	// ?? Update bullets
	for (int i = bullets.size() - 1; i >= 0; --i)
	{
		bullets[i]->Update(frameTime);
		if (!bullets[i]->IsOnScreen())
		{
			delete bullets[i];
			bullets.erase(bullets.begin() + i);
		}
	}



}

void LevelScreen::SwitchTurn()
{
	ismyPlayerTurn = !ismyPlayerTurn;

}

Bullet* LevelScreen::SpawnBullet(sf::Vector2f pos, float speed, float angle)
{
	Bullet* tempBullet = new Bullet(bulletTex,
		speed,
		angle,
		pos);

	bullets.push_back(tempBullet);

	return tempBullet;
}