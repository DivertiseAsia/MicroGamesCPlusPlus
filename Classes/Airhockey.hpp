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

enum entityCategory {
    BOUNDARY =      0x0001,
    CENTER_LINE =   0x0002,
    PUCK      =     0x0004,
    MULLET      =   0x0008,
};


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
    
    //Event handling
    void onPress(cocos2d::Ref*, GameButton::Widget::TouchEventType);
    void onDrag();
    using GameScene::GameScene;
    
private:
    void updateScore();
    
    Ball* _ball;
    GameButton* _button[4];
    
    DrawNode* _drawNode;
    Label* scoreTop;
    Label* scoreBottom;
    
    b2Body* _boxBody;
    b2Body* _ballBody;
    b2World* _world;
    b2MouseJoint* _mouseJoint;
};


#endif /* Airhockey_hpp */
