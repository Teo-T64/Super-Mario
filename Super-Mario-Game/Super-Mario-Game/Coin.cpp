#include "Coin.h"
#include "Resources.h"
#include "Physics.h"
#include <iostream>

Coin::~Coin() {
    if (b2Body_IsValid(this->body)) {
        b2ShapeId shape;
        int shapeCount = b2Body_GetShapes(this->body, &shape, 1);
        if (shapeCount > 0) {
            void* data = b2Shape_GetUserData(shape);
            if (data) delete static_cast<FixtureData*>(data);
        }

        b2DestroyBody(this->body);
        this->body = b2_nullBodyId;
    }
}
Coin::Coin(sf::Vector2f pos) {
    this->position = pos;
    this->tag = "coin";

    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_staticBody;
    bodyDef.position = { pos.x, pos.y };

    this->body = b2CreateBody(Physics::world, &bodyDef);

    b2Polygon box = b2MakeBox(0.5f, 0.5f);
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.isSensor = true;
    shapeDef.enableSensorEvents = true;

    FixtureData* fixtureData = new FixtureData();
    fixtureData->type = FixtureDataType::Object;
    fixtureData->object = this;
    fixtureData->listener = nullptr;
    shapeDef.userData = fixtureData;
    b2CreatePolygonShape(this->body, &shapeDef, &box);
}
void Coin::Begin()
{
    animation = Animation(1.4f,
        {
            AnimFrame(1.3f, Resources::textures["coin14.png"]),
            AnimFrame(1.2f, Resources::textures["coin13.png"]),
            AnimFrame(1.1f, Resources::textures["coin12.png"]),
            AnimFrame(1.0f, Resources::textures["coin11.png"]),
            AnimFrame(0.9f, Resources::textures["coin10.png"]),
            AnimFrame(0.8f, Resources::textures["coin9.png"]),
            AnimFrame(0.7f, Resources::textures["coin8.png"]),
            AnimFrame(0.6f, Resources::textures["coin7.png"]),
            AnimFrame(0.5f, Resources::textures["coin6.png"]),
            AnimFrame(0.4f, Resources::textures["coin5.png"]),
            AnimFrame(0.3f, Resources::textures["coin4.png"]),
            AnimFrame(0.2f, Resources::textures["coin3.png"]),
            AnimFrame(0.1f, Resources::textures["coin2.png"]),
            AnimFrame(0.0f, Resources::textures["coin1.png"]),
        });

}

void Coin::Update(float dTime)
{

    animation.Update(dTime);
}

void Coin::Render(Renderer& renderer)
{
    renderer.Draw(animation.GetTexture(), position, sf::Vector2f(0.8f, 0.8f));
}