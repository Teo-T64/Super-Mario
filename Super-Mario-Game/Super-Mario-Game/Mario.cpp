#include "Mario.h"
#include "Resources.h"


constexpr float M_PI =22.0f/7.0f;
const float movementSpeed = 7.0f;
const float jumpVelocity = 10.0f;
void Mario::Begin() {
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = { position.x, position.y };
    bodyDef.fixedRotation = true;
    body = b2CreateBody(Physics::world, &bodyDef);
    b2Polygon box = b2MakeBox(0.5f, 1.0f);
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.density = 1.0f;
    shapeDef.material.friction = 0.3f;
    shapeDef.userData = this;
    shapeDef.enableContactEvents = true;
    b2CreatePolygonShape(body, &shapeDef, &box);
    b2Circle foot;
    foot.radius = 0.2f;
    foot.center = { 0.0f, -0.9f };
    b2ShapeDef sensorDef = b2DefaultShapeDef();
    sensorDef.isSensor = true;
    sensorDef.userData = this;
    footSensorId = b2CreateCircleShape(body, &sensorDef, &foot);
}

void Mario::Update(float dTime) {

    float move = movementSpeed;
    b2Vec2 velocity = b2Body_GetLinearVelocity(body);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift))
        move *= 2.0f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
        velocity.x = move;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
        velocity.x = -move;
    else
        velocity.x = 0.0f;

    
    static bool jumpPressedLastFrame = false;
    bool jumpPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);

    if (jumpPressed && !jumpPressedLastFrame && IsGrounded())
    {
        velocity.y = -8.0f;
    }
    if (jumpPressed && !jumpPressedLastFrame && IsGrounded()) {
        velocity.y = -jumpVelocity;  
    }

    jumpPressedLastFrame = jumpPressed;

    b2Body_SetLinearVelocity(body, velocity);
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


void Mario::OnBeginContact() {
    groundContact = true;

}


void Mario::OnEndContact() {
    groundContact = false;

}