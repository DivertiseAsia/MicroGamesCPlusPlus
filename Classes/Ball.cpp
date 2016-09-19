//
//  Ball.cpp
//  microgames
//
//  Created by Work on 15/9/16.
//
//

#include "Ball.hpp"


Ball::Ball(float radius, Color4F color, Vec2 initialVelocity)
{
    _radius = radius;
    _color = color;
    _velocity = initialVelocity;
}

Ball::~Ball()
{
    //Release the memoery here if any.
}

bool Ball::init(){
    
    if (!DrawNode::init())
    {
        return false;
    }
    
    this->setContentSize(Size(_radius*2, _radius*2));
    this->drawSolidCircle(Vec2(_radius,_radius), _radius, 3.1415968f, 360, _color);    //Draw circle at 0,0
    
    
    auto rect = this->getBoundingBox();
    this->drawRect( rect.origin, rect.size, Color4F::RED); //For debugging propose
    this->setName("Ball");
    
    this->setAnchorPoint(Vec2(0.5,0.5));
    return true;
}

void Ball::moveNext(){
    auto current = this->getPosition();
    current += _velocity;
    this->setPosition(current);
}