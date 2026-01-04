
#pragma once
#include "Renderer.h"

class Object
{
public:
	virtual ~Object() = default;

	virtual void Begin() {}
	virtual void Update(float deltaTime) {}
	virtual void Render(Renderer& renderer) {}
};