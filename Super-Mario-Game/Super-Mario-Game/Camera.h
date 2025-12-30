#pragma once
#include <SFML/Graphics.hpp>


class Camera {
public:
	Camera(float zoomLvl = 5.0f);
	sf::View GetView(sf::Vector2u windowSize);
	float zoomLvl;
	sf::Vector2f position;
};