//
//  Ball.cpp
//  microgames
//
//  Created by Work on 15/9/16.
//
//

#include "Ball.hpp"

#define BALL_RADIUS 50

Ball::Ball()
{
    _radius = BALL_RADIUS;
    _color = Color4F(Color4F::BLUE);
    _velocity = Vec2(5,5);
}

Ball::~Ball()
{
    
}

bool Ball::init(){
    
    if (!DrawNode::init())
    {
        return false;
    }
    
    this->setContentSize(Size(_radius*2, _radius*2));
    this->drawSolidCircle(Vec2(0,0), _radius, 3.1415968f, 360, _color);    //Draw circle at 0,0
    
    return true;
}

void Ball::moveNext(){
    auto current = this->getPosition();
    current += _velocity;
    this->setPosition(current);
}