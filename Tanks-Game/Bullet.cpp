#include "Bullet.h"
#include <cmath>

static constexpr float PI = 3.14159265358979323846f;

Bullet::Bullet(sf::Texture& bulletTex,
    float firingSpeed,
    float firingAngle,
    sf::Vector2f position)
    : sprite(bulletTex)
    , velocity({ 0,0 })
    , damage(10)
{
    // center via .size (SFML 2.6+)
    sf::Vector2f size = sprite.getGlobalBounds().size;
    sprite.setOrigin(size * 0.5f);

    // position and rotate; setRotation takes sf::Angle
    sprite.setPosition(position);
    sprite.setRotation(sf::degrees(firingAngle));

    // calculate movement vector: convert deg?rad
    float rad = firingAngle * PI / 180.f;
    velocity.x = std::cos(rad) * firingSpeed;
    velocity.y = std::sin(rad) * firingSpeed;  // down is positive in SFML
}

void Bullet::DrawTo(sf::RenderTarget& target)
{

	target.draw(sprite);
}

void Bullet::Update(float deltaTime)
{

    const float gravity = 200.0f; // You can tweak this for realism

    // Apply gravity to vertical velocity
    velocity.y += gravity * deltaTime;

    // Move the bullet by its current velocity
    sprite.move(velocity * deltaTime);
}

bool Bullet::IsOnScreen() const
{
	sf::Vector2f pos = sprite.getPosition();
	return pos.x >= 0 && pos.x <= 1920 && pos.y >= 0 && pos.y <= 1080; // Update if your window size is different
}


