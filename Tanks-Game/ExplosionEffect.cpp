#include "ExplosionEffect.h"

ExplosionEffect::ExplosionEffect(sf::Texture& texture, sf::Vector2f position)
    : sprite(texture), lifetime(0.5f), elapsed(0.f)
{
    sprite.setTexture(texture);
    sprite.setPosition(position);
    sprite.setOrigin(sf::Vector2f(16.f, 16.f));
    sprite.setScale(sf::Vector2f(0.5f, 0.5f));
}

void ExplosionEffect::Update(float deltaTime)
{
    elapsed += deltaTime;
}

void ExplosionEffect::DrawTo(sf::RenderTarget& target)
{
    if (!IsFinished())
        target.draw(sprite);
}

bool ExplosionEffect::IsFinished() const
{
    return elapsed >= lifetime;
}