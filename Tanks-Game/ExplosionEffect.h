#pragma once
#include <SFML/Graphics.hpp>

class ExplosionEffect
{
public:
    ExplosionEffect(sf::Texture& texture, sf::Vector2f position);

    void Update(float deltaTime);
    void DrawTo(sf::RenderTarget& target);
    bool IsFinished() const;

private:
    sf::Sprite sprite;
    float lifetime;
    float elapsed;
};

