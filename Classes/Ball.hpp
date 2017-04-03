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

class Ball : public Node
{
public:
    Ball(float radius=DEFAULT_BALL_RADIUS, Vec2 initialVelocity=DEFAULT_BALL_INITIAL_VELOCITY);
    ~Ball();
    
    static Ball* create(float radius=DEFAULT_BALL_RADIUS);
    
    virtual bool init();
    
    void addMoveListener();
    void addStopListener();
    
    void moveNext();

	// for customize ball style
	void setBallImage(std::string);
    
    CC_SYNTHESIZE(Vec2, _velocity, Velocity); //create getter and setter
    CC_SYNTHESIZE(Vec2, _friction, Fricition);
    CC_SYNTHESIZE(Vec2, _acceleration, Acceleration);
    
private:
    float _radius;
    bool _moved = false;
};

#endif /* Ball_hpp */
