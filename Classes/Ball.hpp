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
    
    virtual bool init();
    
    CREATE_FUNC(Ball);
    
protected:
    float radius();
};

#endif /* Ball_hpp */
