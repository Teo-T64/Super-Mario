#pragma once

#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include "Renderer.h"
#include "Physics.h"
#include "Animation.h"
#include <SFML/Audio.hpp>

class Mario
	:public ContactListener
{
private:
	//sf::Sound jumpSound;
	Animation runAnimation;
	FixtureData fixtureData{};
	sf::Texture textureToDraw;
	b2BodyId body;
	b2ShapeId footSensorId;
	//b2ShapeId mainShapeId;
	size_t groundContact = 0;
	bool facingLeft = false;
	size_t coins{};
public:
	
	void Begin();
	void Update(float dTime);
	void Draw(Renderer& renderer);
	void OnBeginContact(b2ShapeId self, b2ShapeId other)override;
	void OnEndContact(b2ShapeId self, b2ShapeId other) override;
	bool IsGrounded()const { return groundContact > 0; }
	sf::Vector2f position{};
	float angle{};
	

};

