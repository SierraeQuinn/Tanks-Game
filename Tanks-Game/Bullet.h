#pragma once
#include <SFML/Graphics.hpp>
class Player; // Forward declaration


class Bullet
{
	// Functionality

public:

	// Constructors
	Bullet(sf::Texture& bulletTex,
		float firingSpeed,
		float firingAngle,
		sf::Vector2f position, 
		Player* owner);
	    Player* GetOwner() const;

	// Public Functions
	void DrawTo(sf::RenderTarget& target);
	void Update(float deltaTime);
	bool IsOnScreen() const;
	sf::FloatRect GetGlobalBounds() const;


private:

	sf::Sprite sprite;
	sf::Vector2f velocity;
	float damage;
	Player* owner;  

};

