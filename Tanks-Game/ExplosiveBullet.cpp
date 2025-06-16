#include "ExplosiveBullet.h"
#include "Player.h"
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
    this->SetDamage(50); // Direct hit damage, optional
}

void ExplosiveBullet::Update(float frameTime)
{
    Bullet::Update(frameTime);
    // Optional: particle effects or explosion animation here
}

void ExplosiveBullet::DealDamage(std::vector<Player*>& players)
{
    const float aoe = 500.0f;
    const float MaxDamage = 80.0f;

    sf::Vector2f centre = GetPosition();
    for (Player* myPlayer1 : players)
    {
        sf::FloatRect bounds = myPlayer1->GetGlobalBounds();
        sf::Vector2f playerCenter(bounds.left + bounds.width / 2.0f,
            bounds.top + bounds.height / 2.0f);

        float dx = centre.x - playerCenter.x;
        float dy = centre.y - playerCenter.y;
        float distance = std::sqrt(dx * dx + dy * dy);

        if (distance <= aoe)
        {
            float damageFactor = 1.0f - (distance / aoe);
            int finalDamage = static_cast<int>(MaxDamage * damageFactor);
            myPlayer1->ModifyHealth(-finalDamage);
        }
    }
}
