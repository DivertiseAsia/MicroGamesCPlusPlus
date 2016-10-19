#ifndef __Taprace_SCENE_H__
#define __Taprace_SCENE_H__

#include "Shared.h"
#include "cocos2d.h"
#include "Ball.hpp"
#include "GameButton.hpp"
#include "GameScene.h"

#define TAPS_REQUIRED 30

USING_NS_CC;

class Taprace : public GameScene
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
    
    //Event handling
    void onPress(cocos2d::Ref*, GameButton::Widget::TouchEventType);
	using GameScene::GameScene;
    
private:
    Ball* _ball[4];
    GameButton* _button[4];
	int _score[4];
    DrawNode* _drawNode;
};

#endif // __Taprace_SCENE_H__
