#include "Physics.h"

b2WorldId Physics::world = b2_nullWorldId;

void Physics::Init() {
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = { 0.0f, -9.2f };

    Physics::world = b2CreateWorld(&worldDef);
}

void Physics::Update(float dTime) {

    int subStepCount = 4;
    b2World_Step(Physics::world, dTime, subStepCount);
}