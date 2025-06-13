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
	, player1healthText(uiFont)
	, player2healthText(uiFont)
	, player1Health(100)
	, player2Health(100)
	, screenSize(newScreenSize)
	//--

	, timeSinceSpawn(0.0f)
	, spawnCooldown(1.0f)

	, gunTexture("Assets/tanks_turret1.png")
	, ismyPlayerTurn(true)   // enforce start turn here
	
{



	//Ground set up 
	float groundHeight = 365.f; // You can adjust the height
	ground.setSize(sf::Vector2f(screenSize.x, groundHeight)); // Full width
	ground.setFillColor(sf::Color(0, 180, 0));       // Green color
	ground.setPosition(sf::Vector2f(0.f, screenSize.y - groundHeight));    // Align at bottom of screen

	

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



	player1healthText.setCharacterSize(24);
	player1healthText.setFillColor(sf::Color::White);
	player1healthText.setString("Health: " + std::to_string(player1Health));
	player1healthText.setPosition({ 50.f, bottomThirdY + 70.f });

	player2healthText.setCharacterSize(24);
	player2healthText.setFillColor(sf::Color::White);
	player2healthText.setString("Health: " + std::to_string(player2Health));
	player2healthText.setPosition({ screenSize.x - 200.f, bottomThirdY + 70.f });
}

	

LevelScreen::~LevelScreen()
{
	delete myPlayer;
	delete myPlayer2;

		for (int i = 0; i < bullets.size(); ++i)
		{
			delete bullets[i];
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
	target.draw(player1healthText);
	target.draw(player2healthText);
} 

void LevelScreen::Update(float frameTime)
{// ?? Update only the active player
	if (player == 0)
	{
		myPlayer->Update(frameTime);
	}
	else
	{

		myPlayer2->Update(frameTime);
	}

	for (int i = bullets.size() - 1; i >= 0; --i)
	{
		bullets[i]->Update(frameTime);

		// Remove if off screen
		if (!bullets[i]->IsOnScreen())
		{
			delete bullets[i];
			bullets.erase(bullets.begin() + i);
			SwitchTurn();
			continue;
		}

		sf::FloatRect bulletBounds = bullets[i]->GetGlobalBounds();
		sf::FloatRect groundBounds = ground.getGlobalBounds();

		sf::Vector2f bPos = bulletBounds.position;
		sf::Vector2f bSize = bulletBounds.size;

		sf::Vector2f gPos = groundBounds.position;
		sf::Vector2f gSize = groundBounds.size;

		bool intersects =
			bPos.x < gPos.x + gSize.x &&
			bPos.x + bSize.x > gPos.x &&
			bPos.y < gPos.y + gSize.y &&
			bPos.y + bSize.y > gPos.y;

		if (intersects)
		{
			delete bullets[i];
			bullets.erase(bullets.begin() + i);
			SwitchTurn();
			continue;
		}

		Player* owner = bullets[i]->GetOwner(); // Get the shooter

		// Player 1 hit check — but only if the bullet was not shot by Player 1
		if (owner != myPlayer)
		{
			sf::Vector2f p1Pos = myPlayer->GetGlobalBounds().position;
			sf::Vector2f p1Size = myPlayer->GetGlobalBounds().size;

			bool hitsP1 =
				bPos.x < p1Pos.x + p1Size.x &&
				bPos.x + bSize.x > p1Pos.x &&
				bPos.y < p1Pos.y + p1Size.y &&
				bPos.y + bSize.y > p1Pos.y;

			if (hitsP1)
			{
				player1Health -= 10;
				if (player1Health < 0) player1Health = 0;
				player1healthText.setString("Health: " + std::to_string(player1Health));

				delete bullets[i];
				bullets.erase(bullets.begin() + i);
				SwitchTurn();
			

				continue;
			}
		}

		// Player 2 hit check — only if the bullet was not shot by Player 2
		if (owner != myPlayer2)
		{
			sf::Vector2f p2Pos = myPlayer2->GetGlobalBounds().position;
			sf::Vector2f p2Size = myPlayer2->GetGlobalBounds().size;

			bool hitsP2 =
				bPos.x < p2Pos.x + p2Size.x &&
				bPos.x + bSize.x > p2Pos.x &&
				bPos.y < p2Pos.y + p2Size.y &&
				bPos.y + bSize.y > p2Pos.y;

			if (hitsP2)
			{
				player2Health -= 10;
				if (player2Health < 0) player2Health = 0;
				player2healthText.setString("Health: " + std::to_string(player2Health));

				delete bullets[i];
				bullets.erase(bullets.begin() + i);
				SwitchTurn();
				
				continue;
			}
		}
	}
}




void LevelScreen::SwitchTurn()
{
	if (player == 0)
	{
		player = 1;
	}
	else
	{
		player = 0;
	}

}

Bullet* LevelScreen::SpawnBullet(sf::Vector2f pos, float speed, float angle, float windPower, sf::Vector2f velocity,Player* owner)
{
	Bullet* tempBullet = new Bullet(bulletTex,
		speed,
		angle,
		windPower,
		velocity,
		pos,
		owner);

	bullets.push_back(tempBullet);

	// ?? Optional: if you want to switch turns *only after bullet finishes*
	// SwitchTurn();

	return tempBullet;
}

