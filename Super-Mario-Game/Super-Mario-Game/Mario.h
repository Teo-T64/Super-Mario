#pragma once

#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include "Renderer.h"
#include "Physics.h"
class Mario
	:public ContactListener
{
private:
	b2BodyId body;
	b2ShapeId footSensorId;
	int groundContact = 0;
public:

	void Begin();
	void Update(float dTime);
	void Draw(Renderer& renderer);
	void OnBeginContact(b2ShapeId otherShape)override;
	void OnEndContact(b2ShapeId otherShape) override;
	bool IsGrounded()const { return groundContact > 0; }
	sf::Vector2f position{};
	float angle{};

};

