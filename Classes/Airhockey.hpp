//
//  Airhockey.hpp
//  microgames
//
//  Created by Work on 19/10/16.
//
//

#ifndef __AIRHOCKEY_SCENE_H__
#define __AIRHOCKEY_SCENE_H__

#include "Shared.h"
#include "cocos2d.h"
#include "Ball.hpp"
#include "GameButton.hpp"
#include "GameScene.h"
#include <Box2D/Box2D.h>

USING_NS_CC;

#define TEAM_TOP 0
#define TEAM_BOT 1
#define BALL_RESET_OFFSET_Y	3
#define PADDLE_DROP 4
#define PADDLE_DRAG 5

#define PTM_RATIO 32.0
#define GRAVITY b2Vec2(0.0f, 0.0f)

#define BIT_MASK_PUCK 0x0002


class Airhockey : public GameScene
{
public:
    
    //Function members from the based-class
    bool init() override;
    void draw(Renderer* renderer, const Mat4& transform, bool transformUpdated);
    void update(float dt) override;
    void onEnter() override;
    
    // Our game custom functions
    void startGame(float);
    void initTouchHandling();
    void createWall();
    void drawBoard();
    void addScores();
    void placePuck();
    void addMallet(int playerNo, Vec2 pos, Color4F color);
    
    //Event handling
    void onPress(cocos2d::Ref*, GameButton::Widget::TouchEventType);
    void onDrag();
    using GameScene::GameScene;
    
private:
    void updateScore();
    
    Ball* _ball;
    GameButton* _button[4];
    
    DrawNode* _drawNode;
    Label* _scoreTop;
    Label* _scoreBottom;
    
    b2Body* _boxBody;
    b2Body* _ballBody;
    b2World* _world;
    b2MouseJoint* _mouseJoint;
    
    Vec2 _screenOrigin;
    Size _screenSize;
    Vec2 _screenCenter;
};


#endif /* Airhockey_hpp */
