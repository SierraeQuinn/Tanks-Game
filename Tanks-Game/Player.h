#pragma once
#include <SFML/Graphics.hpp>
// Forward Declaration
// Use this instead of an include
// in the .h only
// When using pointers to a class
// (also used to avoid .h files including eachother)
class LevelScreen;

class Player
{
	// Functionality

public:

	// Constructors
	Player(sf::Vector2f pos, LevelScreen* newLevel, sf::Texture* tankTexture, sf::Texture* gunTexture);

	// Public functions
	void DrawTo(sf::RenderTarget& target);
	void Update(float frameTime);
	void SetAngle(float newAngle);
	sf::FloatRect GetGlobalBounds() const;
	const sf::Sprite& GetSprite() const; 
	

private:

	// Private functions
	void Fire();
	void AngleChange(float deltaAngle);
	void StrengthChange(float deltaStrength);
	

	// Data

private:

	sf::Texture* baseTexture;
	sf::Texture* gunTexture;
	sf::Texture arrowTexture;

	sf::Sprite baseSprite;
	sf::Sprite gunSprite;
	sf::Sprite arrowSprite;

	float angle;
	float strength;
	float firingSpeed;
	float windPower;
	sf::Vector2f velocity;

	float fireCooldown;
	float timeSinceFire;

	LevelScreen* level;

	

};
