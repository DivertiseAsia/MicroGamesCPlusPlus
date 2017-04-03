//
//  Ball.cpp
//  microgames
//
//  Created by Work on 15/9/16.
//
//

#include "Ball.hpp"


Ball::Ball(float radius, Vec2 initialVelocity)
{
    _radius = radius;
    _velocity = initialVelocity;
}

Ball::~Ball()
{
    //Release the memoery here if any.
}

Ball* Ball::create(float radius){
    Ball *pRet = new(std::nothrow)Ball(radius, Vec2(0,0));
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

bool Ball::init(){
    
    if (!DrawNode::init())
    {
        return false;
    }
    
    this->setContentSize(Size(_radius*2, _radius*2));
    
    
    auto rect = this->getBoundingBox();
//#define DEBUG_MODE true
#ifdef DEBUG_MODE
    this->drawRect(rect.origin, rect.origin + rect.size, Color4F::RED); //For debugging propose
#endif

    this->setName("Ball");
    this->setAnchorPoint(Vec2(0, 0));
    
    return true;
}

void Ball::moveNext(){
    if (_velocity.getLength() == 0 && _acceleration.getLength() == 0 &&_moved){
        EventCustom event("ball_stop");
        event.setUserData(this);
        _eventDispatcher->dispatchEvent(&event);
    }
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
    
    EventCustom event("ball_move");
    event.setUserData(this);
    _eventDispatcher->dispatchEvent(&event);
    
    if (_velocity.getLength() != 0)
        _moved = true;
}

void Ball::setBallImage(std::string fname)
{
	auto ballImg = Sprite::create(fname);
	ballImg->setScale(0.5);
	this->addChild(ballImg);
}

void Ball::setBallImage(std::string fname, Rect area)
{
	auto ballImg = Sprite::create(fname, area);
	ballImg->setScale(0.5);
	this->addChild(ballImg);
}
