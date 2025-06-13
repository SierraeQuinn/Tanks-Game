#include "Bullet.h"
#include "Player.h"

#include <cmath>

static constexpr float PI = 3.14159265358979323846f;

Bullet::Bullet(sf::Texture& bulletTex,
    float firingSpeed,
    float firingAngle,
    float windPower,
    sf::Vector2f velocity,
    sf::Vector2f position,
    Player* owner) // this is the parameter
    : sprite(bulletTex),
    velocity(velocity),
    damage(10),
    owner(owner),
    windPower(windPower)// this assigns the parameter to the class member
{
    sf::Vector2f size = sprite.getGlobalBounds().size;
    sprite.setOrigin(size * 0.5f);
    sprite.setPosition(position);
    sprite.setRotation(sf::degrees(firingAngle));

 
}

Player* Bullet::GetOwner() const
{
    return owner;
}

void Bullet::DrawTo(sf::RenderTarget& target)
{

	target.draw(sprite);
}

void Bullet::Update(float frameTime)
{

    float gravity = 100.f;
    velocity.y += gravity * frameTime;
    velocity.x += windPower * frameTime;
    sprite.move(velocity * frameTime);

    float angle = std::atan2(velocity.y, velocity.x) * 180.0f / 3.14159265f;
    sprite.setRotation(sf::degrees(angle));
}

bool Bullet::IsOnScreen() const
{
	sf::Vector2f pos = sprite.getPosition();
	return pos.x >= 0 && pos.x <= 1920 && pos.y >= 0 && pos.y <= 1080; // Update if your window size is different
}

sf::FloatRect Bullet::GetGlobalBounds() const 
{
    return sprite.getGlobalBounds();  // This returns sf::FloatRect
}

void Bullet::OnHit(Player* target) {
    // Default bullet deals simple damage
    if (target)
        target->ModifyHealth(-static_cast<int>(damage));
}


