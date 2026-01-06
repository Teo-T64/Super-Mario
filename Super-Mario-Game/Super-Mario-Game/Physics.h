#pragma once
#include <box2d/box2d.h>
#include "Renderer.h"

struct ContactListener {
    virtual void OnBeginContact(b2ShapeId self, b2ShapeId other) = 0;
    virtual void OnEndContact(b2ShapeId self, b2ShapeId other) = 0;
    virtual ~ContactListener() = default;
};

enum class FixtureDataType { Mario=0, MapTile=1, Object=2 };

class Mario;
class Object;
class MyDebugDraw;

struct FixtureData {
    ContactListener* listener = nullptr;
    FixtureDataType type;

    Mario* mario = nullptr;
    Object* object = nullptr;
    int mapX = 0;
    int mapY = 0;

    FixtureData() : listener(nullptr), type(FixtureDataType::MapTile), mario(nullptr) {}
};

class Physics {
public:
    static void Init();
    static void Update(float dTime);
    static void DebugDraw(Renderer& renderer);

    static MyDebugDraw* debugDraw;
    static b2WorldId world;
};