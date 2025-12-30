#include "Renderer.h"

Renderer::Renderer(sf::RenderTarget& target):target(target) {}

void Renderer::Draw(const sf::Texture& texture, const sf::Vector2f& pos, const sf::Vector2f& size) {
    if (!sprite) {
        sprite.emplace(texture);
    }

    sf::Vector2f texSize = static_cast<sf::Vector2f>(texture.getSize());

    sprite->setOrigin(texSize / 2.0f);
    sprite->setTexture(texture,true);
    sprite->setPosition(pos);

    sprite->setScale({ size.x / texSize.x, size.y / texSize.y });

    target.draw(*sprite);
}
