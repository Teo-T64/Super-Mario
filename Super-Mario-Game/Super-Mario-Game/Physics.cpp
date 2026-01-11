#include "Physics.h"
#include "Mario.h"
#include "object.h"
#include<SFML/Graphics.hpp>
#include "box2d/box2d.h"
#include "Game.h"

b2WorldId Physics::world = b2_nullWorldId;
MyDebugDraw* Physics::debugDraw{};

class MyDebugDraw {
public:
    sf::RenderTarget& target;
    MyDebugDraw(sf::RenderTarget& target) : target(target) {}

    static sf::Color MakeColor(b2HexColor color, float alphaMultiplier = 1.0f) {
        std::uint8_t r = static_cast<std::uint8_t>((color >> 16) & 0xFF);
        std::uint8_t g = static_cast<std::uint8_t>((color >> 8) & 0xFF);
        std::uint8_t b = static_cast<std::uint8_t>(color & 0xFF);
        return sf::Color(r, g, b, static_cast<std::uint8_t>(255 * alphaMultiplier));
    }

    static void DrawPolygon(const b2Vec2* vertices, int vertexCount, b2HexColor color, void* context) {
        auto* self = static_cast<MyDebugDraw*>(context);
        sf::ConvexShape shape(vertexCount);
        for (int i = 0; i < vertexCount; i++) {
            shape.setPoint(i, sf::Vector2f(vertices[i].x, vertices[i].y));
        }
        shape.setFillColor(sf::Color::Transparent);
        shape.setOutlineThickness(0.05f); 
        shape.setOutlineColor(MakeColor(color));
        self->target.draw(shape);
    }

    static void DrawSolidPolygon(b2Transform transform, const b2Vec2* vertices, int vertexCount, float radius, b2HexColor color, void* context) {
        auto* self = static_cast<MyDebugDraw*>(context);
        sf::ConvexShape shape(vertexCount);
        for (int i = 0; i < vertexCount; i++) {
            b2Vec2 wp = b2TransformPoint(transform, vertices[i]);
            shape.setPoint(i, sf::Vector2f(wp.x, wp.y));
        }
        shape.setFillColor(MakeColor(color, 0.5f));
        self->target.draw(shape);
    }

    static void DrawCircle(b2Vec2 center, float radius, b2HexColor color, void* context) {
        auto* self = static_cast<MyDebugDraw*>(context);
        sf::CircleShape circle(radius);
        circle.setOrigin(sf::Vector2f(radius, radius));
        circle.setPosition(sf::Vector2f(center.x, center.y));
        circle.setFillColor(sf::Color::Transparent);
        circle.setOutlineThickness(0.05f);
        circle.setOutlineColor(MakeColor(color));
        self->target.draw(circle);
    }

    static void DrawSolidCircle(b2Transform transform, float radius, b2HexColor color, void* context) {
        auto* self = static_cast<MyDebugDraw*>(context);
        sf::CircleShape circle(radius);
        circle.setOrigin(sf::Vector2f(radius, radius));
        circle.setPosition(sf::Vector2f(transform.p.x, transform.p.y));
        circle.setFillColor(MakeColor(color, 0.5f));
        self->target.draw(circle);

        b2Vec2 axis = b2RotateVector(transform.q, { radius, 0.0f });
        DrawSegment(transform.p, { transform.p.x + axis.x, transform.p.y + axis.y }, color, context);
    }

    static void DrawSegment(b2Vec2 p1, b2Vec2 p2, b2HexColor color, void* context) {
        auto* self = static_cast<MyDebugDraw*>(context);
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(p1.x, p1.y), MakeColor(color)),
            sf::Vertex(sf::Vector2f(p2.x, p2.y), MakeColor(color))
        };
        self->target.draw(line, 2, sf::PrimitiveType::Lines);
    }

    static void DrawTransform(b2Transform transform, void* context) {
        float axisScale = 0.5f;
        b2Vec2 rx = b2RotateVector(transform.q, { axisScale, 0.0f });
        DrawSegment(transform.p, { transform.p.x + rx.x, transform.p.y + rx.y }, static_cast<b2HexColor>(0xFF0000), context); 

        b2Vec2 ry = b2RotateVector(transform.q, { 0.0f, axisScale });
        DrawSegment(transform.p, { transform.p.x + ry.x, transform.p.y + ry.y }, static_cast<b2HexColor>(0x00FF00), context); 
    }
};

