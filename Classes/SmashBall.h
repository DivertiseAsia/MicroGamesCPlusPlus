#ifndef __SMASHBALL_SCENE_H__
#define __SMASHBALL_SCENE_H__

#define SMB_PADDLE_LENGTH_PERCENT	0.33f
#define SMB_PADDLE_OFFSET_X_PERCENT .325f
#define SMB_PADDLE_ANG_VEL	14

#include "Shared.h"
#include "cocos2d.h"
#include "GameButton.hpp"
#include "SoundManager.h"
#include "SharedBall.h"
#include <Box2D/Box2D.h>

USING_NS_CC;

class SmashBall : public SharedBall
{
public:
    
    //Function members from the based class
    bool init() override;
    void update(float dt) override;
    
    //Event handling
    void onPress(cocos2d::Ref*, GameButton::Widget::TouchEventType) override;

	using SharedBall::SharedBall;
private:
	DrawNode* addPaddleForPlayer(int, cocos2d::Size, cocos2d::Vec2);
};

#endif // __SMASHBALL_SCENE_H__
