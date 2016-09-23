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

Ball* Ball::create(float radius, Color4F color){
    Ball *pRet = new(std::nothrow)Ball(radius, color, Vec2(0,0));
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

Ball* Ball::create(Color4F color){
    return Ball::create(DEFAULT_BALL_RADIUS, color);
}

bool Ball::init(){
    
    if (!DrawNode::init())
    {
        return false;
    }
    
    this->setContentSize(Size(_radius*2, _radius*2));
    this->drawSolidCircle(Vec2(_radius,_radius), _radius, 3.1415968f, 360, _color);    //Draw circle at 0,0
    
    
    auto rect = this->getBoundingBox();
    this->drawRect( rect.origin, rect.origin+rect.size, Color4F::RED); //For debugging propose
    this->setName("Ball");
    
    this->drawDot(Vec2::ZERO, 10, Color4F::RED);
    this->setAnchorPoint(Vec2(0.5,0.5));
    this->drawDot(Vec2::ZERO, 5, Color4F::GREEN);
    return true;
}

void Ball::moveNext(){
    auto current = this->getPosition();
    current += _velocity;
    this->setPosition(current);
}