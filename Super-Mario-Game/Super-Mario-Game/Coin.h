#pragma once
#include "object.h"
#include "Animation.h"
#include "box2d/box2d.h"
class Coin :
    public Object
{
public: 
    ~Coin();
    Coin(sf::Vector2f pos);
    sf::Vector2f position;
    virtual void Begin() override;
    virtual void Update(float dime) override;

    virtual void Render(Renderer& renderer) override;
private: 
    Animation animation;
    b2BodyId body;
    
};

