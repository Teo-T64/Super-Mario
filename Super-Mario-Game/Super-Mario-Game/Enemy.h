#pragma once
#include "Animation.h"
#include "Object.h"
#include "Physics.h"

class Enemy : public Object {
public:
    Enemy(const sf::Vector2f& pos);

    virtual void Begin() override;
    virtual void Update(float deltaTime) override;
    virtual void Render(Renderer& renderer) override;

    void Die();
    bool IsDead() const;
    b2BodyId GetBody() const { return body; }
private:
    Animation animation;
    float movement = 3.0f;  
    float destroyTimer = 0.0f;
    bool isDead = false;
    FixtureData fixtureData;
    b2BodyId body;
    


};