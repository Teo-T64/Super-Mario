#pragma once

#include <SFML/Graphics.hpp>
#include "Renderer.h"

class Mario{
public:

	void Begin();
	void Update(float dTime);
	void Draw(Renderer& renderer);

	sf::Vector2f position{};
	float angle{};

};

