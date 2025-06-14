#include "LevelScreen.h"
#include "Player.h"
#include "Bullet.h"
#include "ExplosiveBullet.h"
#include "SpeedBullet.h"
#include "ExplosionEffect.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cstdlib> // for rand()
#include <ctime>   // for time()
#include <iostream>
#include <string> 

LevelScreen::LevelScreen(sf::Vector2f newScreenSize)
	: bullets()
	, bulletTex("Assets/tank_bullet1.png")
	, explosiveBulletTex("Assets/tank_bullet3.png")
	, speedBulletTex("Assets/tank_bullet6.png")
	, compassTexture("Assets/Compass.png")
	, compassSprite(compassTexture)
	,explosionBuffer("Assets/explosionCrunch_000.ogg")
	,explosionSound(explosionBuffer)
	

	//--
	, uiFont("Assets/Quantico-Regular.ttf")
	, player1healthText(uiFont)
	, player2healthText(uiFont)
	, windText(uiFont)
	, screenSize(newScreenSize)
	, winnerText(uiFont)
	, restartText(uiFont)
	, turnText(uiFont)
	, player1AmmoText(uiFont)
	, player2AmmoText(uiFont)
	//--

	, timeSinceSpawn(0.0f)
	, spawnCooldown(1.0f)

	, gunTexture("Assets/tanks_turret1.png")
	
	
{
	if (!explosionTexture.loadFromFile("Assets/tank_explosion3.png"))
	{
		std::cerr << "Failed to load explosion texture!" << std::endl;
	}

	//Ground set up 
	float groundHeight = 365.f; // You can adjust the height
	ground.setSize(sf::Vector2f(screenSize.x, groundHeight)); // Full width
	ground.setFillColor(sf::Color(0, 180, 0));       // Green color
	ground.setPosition(sf::Vector2f(0.f, screenSize.y - groundHeight));    // Align at bottom of screen

	compassSprite.setOrigin({0.f,0.f });
	compassSprite.setPosition(sf::Vector2f(50.f, 10.f));
	compassSprite.setRotation(sf::degrees(0.0f));        // Optional: adjust size

	if (!tankTextures[0].loadFromFile("Assets/tanks_tankDesert_body3.png"))
	{
		std::cerr << "Failed to load desert tank texture!" << std::endl;
	}

	if (!tankTextures[1].loadFromFile("Assets/tanks_tankNavy_body3.png"))
	{
		std::cerr << "Failed to load navy tank texture!" << std::endl;
	}
	
	explosionSound.setBuffer(explosionBuffer);

	

	float bottomThirdY = screenSize.y * (2.0f / 3.0f) - tankTextures[0].getSize().y / 2.0f;

	startingPos1 = { 100.0f, bottomThirdY };
	startingPos2 = { screenSize.x - 100.0f, bottomThirdY };

	myPlayer = new Player(
		{ startingPos1 },
		this, &tankTextures[0], &gunTexture);
	// Default actor sets angle = 90, so faces right already.

	myPlayer2 = new Player(
		{ startingPos2 },
		this, &tankTextures[1], &gunTexture);
	// Immediately flip it to face left (–90° = left in your system)
	myPlayer2->SetAngle(-90.0f);


	// winner Text setup
	winnerText.setFont(uiFont);
	winnerText.setCharacterSize(48);
	winnerText.setFillColor(sf::Color::Black);
	winnerText.setPosition({ 700,300 });

	// restart Text setup
	restartText.setFont(uiFont);
	restartText.setCharacterSize(48);
	restartText.setFillColor(sf::Color::Black);
	restartText.setString("Press Enter to Restart ");
	restartText.setPosition({ 700,400 });
	

	windText.setFont(uiFont);
	windText.setCharacterSize(24);
	windText.setFillColor(sf::Color::Black);
	windText.setPosition({ screenSize.x / 2.f - 150.f, 50.f });
	windText.setString("Wind: 0");

	turnText.setFont(uiFont);
	turnText.setCharacterSize(24);
	turnText.setFillColor(sf::Color::Black);
	turnText.setPosition({ screenSize.x / 2.f - 80.f, 10.f }); // Adjust as needed
	turnText.setString("Player 1's Turn");

	// Generate random wind on startup
	GenerateRandomWind();

	player1healthText.setCharacterSize(24);
	player1healthText.setFillColor(sf::Color::White);
	player1healthText.setString(" Player 1 Health: " + std::to_string(myPlayer->GetHealth()));
	player1healthText.setPosition({ 50.f, bottomThirdY + 70.f });

	player1AmmoText.setFont(uiFont);
	player1AmmoText.setCharacterSize(20);
	player1AmmoText.setFillColor(sf::Color::White);
	player1AmmoText.setPosition({ 50.f, 800.f }); // Adjust Y as needed

	player2healthText.setCharacterSize(24);
	player2healthText.setFillColor(sf::Color::White);
	player2healthText.setString("Player 2 Health: " + std::to_string(myPlayer2->GetHealth()));
	player2healthText.setPosition({ screenSize.x - 300.f, bottomThirdY + 70.f });
	player1AmmoText.setString("Player 1 Ammo: Normal");

	player2AmmoText.setFont(uiFont);
	player2AmmoText.setCharacterSize(20);
	player2AmmoText.setFillColor(sf::Color::White);
	player2AmmoText.setPosition({ screenSize.x - 300.f, 800.f }); // Adjust as needed
	player2AmmoText.setString("Player 2 Ammo: Normal");
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

	for (auto& explosion : explosions)
	{
		explosion.DrawTo(target);
	}

	// UI
	target.draw(player1healthText);
	target.draw(player2healthText);
	target.draw(windText);
	target.draw(compassSprite);
	target.draw(turnText);
	target.draw(player1AmmoText);
	target.draw(player2AmmoText);

	if (gameOver)
	{
		target.draw(winnerText);
		target.draw(restartText);
	}
} 

