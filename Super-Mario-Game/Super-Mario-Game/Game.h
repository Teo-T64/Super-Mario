#pragma once
#include <SFML/Graphics.hpp>
#include "Renderer.h"

void Begin(const sf::Window& window);
void update(float dTime);
void render(Renderer& renderer);