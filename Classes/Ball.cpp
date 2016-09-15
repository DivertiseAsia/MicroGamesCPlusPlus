//
//  Ball.cpp
//  microgames
//
//  Created by Work on 15/9/16.
//
//

#include "Ball.hpp"

#define BALL_RADIUS 100

Ball::Ball()
{
}

Ball::~Ball()
{
    
}

bool Ball::init(){
    
    if (!DrawNode::init())
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    this->setContentSize(Size(BALL_RADIUS*2, BALL_RADIUS*2));
    this->drawSolidCircle(Vec2(0,0), 100, 3.14, 360, Color4F::BLUE);
    

    return true;
}