void LevelScreen::Update(float frameTime)
{
	if (gameOver)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter))
		{
			RestartGame(); // you'll define this
		}
		return; // skip the rest of Update
	}

	bool tabPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Tab);

	// Trigger only when Tab is *just* pressed (not held)
	if (tabPressed && !wasTabPressedLastFrame)
	{
		if (player == 0)
		{
			int nextAmmo = static_cast<int>(player1AmmoType) + 1;
			if (nextAmmo > static_cast<int>(AmmoType::Speed)) nextAmmo = 0;
			player1AmmoType = static_cast<AmmoType>(nextAmmo);
		}
		else
		{
			int nextAmmo = static_cast<int>(player2AmmoType) + 1;
			if (nextAmmo > static_cast<int>(AmmoType::Speed)) nextAmmo = 0;
			player2AmmoType = static_cast<AmmoType>(nextAmmo);
		}

		UpdateAmmoTypeText(); // Refresh display
	}
	wasTabPressedLastFrame = tabPressed;


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
			if (dynamic_cast<ExplosiveBullet*>(bullets[i])) 
			{
				explosions.emplace_back(explosionTexture, bullets[i]->GetPosition());
				explosionSound.play();
			}

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
			if (dynamic_cast<ExplosiveBullet*>(bullets[i])) 
			{
				explosions.emplace_back(explosionTexture, bullets[i]->GetPosition());
				explosionSound.play();
			}

			delete bullets[i];
			bullets.erase(bullets.begin() + i);
			SwitchTurn();
			continue;
		}

		Player* owner = bullets[i]->GetOwner(); // Get the shooter

		// Player 1 hit check — only if the bullet was NOT shot by Player 1
		if (owner != myPlayer) 
		{
			sf::Vector2f p1Pos = myPlayer->GetGlobalBounds().position;
			sf::Vector2f p1Size = myPlayer->GetGlobalBounds().size;

			bool hitsP1 =
				bPos.x < p1Pos.x + p1Size.x &&
				bPos.x + bSize.x > p1Pos.x &&
				bPos.y < p1Pos.y + p1Size.y &&
				bPos.y + bSize.y > p1Pos.y;

			if (hitsP1) {
				bullets[i]->OnHit(myPlayer);  // Damage logic handled inside Bullet
				player1healthText.setString("Health: " + std::to_string(myPlayer->GetHealth()));

				if (dynamic_cast<ExplosiveBullet*>(bullets[i])) 
				{
					explosions.emplace_back(explosionTexture, bullets[i]->GetPosition());
				}
				delete bullets[i];
				bullets.erase(bullets.begin() + i);

				if (myPlayer->GetHealth() <= 0)
				{
					// Player 2 wins
					winnerText.setString("Player 2 Wins!");
					gameOver = true;
					return; // Exit early if needed
				}
				SwitchTurn();
				continue;
			}
		}

		// Player 2 hit check — only if the bullet was NOT shot by Player 2
		if (owner != myPlayer2) 
		{
			sf::Vector2f p2Pos = myPlayer2->GetGlobalBounds().position;
			sf::Vector2f p2Size = myPlayer2->GetGlobalBounds().size;

			bool hitsP2 =
				bPos.x < p2Pos.x + p2Size.x &&
				bPos.x + bSize.x > p2Pos.x &&
				bPos.y < p2Pos.y + p2Size.y &&
				bPos.y + bSize.y > p2Pos.y;

			if (hitsP2) {
				bullets[i]->OnHit(myPlayer2);  // Damage logic handled inside Bullet
				player2healthText.setString("Health: " + std::to_string(myPlayer2->GetHealth()));

				if (dynamic_cast<ExplosiveBullet*>(bullets[i]))
				{
					explosions.emplace_back(explosionTexture, bullets[i]->GetPosition());
				}


				delete bullets[i];
				bullets.erase(bullets.begin() + i);
				// 🏆 Win Condition Check
				if (myPlayer2->GetHealth() <= 0)
				{
					// Player 1 wins
					winnerText.setString("Player 1 Wins!");
					gameOver = true;
					return; // Exit early if needed
				}
				SwitchTurn();
				continue;
			}
		}
	}

	for (int i = explosions.size() - 1; i >= 0; --i)
	{
		explosions[i].Update(frameTime);
		if (explosions[i].IsFinished())
			explosions.erase(explosions.begin() + i);
	}
	int p1Health = myPlayer->GetHealth();
	int p2Health = myPlayer2->GetHealth();


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

	if (player == 0)
		turnText.setString("Player 1's Turn");
	else
		turnText.setString("Player 2's Turn");

	GenerateRandomWind();
	UpdateAmmoTypeText();
}

