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
		float angle,
		float windPower,
		sf::Vector2f velocity,
		Player* owner);
	void SwitchTurn();


private:

	Player* myPlayer;
	Player* myPlayer2;
	int player = 0;
	std::vector<Bullet*> bullets;
	sf::Texture bulletTex;
	sf::RectangleShape ground;
	

	//---
	sf::Font uiFont;
	sf::Text player1healthText;
	sf::Text player2healthText;

	int player1Health;
	int player2Health;
	sf::Vector2f screenSize;
	//---

	sf::Texture tankTextures[2];
	sf::Texture gunTexture;

	bool ismyPlayerTurn = true;
	float timeSinceSpawn;
	float spawnCooldown;
	


};
