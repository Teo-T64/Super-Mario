#pragma once

#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include "Renderer.h"

class Mario{
private:
	b2BodyId body;

public:

	void Begin();
	void Update(float dTime);
	void Draw(Renderer& renderer);

	sf::Vector2f position{};
	float angle{};

};

