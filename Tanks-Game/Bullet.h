#pragma once
#include <SFML/Graphics.hpp>
class Player; // Forward declaration

class Bullet {
public:
    Bullet(const sf::Texture& bulletTex,
        float firingSpeed,
        float firingAngle,
        float windPower,
        sf::Vector2f velocity,
        sf::Vector2f position,
        Player* owner);


    virtual ~Bullet() = default;

    virtual void Update(float frameTime);
    virtual void DrawTo(sf::RenderTarget& target);
    virtual void OnHit(Player* target); // <-- For effects
    virtual float GetDamage() const { return damage; }
    void SetDamage(float dmg) { damage = dmg; }

    bool IsOnScreen() const;
    sf::FloatRect GetGlobalBounds() const;
    Player* GetOwner() const;

protected: // <-- changed from private to protected
    sf::Sprite sprite;
    float windPower;
    Player* owner;
    float damage;
    sf::Vector2f velocity;
};

