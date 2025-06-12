#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Bullet.h"


class LevelScreen
{

public:

	// Constructors
	LevelScreen(sf::Vector2f screenSize);
	~LevelScreen();

	// Public Functions
	void DrawTo(sf::RenderTarget& target);
	void Update(float frameTime);

	Bullet* SpawnBullet(sf::Vector2f pos,
		float speed,
		float angle);
	void SwitchTurn();


private:

	Player* myPlayer;
	Player* myPlayer2;
	std::vector<Bullet*> bullets;
	sf::Texture bulletTex;
	sf::RectangleShape ground;
	

	//---
	sf::Font uiFont;
	sf::Text healthText;

	int cityHealth;
	sf::Vector2f screenSize;
	//---

	sf::Texture tankTextures[2];
	sf::Texture gunTexture;

	bool ismyPlayerTurn = true;
	float timeSinceSpawn;
	float spawnCooldown;


};
