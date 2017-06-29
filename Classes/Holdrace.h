#ifndef __HOLDRACE_SCENE_H__
#define __HOLDRACE_SCENE_H__

#include "Shared.h"
#include "cocos2d.h"
#include "Ball.hpp"
#include "GameButton.hpp"
#include "GameScene.h"
#include "SoundManager.h"

#define HR_BALL_ACCELERATION	0.01f
#define HR_BALL_FRICTION		1.01f

USING_NS_CC;

class Holdrace : public GameScene
{
public:
    
    //Function members form the based class
	bool init() override;
    void draw(Renderer* renderer, const Mat4& transform, bool transformUpdated);
    void update(float dt) override;
    void onEnter() override;
    
    // Our game custom functions
    void startGame();
    void showWinner();
    
    //Event handling
	void onPress(Ref*, GameButton::Widget::TouchEventType);
    
	using GameScene::GameScene;
private:
	bool _moved[4] = { false, false, false, false };
    int _moveCount = 0;
    Ball* _ball[4];
    GameButton* _button[4];
    DrawNode* _drawNode;
	float mouseWidth;
	float mouseHeight;
	std::vector<int> miceStep;
	bool jumped[4] = { false, false, false, false };
	int running[4] = { 0, 0, 0, 0 };
	float vmax[4] = { .0f, .0f, .0f, .0f };
	float t[4] = { .0f, .0f, .0f, .0f };
	void showInstruction();
};

#endif // __HOLDRACE_SCENE_H__
