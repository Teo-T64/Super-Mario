#include "Mario.h"
#include "Resources.h"


constexpr float M_PI =22.0f/7.0f;
const float movementSpeed = 5.0f;
const float jumpVelocity = 8.0f;
void Mario::Begin() 
{
    runAnimation = Animation(0.6f,
        {
            AnimFrame{0.40f, Resources::textures["run3.png"]},
            AnimFrame{0.20f, Resources::textures["run2.png"]},
            AnimFrame{0.0f, Resources::textures["run1.png"]},
        });
   //jumpSound.setBuffer(Resources::sounds["jump.wav"]);
   // jumpSound.setVolume(20);
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
    runAnimation.Update(dTime);
       
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift))
        move *= 2.0f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
        velocity.x = move;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
        velocity.x = -move;
;

    
    static bool jumpPressedLastFrame = false;
    bool jumpPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);

    
    if (jumpPressed && !jumpPressedLastFrame && IsGrounded()) {
        velocity.y = -jumpVelocity;  
     //   jumpSound.play();
    }

    jumpPressedLastFrame = jumpPressed;
    textureToDraw = runAnimation.GetTexture();
    if (velocity.x < 0.0f)
        facingLeft = true;
    else if (velocity.x > 0.0f)
        facingLeft = false;
    else
        textureToDraw = Resources::textures["idle.png"];

    if (!IsGrounded())
       textureToDraw = Resources::textures["jump.png"];

    b2Body_SetLinearVelocity(body, velocity);
    b2Vec2 b2Pos = b2Body_GetPosition(body);
    position = sf::Vector2f(b2Pos.x, b2Pos.y);


    b2Rot rotation = b2Body_GetRotation(body);
    float radians = b2Rot_GetAngle(rotation);
    angle = radians * 180.0f / M_PI;

}

void Mario::Draw(Renderer& renderer) {
    renderer.Draw(textureToDraw, position,
        sf::Vector2f(facingLeft ? -1.0f : 1.0f, 2.0f),
        angle
    );
}


void Mario::OnBeginContact() {
    groundContact ++;

}


void Mario::OnEndContact() {
    if(groundContact>0)
        groundContact--;

}