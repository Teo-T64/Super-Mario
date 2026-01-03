#pragma once
#include "Renderer.h"
#include <box2d/box2d.h>

class MyDebugDraw;
struct ContactListener
{
    virtual void OnBeginContact() = 0;
    virtual void OnEndContact() = 0;
    virtual ~ContactListener() = default;
};

class Physics {

public:
    static void Init();
    static void Update(float dTime);
    static void DebugDraw(Renderer& renderer);
    static MyDebugDraw* debugDraw;
    static b2WorldId world;
};

