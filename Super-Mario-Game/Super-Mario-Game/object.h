
#pragma once
#include "Renderer.h"
#include "box2d/box2d.h"
#include <string.h>
#include <iostream>
class Object
{
public:
	virtual ~Object() = default;

	std::string tag{};
	bool toDestroy = false;
	virtual void Begin() {}
	virtual void Update(float deltaTime) {}
	virtual void Render(Renderer& renderer) {}
	sf::Vector2f position{};
	float angle = 0.0f;


};