#pragma once
#include "Bullet.h"

class ExplosiveBullet : public Bullet
{
public:
    ExplosiveBullet(sf::Texture& tex,
        float speed,
        float angle,
        float windPower,
        sf::Vector2f velocity,
        sf::Vector2f position,
        Player* owner);

    void Update(float frameTime) override;
    void DealDamage(std::vector<Player*>& players);
};
