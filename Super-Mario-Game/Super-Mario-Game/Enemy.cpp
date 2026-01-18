#include "Enemy.h"
#include "Resources.h"
#include <box2d/box2d.h>
#include <iostream>

constexpr float M_PI = 3.14159265359f;

Enemy::Enemy(const sf::Vector2f& pos) {
    position = pos;
    body = b2_nullBodyId;  
    tag = "enemy";
}


void Enemy::Begin() {
    
    animation = Animation(0.5f,
        {
            AnimFrame(0.25f, Resources::textures["enemy2.png"]),
            AnimFrame(0.00f, Resources::textures["enemy1.png"]),
        });
   
    tag = "enemy";
    
    
    fixtureData.type = FixtureDataType::Object;
    fixtureData.object = this;
    fixtureData.listener = nullptr;

   
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = { position.x, position.y };
    bodyDef.fixedRotation = true;
    body = b2CreateBody(Physics::world, &bodyDef);

    
    b2Circle circle;
    circle.radius = 0.5f;
    circle.center = { 0.0f, 0.0f };

    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.density = 1.0f;
    shapeDef.material.friction = 0.0f;
    shapeDef.enableContactEvents = true;

    shapeDef.userData = &fixtureData;
    b2CreateCircleShape(body, &shapeDef, &circle);

    std::cout << "Enemy created at: (" << position.x << ", " << position.y << ")" << std::endl;
}

void Enemy::Update(float deltaTime) {
    if (isDead) {
        destroyTimer += deltaTime;

        if (destroyTimer >= 0.05f) {
            if (b2Body_IsValid(body)) {
                b2DestroyBody(body);
                body = b2_nullBodyId;
            }
        }
        return;
    }
    this->tag = "enemy";
  
    animation.Update(deltaTime);

    
    if (b2Body_IsValid(body) == false) return;


    b2Vec2 velocity = b2Body_GetLinearVelocity(body);

    
    if (std::abs(velocity.x) <= 0.1f) {
        movement *= -1.0f;
    }

    
    velocity.x = movement;
    velocity.y = 0.0f; 

    b2Body_SetLinearVelocity(body, velocity);

    
    b2Vec2 b2Pos = b2Body_GetPosition(body);
    position = sf::Vector2f(b2Pos.x, b2Pos.y);

   
    b2Rot rotation = b2Body_GetRotation(body);
    float radians = b2Rot_GetAngle(rotation);
    angle = radians * 180.0f / M_PI;
}

void Enemy::Render(Renderer& renderer) {
    if (isDead) {
        sf::Vector2f scale(1.0f, 0.1f);
        sf::Vector2f drawPos(position.x, position.y + 0.45f);
        renderer.Draw(animation.GetTexture(), drawPos, scale, angle);
    }
    else {
        renderer.Draw(animation.GetTexture(), position, sf::Vector2f(1.0f, 1.0f), angle);
    }
}

void Enemy::Die(){

        isDead = true;
        
}
bool Enemy::IsDead()const
{
    return isDead;
}