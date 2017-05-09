#ifndef __PINBALL_SCENE_H__
#define __PINBALL_SCENE_H__


#define PB_PADDLE_OFFSET_X_PERCENT .14f
#define PB_PADDLE_LENGTH_PERCENT .250f
#define PB_MIN_ANGLE {-15, 165, 80, 260}
#define PB_MAX_ANGLE_DIFF 115
#define PB_PADDLE_ANG_VEL	12

#include "Shared.h"
#include "cocos2d.h"
#include "GameButton.hpp"
#include "SoundManager.h"
#include "SharedBall.h"
#include <Box2D/Box2D.h>

USING_NS_CC;

class Pinball : public SharedBall
{
public:
    
    //Function members from the based class
    bool init() override;
    void update(float dt) override;
    
    //Event handling
    void onPress(cocos2d::Ref*, GameButton::Widget::TouchEventType) override;

	using SharedBall::SharedBall;
private:
	void lockPaddleAngle(int);
	float getMaxPaddleAngle(float);
	float getMinPaddleAngle(int);
	DrawNode* addPaddleForPlayer(int, cocos2d::Size, cocos2d::Vec2);
};

#endif // __PINBALL_SCENE_H__
