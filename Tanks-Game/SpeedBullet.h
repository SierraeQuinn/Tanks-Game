#pragma once
#include "Bullet.h"

class SpeedBullet : public Bullet {
public:
    SpeedBullet(sf::Texture& texture,     // not const
        float speed,
        float angle,
        float windPower,
        sf::Vector2f velocity,
        sf::Vector2f position,
        Player* owner);

    void OnHit(Player* target) override;
};
