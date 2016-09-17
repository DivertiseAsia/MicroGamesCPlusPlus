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

class Ball : public DrawNode
{
public:
    Ball();
    ~Ball();
    
    CREATE_FUNC(Ball);
    virtual bool init();
    
    Vec2 _velocity;
    
    void moveNext();
    
private:
    float _radius;
    Color4F _color;
    
    
};

#endif /* Ball_hpp */