Bullet* LevelScreen::SpawnBullet(sf::Vector2f pos, float speed, float angle, float windPower, sf::Vector2f velocity, Player* owner)
{
	Bullet* tempBullet = nullptr;

	// Get the correct ammo type
	AmmoType currentAmmoType = (player == 0) ? player1AmmoType : player2AmmoType;

	switch (currentAmmoType)
	{
	case AmmoType::Normal:
		tempBullet = new Bullet(bulletTex, speed, angle, windPower, velocity, pos, owner);
		break;
	case AmmoType::Explosive:
		tempBullet = new ExplosiveBullet(explosiveBulletTex, speed, angle, windPower, velocity, pos, owner);
		break;
	case AmmoType::Speed:
		tempBullet = new SpeedBullet(speedBulletTex, speed, angle, windPower, velocity, pos, owner);
		break;
	}

	bullets.push_back(tempBullet);
	return tempBullet;
}

void LevelScreen::GenerateRandomWind()
{
	int direction = std::rand() % 2; // 0 or 1
	windDirectionStr = (direction == 0) ? "East" : "West";

	windPower = static_cast<float>(std::rand() % 151); // Range: 0 to 150

	if (windDirectionStr == "West")
		windPower = -windPower;  // Negative wind for West

	windText.setString("Wind is blowing towards the: " + windDirectionStr + " at " + std::to_string(static_cast<int>(std::abs(windPower))));
}

void LevelScreen::RestartGame()
{
	gameOver = false;

	// Reset health
	myPlayer->SetHealth(100);
	myPlayer2->SetHealth(100);

	// Reset positions (if you want to)
	myPlayer->SetPosition(startingPos1);
	myPlayer2->SetPosition(startingPos2);

	// Reset other game states
	player = 0; // or whichever player starts
	bullets.clear();
	explosions.clear();

	player1healthText.setString("Health: 100");
	player2healthText.setString("Health: 100");
}

void LevelScreen::UpdateAmmoTypeText()
{
	std::string name1, name2;

	switch (player1AmmoType)
	{
	case AmmoType::Normal:    name1 = "Normal"; break;
	case AmmoType::Explosive: name1 = "Explosive"; break;
	case AmmoType::Speed:     name1 = "Speed"; break;
	}

	switch (player2AmmoType)
	{
	case AmmoType::Normal:    name2 = "Normal"; break;
	case AmmoType::Explosive: name2 = "Explosive"; break;
	case AmmoType::Speed:     name2 = "Speed"; break;
	}

	player1AmmoText.setString("P1 Ammo: " + name1);
	player2AmmoText.setString("P2 Ammo: " + name2);
}




