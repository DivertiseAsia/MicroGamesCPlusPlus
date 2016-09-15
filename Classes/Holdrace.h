#ifndef __HOLDRACE_SCENE_H__
#define __HOLDRACE_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class Holdrace : public Node
{
public:
    static cocos2d::Scene* createScene(); //Create the scene which holds the root node.
    
    virtual bool init();
    
    cocos2d::DrawNode* drawNode;
    
    // implement the "static create()" method manually
    CREATE_FUNC(Holdrace);
};

#endif // __HOLDRACE_SCENE_H__
