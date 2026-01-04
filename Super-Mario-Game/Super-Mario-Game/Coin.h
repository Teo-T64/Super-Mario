#pragma once
#include "object.h"
#include "Animation.h"
class Coin :
    public Object
{
public: 
    sf::Vector2f position;
    virtual void Begin() override;
    virtual void Update(float dime) override;

    virtual void Render(Renderer& renderer) override;
private: 
    Animation animation;
    
};

