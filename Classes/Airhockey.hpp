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

USING_NS_CC;

#define TEAM_TOP 0
#define TEAM_BOT 1
#define BALL_RESET_OFFSET_Y	3
#define PADDLE_DROP 4
#define PADDLE_DRAG 5

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
    
    //Event handling
    void onPress(cocos2d::Ref*, GameButton::Widget::TouchEventType);
    void onDrag();
    using GameScene::GameScene;
    
private:
    void updateScore();
    
    Ball* _ball;
    GameButton* _button[4];
    DrawNode* _paddle[4];
    int _score[2];
    DrawNode* _drawNode;
    Label* scoreTop;
    Label* scoreBottom;
};

#endif /* Airhockey_hpp */
