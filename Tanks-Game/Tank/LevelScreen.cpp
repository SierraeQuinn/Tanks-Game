#include "LevelScreen.h"
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
{


	// UI Setup
	healthText.setPosition({ 50, 50 });
	healthText.setString("Health: " + std::to_string(cityHealth));

	tankTextures[0].loadFromFile("Assets/tanks_tankDesert_body3.png");
	tankTextures[1].loadFromFile("Assets/tanks_tankNavy_body3.png");

	float bottomThirdY = screenSize.y * (2.0f / 3.0f) - tankTextures[0].getSize().y / 2.0f;

	myPlayer = new Player(
		{ 100.0f, bottomThirdY },
		this, &tankTextures[0], &gunTexture);

	myPlayer2 = new Player(
		{ screenSize.x - 100.0f, bottomThirdY },
		this, &tankTextures[1], &gunTexture);
}

	

LevelScreen::~LevelScreen()
{

	for (int i = 0; i < bullets.size(); ++i)
	{
		delete myPlayer;
		delete myPlayer2;

		for (int i = 0; i < bullets.size(); ++i)
		{
			delete bullets[i];
		}
	}
}


void LevelScreen::DrawTo(sf::RenderTarget& target)
{
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

	myPlayer->SetActive(ismyPlayerTurn);
	myPlayer2->SetActive(!ismyPlayerTurn);
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