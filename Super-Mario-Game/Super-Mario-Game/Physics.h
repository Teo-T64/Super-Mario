#pragma once
#include <box2d/box2d.h>

class Physics {
private:
    static b2WorldId world;

public:
    static void Init();
    static void Update(float dTime);

    static b2WorldId GetWorldId() { return world; }
};