class MyGlobalContactListener {
public:
    static void BeginContact(b2ShapeId shapeIdA, b2ShapeId shapeIdB) {
        FixtureData* dataA = (FixtureData*)b2Shape_GetUserData(shapeIdA);
        FixtureData* dataB = (FixtureData*)b2Shape_GetUserData(shapeIdB);

        if (dataA && dataA->listener) {
            dataA->listener->OnBeginContact(shapeIdA, shapeIdB);
        }

        if (dataB && dataB->listener) {
            dataB->listener->OnBeginContact(shapeIdB, shapeIdA);
        }
    }

    static void EndContact(b2ShapeId shapeIdA, b2ShapeId shapeIdB) {
        FixtureData* dataA = (FixtureData*)b2Shape_GetUserData(shapeIdA);
        FixtureData* dataB = (FixtureData*)b2Shape_GetUserData(shapeIdB);

        if (dataA && dataA->listener) {
            dataA->listener->OnEndContact(shapeIdA, shapeIdB);
        }

        if (dataB && dataB->listener) {
            dataB->listener->OnEndContact(shapeIdB, shapeIdA);
        }
    }
};


void Physics::Init() {
    b2WorldDef worldDef = b2DefaultWorldDef();

    worldDef.gravity = { 0.0f, 9.2f };

    Physics::world = b2CreateWorld(&worldDef);

    
    
}
static float accumulator = 0.0f;

void Physics::Update(float dTime) {
    const float timeStep = 1.0f / 60.0f;
    accumulator += dTime;

    while (accumulator >= timeStep) {
        b2World_Step(world, timeStep, 4);

        b2ContactEvents contactEvents = b2World_GetContactEvents(world);
        for (int i = 0; i < contactEvents.beginCount; ++i) {
            auto& e = contactEvents.beginEvents[i];
            MyGlobalContactListener::BeginContact(e.shapeIdA, e.shapeIdB);
            printf("Solid Contact Detected!\n");
        }
        for (int i = 0; i < contactEvents.endCount; ++i) {
            auto& e = contactEvents.endEvents[i];
            MyGlobalContactListener::EndContact(e.shapeIdA, e.shapeIdB);
        }

        b2SensorEvents sensorEvents = b2World_GetSensorEvents(world);
        for (int i = 0; i < sensorEvents.beginCount; ++i) {
            auto& e = sensorEvents.beginEvents[i];
            MyGlobalContactListener::BeginContact(e.sensorShapeId, e.visitorShapeId);
            printf("Sensor overlap detected!\n");
        }
        for (int i = 0; i < sensorEvents.endCount; ++i) {
            auto& e = sensorEvents.endEvents[i];
            MyGlobalContactListener::EndContact(e.sensorShapeId, e.visitorShapeId);
        }

        accumulator -= timeStep;
    }
}
void Physics::DebugDraw(Renderer& renderer) {

    MyDebugDraw myDrawer(renderer.target);

    b2DebugDraw drawInterface = b2DefaultDebugDraw();

    drawInterface.DrawPolygonFcn = MyDebugDraw::DrawPolygon;
    drawInterface.DrawSolidPolygonFcn = MyDebugDraw::DrawSolidPolygon;
    drawInterface.DrawCircleFcn = MyDebugDraw::DrawCircle;
    drawInterface.DrawSolidCircleFcn = MyDebugDraw::DrawSolidCircle;
    drawInterface.DrawSegmentFcn = MyDebugDraw::DrawSegment;
    drawInterface.DrawTransformFcn = MyDebugDraw::DrawTransform;

    drawInterface.context = &myDrawer;
    drawInterface.drawShapes = true;
    drawInterface.drawJoints = true;
    drawInterface.drawMass = true;

    b2World_Draw(Physics::world, &drawInterface);
}