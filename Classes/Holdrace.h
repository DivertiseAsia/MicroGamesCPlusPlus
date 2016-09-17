#ifndef __HOLDRACE_SCENE_H__
#define __HOLDRACE_SCENE_H__

#include "cocos2d.h"
#include "Ball.hpp"

USING_NS_CC;

class Holdrace : public Node
{
public:
    static cocos2d::Scene* createScene(); //Create the scene which holds the root node.
    
    //Function members form the based class
    bool init() override;
    void draw(Renderer* renderer, const Mat4& transform, bool transformUpdated);
    void update(float dt) override;
    void onEnter() override;
    
    // implement the "static create()" method manually
    CREATE_FUNC(Holdrace);
    
    // Our game custom functions
    void startGame(float);
    void initTouchHandling();
    
    
private:
    Ball *ball;
    DrawNode* drawNode;
};

#endif // __HOLDRACE_SCENE_H__
