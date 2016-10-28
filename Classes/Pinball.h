#ifndef __PINBALL_SCENE_H__
#define __PINBALL_SCENE_H__

#include "Shared.h"
#include "cocos2d.h"
#include "GameButton.hpp"
#include "SoundManager.h"
#include "GameScene.h"
#include <Box2D/Box2D.h>

#define PB_GRAVITY 10.0f
#define PB_POINTS_TO_WIN 4
#define PB_TEAM_TOP 0
#define PB_TEAM_BOT 1
#define PB_TEAM_TOP_PLAYERS {SHARED_PLAYER1, SHARED_PLAYER4}
#define PB_TEAM_BOT_PLAYERS {SHARED_PLAYER2, SHARED_PLAYER3}
#define PB_PADDLE_LENGTH_PERCENT	0.33f
#define PB_PADDLE_WIDTH_PX	20
#define PB_PADDLE_ANG_VEL	14
#define PB_PADDLE_CONTROL_RAD 30.0f
#define PB_PADDLE_OFFSET_X_PERCENT .325f
#define PB_BOX_SIZE		50.0f
#define PB_BOX_YOFFSET		200
#define PB_WALL_WIDTH		5.0f
#define PB_BALL_RESET_OFFSET_Y	3
#define PB_BALL_RESET_OFFSET_X	20

#define PB_SCORE_OFFSET_X 50
#define PB_SCORE_OFFSET_Y 50

#define PB_DEFAULT_DENSITY		10.0f
#define PB_DEFAULT_FRICTION	0.8f
#define PB_DEFAULT_RESTITUTION 0.6f

#define SCALE_RATIO 32.0

USING_NS_CC;

class Pinball : public GameScene, public b2ContactListener
{
public:
    
    //Function members from the based class
    bool init() override;
    void draw(Renderer* renderer, const Mat4& transform, bool transformUpdated);
    void update(float dt) override;
    void onEnter() override;
	void onGameStart() override;
    
    // Our game custom functions
    void startGame();
    
    //Event handling
    void onPress(cocos2d::Ref*, GameButton::Widget::TouchEventType);
    void BeginContact(b2Contact* contact) override;
    void EndContact(b2Contact* contact) override;
    
    using GameScene::GameScene;
	b2World *world;
	float deltaTime;
    
    
private:
	void createBox(Vec2);
	void createWall(Vec2, float);
	void onShake(cocos2d::Acceleration *, cocos2d::Event *);
	GameButton* addButtonForPlayer(int);
	DrawNode* addPaddleForPlayer(int, cocos2d::Size, cocos2d::Vec2);
	void updateScore();
    DrawNode* _ball;
	b2Body* ballBody;
    GameButton* _button[4];
	DrawNode* _paddle[4];
	b2Body* paddleControlBody[4];
	Vec2 _positions[4];
	int _score[2];
    DrawNode* _drawNode;
	Label* scoreTop;
	Label* scoreBottom;
};

#endif // __PINBALL_SCENE_H__
