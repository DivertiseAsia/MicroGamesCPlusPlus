#ifndef __PINBALL_SCENE_H__
#define __PINBALL_SCENE_H__

#include "Shared.h"
#include "cocos2d.h"
#include "Ball.hpp"
#include "GameButton.hpp"
#include "GameScene.h"

#define POINTS_TO_WIN 10
#define TEAM_TOP 0
#define TEAM_BOT 1
#define TEAM_TOP_PLAYERS {SHARED_PLAYER1, SHARED_PLAYER4}
#define TEAM_BOT_PLAYERS {SHARED_PLAYER2, SHARED_PLAYER3}
#define MIN_ANGLE {80, 260, 40, 220}
#define MAX_ANGLE_DIFF 60
#define PADDLE_LENGTH_PERCENT	0.4
#define PADDLE_WIDTH_PX	30
#define PADDLE_ANG_VEL	5
#define CAT_MASK_PADDLE	0x01
#define CAT_MASK_STAT	0x02
#define CAT_MASK_BALL	0x05
#define BALL_RESET_OFFSET_Y	3

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
	void updateScore();
	void lockPaddleAngle(int);
	float getMaxPaddleAngle(float);
	float getMinPaddleAngle(int);
    Ball* _ball;
    GameButton* _button[4];
	DrawNode* _paddle[4];
	int _score[2];
    DrawNode* _drawNode;
	Label* scoreTop;
	Label* scoreBottom;
};

#endif // __PINBALL_SCENE_H__
