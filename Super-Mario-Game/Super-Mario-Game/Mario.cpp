#include "Mario.h"
#include "Resources.h"

const float movementSpeed = 180.0f;

void Mario::Begin() {

}


void Mario::Update(float dTime) {
	float move = movementSpeed;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)) {
        move *= 2;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
        position.x += move * dTime;

    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
        position.x -= move * dTime;

    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
        position.y -= move * dTime;

    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
        position.y += move * dTime;

    }

}

void Mario::Draw(Renderer& renderer) {

    renderer.Draw(Resources::textures["mario.png"], position,
        sf::Vector2f(16.0f,32.0f)
    );

}