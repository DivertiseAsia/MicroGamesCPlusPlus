#ifndef __HOLDRACE_SCENE_H__
#define __HOLDRACE_SCENE_H__

#include "Shared.h"
#include "cocos2d.h"
#include "Ball.hpp"
#include "GameButton.hpp"
#include "GameScene.h"

#define BALL_SPEEDMAX 5
#define BALL_ACCELERATION 0.01
#define BALL_FRICTION 1.01

USING_NS_CC;

class Holdrace : public GameScene
{
public:
    
    //Function members form the based class
	bool init() override;
    void draw(Renderer* renderer, const Mat4& transform, bool transformUpdated);
    void update(float dt) override;
    void onEnter() override;
    
    // Our game custom functions
    void startGame(float);
    void initTouchHandling();
    
    //Event handling
    static void onPress(Ref*, GameButton::Widget::TouchEventType);
    
	using GameScene::GameScene;
private:
    Ball* _ball[4];
    GameButton* _button[4];
    
    DrawNode* _drawNode;
};

#endif // __HOLDRACE_SCENE_H__
