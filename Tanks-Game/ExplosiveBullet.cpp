#include "ExplosiveBullet.h"
#include <cmath>

ExplosiveBullet::ExplosiveBullet(sf::Texture& tex,
    float speed,
    float angle,
    float windPower,
    sf::Vector2f velocity,
    sf::Vector2f position,
    Player* owner)
    : Bullet(tex, speed, angle, windPower, velocity, position, owner)
{
    // Explosive bullets do more damage (optional tweak)
    this->SetDamage(20); // You'll need a SetDamage() method in Bullet
}

void ExplosiveBullet::Update(float frameTime)
{
    Bullet::Update(frameTime);
    // Add explosive effects later if needed
}