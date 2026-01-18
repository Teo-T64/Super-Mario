#pragma once
#include <SFML/Graphics.hpp>
#include "Renderer.h"
#include "object.h"
#include "Camera.h"

extern Camera camera;
extern bool paused;
extern size_t coinsInLvl;
extern std::vector<Object*> objects;

void Begin(const sf::Window& window);
void update(float dTime);
void Render(Renderer& renderer);
void RenderUI(Renderer& renderer);