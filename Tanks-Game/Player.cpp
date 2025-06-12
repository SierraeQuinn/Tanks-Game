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

void Player:: Update(float dt)
{

	if (!isActive) return;

	// Update cooldown timer
	timeSinceFire += dt;

	// Fire if space is pressed and cooldown has passed
	if (canFireThisTurn && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
	{
		if (timeSinceFire >= fireCooldown)
		{
			Fire();
			canFireThisTurn = false;
			timeSinceFire = 0.0f;
		}
	}

	const float ANGLE_SPEED = 90.0f;
	const float STRENGTH_SPEED = 1.0f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
	{
		// Decrease Angle
		AngleChange(-ANGLE_SPEED * dt);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
	{
		// Increase Angle
		AngleChange(ANGLE_SPEED * dt);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
	{
		// Increase Strength
		StrengthChange(STRENGTH_SPEED * dt);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
	{
		// Decrease Strength
		StrengthChange(-STRENGTH_SPEED * dt);
	}
}


void Player::Fire()
{
	// Block if player already fired this turn
	if (!canFireThisTurn)
		return;

	// 1) Get the sprite’s current rotation as an sf::Angle
	sf::Angle rot = gunSprite.getRotation();

	// 2) Derive a float barrel length
	float barrelLength = 40.f;
	if (gunTexture)
		barrelLength = static_cast<float>(gunTexture->getSize().x);

	// 3) Build a unit vector from the angle
	float rad = rot.asRadians();
	sf::Vector2f direction{ std::cos(rad), std::sin(rad) };

	// 4) Compute barrel‐tip world pos
	sf::Vector2f bulletPos = gunSprite.getPosition() + direction * barrelLength;

	// 5) Spawn using the *float* degrees for Bullet’s constructor
	float deg = rot.asDegrees();
	level->SpawnBullet(bulletPos, strength * firingSpeed, deg, this);

	// 🔒 Disallow further firing until reset
	canFireThisTurn = false;
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

void Player::SetActive(bool active)
{
	isActive = active;

	if (active)
		canFireThisTurn = true; // ✅ Reset fire permission on your turn
}

void Player::ResetFire() {
	canFireThisTurn = true;
}