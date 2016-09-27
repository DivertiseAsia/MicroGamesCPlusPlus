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
    this->setAnchorPoint(Vec2(0.5,0.5));
    return true;
}

void Ball::moveNext(){
    //Update velocity (by acceleration)
    _velocity += _acceleration;
    
    if (_acceleration == Vec2::ZERO){
        assert(_friction.x >=0&& _friction.y>=0);
        //Slow down by friction
        if (_friction.y > 1)
            _velocity.y = (fabs(_velocity.y) < _friction.y-1) ? 0 : _velocity.y / _friction.y;
        
        if (_friction.x > 1)
            _velocity.x = (fabs(_velocity.x) < _friction.x-1) ? 0 : _velocity.x / _friction.x;
    
    }
    
    //Update Position
    auto current = this->getPosition();
    current += _velocity;
    this->setPosition(current);
    
    //if (this->getTag()==1)
    //    log("%i: My speed = %.0f", this->getTag(),_velocity.y);
}