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

#define AR_PADDLE_DROP 4
#define AR_PADDLE_DRAG 5

#define AR_PTM_RATIO 32.0

#define AR_BIT_MASK_PUCK 0x0002

#define AR_GOAL_SIZE 0.3f
#define AR_MAX_SCORE 3

#define AH_TEAM_TOP_PLAYERS {SHARED_PLAYER1, SHARED_PLAYER4}
#define AH_TEAM_BOT_PLAYERS {SHARED_PLAYER2, SHARED_PLAYER3}


class Airhockey : public GameScene, public b2ContactListener
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
    
    
    void BeginContact(b2Contact* contact) {
        
        //check if fixture A was a ball
        auto bodyUserData = contact->GetFixtureA()->GetUserData();
        if ( bodyUserData ){
            log("fixtureA:%i", (long)bodyUserData);
        }
        
        if (contact->GetFixtureA() == _topGoal && contact->GetFixtureB() == _ballFixture){
            _scoreBottom->setUserData((void*)((long)_scoreBottom->getUserData()+1));
            updateScore();
            _needReset = true;
        }
        
        if (contact->GetFixtureA() == _bottomGoal && contact->GetFixtureB() == _ballFixture){
            _scoreTop->setUserData((void*)((long)_scoreTop->getUserData()+1));
            updateScore();
            _needReset = true;
        }
        
        
    }
    
    void EndContact(b2Contact* contact) {
        
    }
    
private:
    void updateScore();
    void resetGame(float);
    void checkForGoal();
    
    bool _gamePause = false;
    bool _needReset = false;
    
    Ball* _ball;
    GameButton* _button[4];
    
    DrawNode* _drawNode;
    Label* _scoreTop;
    Label* _scoreBottom;
    
    b2Body* _boxBody;
    b2Body* _ballBody;
    b2Fixture* _ballFixture;
    b2World* _world;
    b2MouseJoint* _mouseJoint;
    b2Fixture* _bottomGoal;
    b2Fixture* _topGoal;
    
    Vec2 _screenOrigin;
    Size _screenSize;
    Vec2 _screenCenter;
};


#endif /* Airhockey_hpp */
