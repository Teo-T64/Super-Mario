#include "Mario.h"
#include "Resources.h"
#include "Physics.h"

constexpr float M_PI =22.0f/7.0f;
const float movementSpeed = 5.0f;

void Mario::Begin() {
    b2BodyDef bodyDef = b2DefaultBodyDef();

    bodyDef.type = b2_dynamicBody;

    bodyDef.position = { position.x, position.y };


    body = b2CreateBody(Physics::world, &bodyDef);

    b2Polygon box = b2MakeBox(0.5f, 1.0f);

    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.density = 1.0f;
    shapeDef.material.friction = 0.3f;

    b2CreatePolygonShape(body, &shapeDef, &box);


}
void Mario::Update(float dTime) {

    float move = movementSpeed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)) move *= 2;
    b2Vec2 b2Pos = b2Body_GetPosition(body);
    position = sf::Vector2f(b2Pos.x, b2Pos.y);


    b2Rot rotation = b2Body_GetRotation(body);
    float radians = b2Rot_GetAngle(rotation);
    angle = radians * 180.0f / M_PI;

}

void Mario::Draw(Renderer& renderer) {

    renderer.Draw(Resources::textures["mario.png"], position,
        sf::Vector2f(1.0f, 2.0f), angle);
}