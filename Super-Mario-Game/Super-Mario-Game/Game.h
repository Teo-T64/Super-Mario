#pragma once
#include <SFML/Graphics.hpp>
#include "Renderer.h"
#include "Camera.h"

extern Camera camera;

void Begin(const sf::Window& window);
void update(float dTime);
void Render(Renderer& renderer);