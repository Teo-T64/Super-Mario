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
	sf::Texture textureToDraw;
	b2BodyId body;
	b2ShapeId footSensorId;
	size_t groundContact = 0;
	bool facingLeft = false;
public:
	
	void Begin();
	void Update(float dTime);
	void Draw(Renderer& renderer);
	virtual void OnBeginContact()override;
	virtual void OnEndContact() override;
	bool IsGrounded()const { return groundContact > 0; }
	sf::Vector2f position{};
	float angle{};
	

};

