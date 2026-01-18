#include "Mario.h"
#include "object.h"
#include "Resources.h"
#include "Game.h"
#include <iostream>
#include <box2d/box2d.h>
#include "Enemy.h"
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
    fixtureData.listener = this;
    fixtureData.mario = this;
    fixtureData.type = FixtureDataType::Mario;

    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = { position.x, position.y };
    bodyDef.fixedRotation = true;
    body = b2CreateBody(Physics::world, &bodyDef);

    b2Polygon box = b2MakeBox(0.35f, 0.75f);
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.userData = &fixtureData;
    shapeDef.enableContactEvents = true;
    b2CreatePolygonShape(body, &shapeDef, &box);
    b2Circle foot;
    foot.radius = 0.3f;

    foot.center = { 0.0f, 0.80f };

    b2ShapeDef sensorDef = b2DefaultShapeDef();
    sensorDef.isSensor = true;
    sensorDef.userData = &fixtureData;
    sensorDef.enableSensorEvents = true;

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
    //std::cout << "Grounded: " << groundContact << " | IsGrounded(): " << (IsGrounded() ? "YES" : "NO") << std::endl;
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
void Mario::OnBeginContact(b2ShapeId self, b2ShapeId other) {
    void* userPtr = b2Shape_GetUserData(other);
    if (!userPtr) return;

    FixtureData* data = static_cast<FixtureData*>(userPtr);


    if (data->type == FixtureDataType::Object) {
        if (data->object->tag == "coin" && !data->object->toDestroy) {
            data->object->toDestroy = true;
            b2Filter filter = { 0 };
            b2Shape_SetFilter(other, filter);
            this->coins++;
            std::cout << "Collected Coin! Total: " << this->coins << std::endl;
        }
    }


    if (data->type == FixtureDataType::Object && data->object->tag == "enemy") {

        Enemy* enemy = dynamic_cast<Enemy*>(data->object);
        if (!enemy || enemy->IsDead()) return;

        bool isFootSensor = B2_ID_EQUALS(self, footSensorId);

        b2Vec2 marioPos = b2Body_GetPosition(body);
        b2Vec2 enemyPos = b2Body_GetPosition(enemy->GetBody());


        if (isFootSensor || (marioPos.y < enemyPos.y - 0.5f)) {
            enemy->Die();

            b2Vec2 vel = b2Body_GetLinearVelocity(body);
            vel.y = -jumpVelocity * 0.8f;
            b2Body_SetLinearVelocity(body, vel);

            std::cout << "SUCCESS: Stomped from above!" << std::endl;
            return;
        }else if (!enemy->IsDead()) {
            std::cout << "Mario hit enemy from side! Mario died." << std::endl;
            isDead = true;

            b2Vec2 velocity = b2Body_GetLinearVelocity(body);
            b2Vec2 enemyPos = b2Body_GetPosition(enemy->GetBody());
            b2Vec2 marioPos = b2Body_GetPosition(body);

            float direction = (marioPos.x < enemyPos.x) ? -1.0f : 1.0f;
            velocity.x = direction * 4.0f;
            velocity.y = -3.0f;
            b2Body_SetLinearVelocity(body, velocity);
        }
    }

    if (data->type == FixtureDataType::MapTile) {
        groundContact++;
    }
}

void Mario::OnEndContact(b2ShapeId self, b2ShapeId other) {
    FixtureData* data = static_cast<FixtureData*>(b2Shape_GetUserData(other));

    if (!data) return;

    if (data && data->type == FixtureDataType::MapTile) {
        if (self.index1 == footSensorId.index1 || self.index1 == body.index1) {
            groundContact--;
            if (groundContact < 0) groundContact = 0;
        }
    }

}