#pragma once
#include "Renderer.h"
#include <box2d/box2d.h>

class MyDebugDraw;


class Physics {

public:
    static void Init();
    static void Update(float dTime);
    static void DebugDraw(Renderer& renderer);
    static MyDebugDraw* debugDraw;
    static b2WorldId world;

};

