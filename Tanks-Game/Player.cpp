#include "Player.h"
#include "LevelScreen.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>



Player::Player(sf::Vector2f pos, LevelScreen* newLevel, sf::Texture* tankTexture, sf::Texture* gunTexture)
	: arrowTexture("Assets/tank_arrowFull.png")
	, baseSprite(*tankTexture)
	, gunSprite(*gunTexture)
	, arrowSprite(arrowTexture)
	, angle(90.0f)
	, strength(1.0f)
	, firingSpeed(500.0f)
	, fireCooldown(0.2f)
	, timeSinceFire(0.2f)
	, level(newLevel)
{
	baseSprite.setOrigin(baseSprite.getGlobalBounds().size * 0.5f);
	baseSprite.setPosition(pos);
	gunSprite.setOrigin({0, static_cast<float>( gunTexture->getSize().y/2)});
	gunSprite.setPosition(pos - sf::Vector2f(0,15));
	gunSprite.setRotation(sf::degrees(-0.0f));
	arrowSprite.setOrigin({ -100, arrowSprite.getGlobalBounds().size.y /2 });
	arrowSprite.setPosition(pos);
	arrowSprite.setRotation(sf::degrees(0.0f));
}

void Player::DrawTo(sf::RenderTarget& target)
{
	target.draw(gunSprite);
	target.draw(baseSprite);
	target.draw(arrowSprite);
}

void Player::Update(float frameTime)
{
	const float ANGLE_SPEED = 90.0f;
	const float STRENGTH_SPEED = 1.0f;

	timeSinceFire += frameTime;

	// Input handling
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)
		&& timeSinceFire >= fireCooldown)
	{
		// Reset the time since firing
		timeSinceFire = 0.0f;

		// Fire ze lazers!
		Fire();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
	{
		// Decrease Angle
		AngleChange(-ANGLE_SPEED * frameTime);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
	{
		// Increase Angle
		AngleChange(ANGLE_SPEED * frameTime);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
	{
		// Increase Strength
		StrengthChange(STRENGTH_SPEED * frameTime);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
	{
		// Decrease Strength
		StrengthChange(-STRENGTH_SPEED * frameTime);
	}
}

void Player::Fire()
{
	float radians = (angle - 90.0f) * 3.14159265f / 180.0f;
	velocity.x = std::cos(radians) * strength * firingSpeed;
	velocity.y = std::sin(radians) * strength * firingSpeed;

	// This should come from level, randomise it at the start of a players turn
	windPower = static_cast<float>(rand() % 200 - 100);

	level->SpawnBullet(gunSprite.getPosition(), strength * firingSpeed * 1000, (angle - 90.0f), level->GetWindPower(), velocity, this);
}

void Player::AngleChange(float deltaAngle)
{
	angle += deltaAngle;
	const float LOW_LIMIT = -90.0f;
	const float UP_LIMIT = 90.0f;
	if (angle < LOW_LIMIT) angle = LOW_LIMIT;
	if (angle > UP_LIMIT) angle = UP_LIMIT;

	// Update sprites
	gunSprite.setRotation(sf::degrees(angle - 90.0f));
	arrowSprite.setRotation(sf::degrees(angle - 90.0f));

	// Calculate velocity based on angle (convert to radians for trig functions)
	float radians = angle * 3.14159265f / 180.0f;
	velocity.x = std::cos(radians) * strength * firingSpeed;
	velocity.y = std::sin(radians) * strength * firingSpeed;
}

void Player::StrengthChange(float deltaStrength)
{
	strength += deltaStrength;
	const float LOW_LIMIT = 0.75f;
	const float UP_LIMIT = 1.5f;
	if (strength < LOW_LIMIT) strength = LOW_LIMIT;
	if (strength > UP_LIMIT) strength = UP_LIMIT;

	// Update sprites
	arrowSprite.setScale({ strength, strength });

}
void Player::SetAngle(float newAngle)
{
	angle = newAngle;

	// Re-apply exactly the same rotation math you do in the ctor:
	gunSprite.setRotation(sf::degrees(angle - 90.0f));
	arrowSprite.setRotation(sf::degrees(angle - 90.0f));
}

sf::FloatRect Player::GetGlobalBounds() const 
{
	return baseSprite.getGlobalBounds();
}

void Player::ModifyHealth(int delta) 
{
	health += delta;
	if (health < 0) health = 0;
}
int Player::GetHealth() const {
	return health;
}
void Player::TakeDamage(int damage)
{
	health -= damage;
	if (health < 0)
		health = 0;
}

void Player::SetHealth(int hp)
{
	health = hp;
}

void Player::SetPosition(sf::Vector2f newPos)
{
	baseSprite.setPosition(newPos);
	gunSprite.setPosition(newPos); // optional: if your gun follows the base
}