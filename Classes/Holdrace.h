#ifndef __HOLDRACE_SCENE_H__
#define __HOLDRACE_SCENE_H__

#include "Shared.h"
#include "cocos2d.h"
#include "Ball.hpp"
#include "GameButton.hpp"

#define BALL_SPEED 5

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
    
    //Event handling
    static void onPress(Ref*, GameButton::Widget::TouchEventType);
    
    
private:
    Ball* _ball[4];
    GameButton* _button[4];
    
    DrawNode* _drawNode;
};

#endif // __HOLDRACE_SCENE_H__
