#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Renderer.h"

class Map{
public:
	Map(float cellSize = 32.0f);
	void createCheckerBoard(size_t width, size_t height);
	void Draw(Renderer& renderer);
	sf::Vector2f CreateFromImg(const sf::Image& img);
	std::vector<std::vector<int>> grid;
	float cellSize;

};

