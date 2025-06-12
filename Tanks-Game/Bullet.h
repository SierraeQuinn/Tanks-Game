#pragma once
#include <SFML/Graphics.hpp>


class Bullet
{
	// Functionality

public:

	// Constructors
	Bullet(sf::Texture& bulletTex,
		float firingSpeed,
		float firingAngle,
		sf::Vector2f position);

	// Public Functions
	void DrawTo(sf::RenderTarget& target);
	void Update(float deltaTime);
	bool IsOnScreen() const;
	
	// Data
private:
	sf::Sprite sprite;
	sf::Vector2f velocity;
	float damage;

};

