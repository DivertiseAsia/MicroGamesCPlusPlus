//
//  BallSprite.hpp
//  microgames
//
//  Created by Work on 15/9/16.
//
//

#ifndef Ball_hpp
#define Ball_hpp

#include <stdio.h>
#include "cocos2d.h"

USING_NS_CC; //Alias to declare using cocos namespace

#define DEFAULT_BALL_RADIUS 20
#define DEFAULT_BALL_COLOR Color4F::BLUE
#define DEFAULT_BALL_INITIAL_VELOCITY Vec2(0,0)

class Ball : public DrawNode
{
public:
    Ball(float radius=DEFAULT_BALL_RADIUS, Color4F color=DEFAULT_BALL_COLOR, Vec2 initialVelocity=DEFAULT_BALL_INITIAL_VELOCITY);
    ~Ball();
    
    static Ball* create(float radius=DEFAULT_BALL_RADIUS, Color4F color=DEFAULT_BALL_COLOR);
    static Ball* create(Color4F color);
    
    virtual bool init();
    
    void moveNext();
    
    CC_SYNTHESIZE(Vec2, _velocity, Velocity); //create getter and setter
    
private:
    float _radius;
    Color4F _color;
    
};

#endif /* Ball_hpp */
