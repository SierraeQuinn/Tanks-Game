#include "SpeedBullet.h"
#include "Player.h"
#include <cmath>

SpeedBullet::SpeedBullet(sf::Texture& bulletTex,
    float firingSpeed,
    float firingAngle,
    float windPower,
    sf::Vector2f velocity,
    sf::Vector2f position,
    Player* owner)
    : Bullet(bulletTex, firingSpeed, firingAngle, windPower, velocity, position, owner)
{
}

void SpeedBullet::OnHit(Player* target)
{
    float speedMagnitude = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
    int damage = static_cast<int>(speedMagnitude * 0.1f); // scale as needed

    target->TakeDamage(damage);
}
