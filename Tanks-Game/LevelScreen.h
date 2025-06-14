#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Bullet.h"
#include "ExplosionEffect.h"


enum class AmmoType
{
	Normal,
	Explosive,
	Speed
	
};

class LevelScreen
{


public:

	// Constructors
	LevelScreen(sf::Vector2f screenSize);
	~LevelScreen();

	// Public Functions
	void DrawTo(sf::RenderTarget& target);
	void Update(float frameTime);
	void GenerateRandomWind();

	Bullet* SpawnBullet(sf::Vector2f pos,
		float speed,
		float angle,
		float windPower,
		sf::Vector2f velocity,
		Player* owner);
	void SwitchTurn();
	void RestartGame();
	float GetWindPower() const { return windPower; }
	void UpdateAmmoTypeText();


private:

	Player* myPlayer;
	Player* myPlayer2;
	int player = 0;
	std::vector<Bullet*> bullets;
	sf::Texture bulletTex;
	sf::Texture explosiveBulletTex;
	sf::Texture speedBulletTex;
	sf::Texture compassTexture;
	sf::Sprite compassSprite;
	sf::RectangleShape ground;
	sf::Texture explosionTexture;
	std::vector<ExplosionEffect> explosions;
	

	//---
	sf::Font uiFont;
	sf::Text player1healthText;
	sf::Text player2healthText;
	sf::Text windText;
	std::string windDirectionStr;
	float windPower = 100.0f;
	bool prev1 = false;
	bool prev2 = false;
	bool prev3 = false;
	bool gameOver = false;
	bool wasTabPressedLastFrame = false;
	sf::Text winnerText;
	sf::Text restartText;
	sf::Text turnText;
	sf::Text player1AmmoText;
	sf::Text player2AmmoText;
	sf::Vector2f startingPos1;
	sf::Vector2f startingPos2;
	
	

	
	sf::Vector2f screenSize;
	//---

	sf::Texture tankTextures[2];
	sf::Texture gunTexture;

	bool ismyPlayerTurn = true;
	float timeSinceSpawn;
	float spawnCooldown;
	AmmoType player1AmmoType = AmmoType::Normal;
	AmmoType player2AmmoType = AmmoType::Normal;
	


};


