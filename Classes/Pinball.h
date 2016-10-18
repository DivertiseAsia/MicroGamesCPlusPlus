#ifndef __PINBALL_SCENE_H__
#define __PINBALL_SCENE_H__

#include "Shared.h"
#include "cocos2d.h"
#include "Ball.hpp"
#include "GameButton.hpp"
#include "GameScene.h"

#define POINTS_TO_WIN 3
#define TEAM_TOP 0
#define TEAM_BOT 1
#define TEAM_TOP_PLAYERS {SHARED_PLAYER1, SHARED_PLAYER4}
#define TEAM_BOT_PLAYERS {SHARED_PLAYER2, SHARED_PLAYER3}
USING_NS_CC;

class Pinball : public GameScene
{
public:
    
    //Function members from the based class
    bool init() override;
    void draw(Renderer* renderer, const Mat4& transform, bool transformUpdated);
    void update(float dt) override;
    void onEnter() override;
    
    // Our game custom functions
    void startGame(float);
    void initTouchHandling();
    
    //Event handling
    void onPress(cocos2d::Ref*, GameButton::Widget::TouchEventType);
	using GameScene::GameScene;
    
private:
    Ball* _ball;
    GameButton* _button[4];
	int _score[2];
    DrawNode* _drawNode;
};

#endif // __PINBALL_SCENE_H__
