#ifndef __SHARED_BALL_H__
#define __SHARED_BALL_H__

#include "Shared.h"
#include "cocos2d.h"
#include "GameButton.hpp"
#include "SoundManager.h"
#include "GameScene.h"
#include <Box2D/Box2D.h>

#define SB_GRAVITY 10.0f
#define SB_POINTS_TO_WIN 4
#define SB_TEAM_TOP 0
#define SB_TEAM_BOT 1
#define SB_TEAM_TOP_PLAYERS {SHARED_PLAYER1, SHARED_PLAYER4}
#define SB_TEAM_BOT_PLAYERS {SHARED_PLAYER2, SHARED_PLAYER3}
#define SB_PADDLE_WIDTH_PX	20
#define SB_PADDLE_CONTROL_RAD 30.0f
#define SB_BOX_SIZE		50.0f
#define SB_BOX_YOFFSET		200
#define SB_WALL_WIDTH		5.0f
#define SB_BALL_RESET_OFFSET_Y	3
#define SB_BALL_RESET_OFFSET_X	20

#define SB_SCORE_OFFSET_X 50
#define SB_SCORE_OFFSET_Y 50

#define SB_DEFAULT_DENSITY		10.0f
#define SB_DEFAULT_FRICTION	0.8f
#define SB_DEFAULT_RESTITUTION 0.6f

#define SB_SCALE_RATIO 32.0

#define SB_BITMASK_BUTTON 0x0002
#define SB_BITMASK_PADDLE 0x0004

USING_NS_CC;

class SharedBall : public GameScene, public b2ContactListener
{
public:
    
    //Function members from the based class
    void draw(Renderer* renderer, const Mat4& transform, bool transformUpdated);
    void update(float dt) override;
    
    // Our game custom functions
    void startGame();
    
    //Event handling
    virtual void onPress(cocos2d::Ref*, GameButton::Widget::TouchEventType);
    void BeginContact(b2Contact* contact) override;
    void EndContact(b2Contact* contact) override;
    
    using GameScene::GameScene;
	b2World *world;
	float deltaTime;
    
    
protected:
	void createBox(Vec2, std::string);
	void addScore(int team, int amount);
	void createWall(Vec2, float);
	void onShake(cocos2d::Acceleration *, cocos2d::Event *);
	void updatePhysics(float dt);
	GameButton* addButtonForPlayer(int, int);
	void createBall(Vec2, float, std::string);
	void setupScore(int,Vec2,Vec2);
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
	float buttonScale = 0.5;
};

#endif // __SHARED_BALL_H__
