
#pragma once
#include "Renderer.h"
#include "box2d/box2d.h"

class Object
{
public:
	virtual ~Object() = default;

	std::string tag{};
	bool toDestroy = false;
	virtual void Begin() {}
	virtual void Update(float deltaTime) {}
	virtual void Render(Renderer& renderer) {}
};