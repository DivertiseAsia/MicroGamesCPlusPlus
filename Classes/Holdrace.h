#ifndef __HOLDRACE_SCENE_H__
#define __HOLDRACE_SCENE_H__

#include "cocos2d.h"
#include "Ball.hpp"

USING_NS_CC;

class Holdrace : public Node
{
public:
    static cocos2d::Scene* createScene(); //Create the scene which holds the root node.
    
    bool init() override;
    void draw(Renderer* renderer, const Mat4& transform, bool transformUpdated);
    void update(float dt) override;
    void onEnter() override;
    
    cocos2d::DrawNode* drawNode;
    
    // implement the "static create()" method manually
    CREATE_FUNC(Holdrace);
    
private:
    Ball *ball;
    
};

#endif // __HOLDRACE_SCENE_H__
