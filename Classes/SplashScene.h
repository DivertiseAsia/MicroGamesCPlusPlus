#ifndef __SPLASH_SCENE_H__
#define __SPLASH_SCENE_H__

#include "cocos2d.h"

class Splash : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void endSplash(float duration);
    
    // implement the "static create()" method manually
    CREATE_FUNC(Splash);
};

#endif // __HELLOWORLD_SCENE_H__
