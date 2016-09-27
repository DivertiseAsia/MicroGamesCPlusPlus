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
    void startGame();
    void initTouchHandling();
    void showWinner();
    
    
    //Event handling
    void onPress(Ref*, GameButton::Widget::TouchEventType);
    void onBallStopped();
    
	using GameScene::GameScene;
private:
    bool _moved[4] = {false, false, false, false};
    int _moveCount = 0;
    Ball* _ball[4];
    GameButton* _button[4];
    DrawNode* _drawNode;
    
    Ball* getWinner();
};

#endif // __HOLDRACE_SCENE_H__
