#ifndef __WETKITTEN_SCENE_H__
#define __WETKITTEN_SCENE_H__

#include "Shared.h"
#include "cocos2d.h"
#include "Ball.hpp"
#include "GameButton.hpp"
#include "SoundManager.h"
#include "GameScene.h"
#include <vector>


#define TR_TAPS_REQUIRED 30
#define SB_BOX_SIZE	50.0f

USING_NS_CC;

class WetKitten : public GameScene
{
public:

	//Function members form the based class
	bool init() override;
	void draw(Renderer* renderer, const Mat4& transform, bool transformUpdated);
	void update(float dt) override;
	void onEnter() override;

	// Our game custom functions
	void startGame();

	//Event handling
	void onPress(cocos2d::Ref*, GameButton::Widget::TouchEventType);
	using GameScene::GameScene;

	//Animate mices
	cocos2d::Vector<SpriteFrame*> getAnimation(std::string imgscr, int count, int player, float width, float height);

private:
	Ball* _ball[4];
	GameButton* _button[4];
	int _score[4];
	DrawNode* _drawNode;
	int px[4];
	float mouseWidth;
	float mouseHeight;
	char frameName[4][3][26] = {
		{"item/WetKitten_Cat4-1.png" , "item/WetKitten_Cat4-2.png" , "item/WetKitten_Cat4-3.png" },
		{"item/WetKitten_Cat1-1.png" , "item/WetKitten_Cat1-2.png" , "item/WetKitten_Cat1-3.png" },
		{"item/WetKitten_Cat3-1.png" , "item/WetKitten_Cat3-2.png" , "item/WetKitten_Cat3-3.png" },
		{"item/WetKitten_Cat2-1.png" , "item/WetKitten_Cat2-2.png" , "item/WetKitten_Cat2-3.png" }
	};
	int frameIndex = 0;
	int frameTime =0;
	std::vector<int> miceStep;

	int water1Position = 0;
	int water2Position = 0;
	Sprite* water1;
	Sprite* water2;

	std::vector<Ball*> boxs;
	int boxTime = 80;
	int boxPosition[4];
	void showInstruction();

	bool isDie[4] = { false,false,false,false };
	int countPlayer;
	bool run = true;
};

#endif // __Taprace_SCENE_H__
