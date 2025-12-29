#pragma once
#include <SFML/Graphics.hpp>
#include <optional>

class Renderer {
private:
	std::optional<sf::Sprite> sprite;
	sf::RenderTarget& target;
public:
	Renderer(sf::RenderTarget& target);

	void Draw(const sf::Texture& texture,const sf::Vector2f& pos,const sf::Vector2f& size);


};