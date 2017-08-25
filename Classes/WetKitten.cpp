//
//  WetKitten.cpp
//  microgames
//
//  Created by Work on 25/8/17.
//
//

#include "WetKitten.h"
#include "GameList.h"
#include "GLES-Render/B2DebugDrawLayer.h"




USING_NS_CC;

#define OFFSET_Y_SCREEN 45
#define MAX_NUMBER_PLAYER 4

// on "init" you need to initialize your instance
bool WetKitten::init()
{
	//////////////////////////////
	// 1. super init first
	if (!GameScene::initWithColor(Color4B(SHARED_COLOR_BACKGROUND)))
	{
		return false;
	}
	GameScene::gameType = GameType::WETKITTEN;

	//http://www.cocos2d-x.org/wiki/Multi_resolution_support

	//auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto screenSize = Director::getInstance()->getVisibleSize();

	Size winSize = Director::getInstance()->getWinSize();
	auto screenCenter = Vec2(winSize.width / 2, winSize.height / 2);

	// Add object to the scene here.
	Shared::instance()->setBackground("bg/BG_WetKitten_1.png", this);

	this->water1 = Sprite::create("bg/BG_WetKitten_2.png");
	auto scaleX = winSize.width / this->water1->getContentSize().width;
	auto scaleY = winSize.height / this->water1->getContentSize().height;
	this->water1->setScaleX(scaleX);
	this->water1->setScaleY(scaleY);
	this->water1->setAnchorPoint(Vec2(0, 0));
	this->water1Position = 0;
	this->water1->setPosition(Vec2(0, 0));
	this->addChild(this->water1);

	this->water2 = Sprite::create("bg/BG_WetKitten_2.png");
	scaleX = winSize.width / this->water2->getContentSize().width;
	scaleY = winSize.height / this->water2->getContentSize().height;
	this->water2->setScaleX(scaleX);
	this->water2->setScaleY(scaleY);
	this->water2->setAnchorPoint(Vec2(0, 0));
	this->water2Position = -screenSize.height;
	this->water2->setPosition(Vec2(0, this->water2Position));
	this->addChild(this->water2);

	// Draw the finish line
	_drawNode = DrawNode::create();
	auto lineImg = Sprite::create("item/Item_Tap Race_Goal.png");
	lineImg->setScale((winSize.width / 3) / lineImg->getContentSize().width);
	lineImg->setPosition(Vec2(screenCenter.x, OFFSET_Y_SCREEN));
	_drawNode->addChild(lineImg);
	this->addChild(_drawNode);
	auto ballspeed = (screenSize.height - OFFSET_Y_SCREEN * 2) / TR_TAPS_REQUIRED;

	// Create balls
	auto mouseDistance = winSize.width * 0.1;
	auto mouseSprite = Sprite::create("item/WetKitten_Cat1-1.png");
	auto sheetSize = mouseSprite->getContentSize();
	mouseWidth = sheetSize.width;
	mouseHeight = sheetSize.height;

	miceStep = std::vector<int>(numberOfPlayers, 1);

	const int ofsetY[] = {
						 75 * 2,
						 75 * 3 + 75,
						 75 * 4 + 75 * 2 ,
						 75 * 5 + 75 * 3,};
	float buttonScale = 0.5;
	int index = 0;
	bool born[] = { false,false,false,false };
	this->boxPosition[0] = winSize.width * 0.20;
	this->boxPosition[1] = winSize.width * 0.40;
	this->boxPosition[2] = winSize.width * 0.60;
	this->boxPosition[3] = winSize.width * 0.80;
	for (int i = 0; i < numberOfPlayers; i++) {
		do {
			 index = cocos2d::RandomHelper::random_int(0, numberOfPlayers);
		} while (born[index] != false);
		born[index] = true;
		Vec2 p;
		if (index % 2 == 1) p = Vec2(winSize.width / 2 - 12.5 * 2, ofsetY[index]);
		else p = Vec2(winSize.width / 2 + 17.5 * 2, ofsetY[index]);

		px[i] = p.x;
		_ball[i] = Ball::create();
		_ball[i]->setBallImage(this->frameName[i][0], Rect(0, 0, mouseWidth, mouseHeight));
		_ball[i]->setScale(1.75);
		_ball[i]->setPosition(p);
		this->addChild(_ball[i]);

		_button[i] = GameButton::create();
		_button[i]->setPosition(Shared::instance()->getPlayerPosition(i));
		_button[i]->setAnchorPoint(Shared::instance()->getPlayerAnchor(i));
		if (screenSize.width / screenSize.height < 0.75 && buttonScale == 0.5)
			buttonScale = SHARED_BUTTON_WIDTH_PERCENT * screenSize.width / _button[i]->getContentSize().width;
		_button[i]->setScale(buttonScale);
		_button[i]->setTag(i);  //Set the number to indicate button order.
		_button[i]->setUserData(&miceStep[i]);
		_button[i]->addTouchEventListener(CC_CALLBACK_2(WetKitten::onPress, this));
		_button[i]->setBall(_ball[i]);
		_button[i]->setLocalZOrder(10);
		_button[i]->setPlayer(i);
		_button[i]->changeFormat("Paw");
		_score[i] = 0;
		this->addChild(_button[i]);
	}


	//Debug Layer
	//this->addChild(B2DebugDrawLayer::create(this->getScene(), 1), 1000);

	this->setName("WetKittenSceneRoot");
	this->showInstruction();
	//this->scheduleUpdate();
	return true;
}

void WetKitten::showInstruction() {
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto visibleSize = Director::getInstance()->getVisibleSize();

	GameScene::rectOverlay = DrawNode::create();
	rectOverlay->setContentSize(visibleSize);
	Vec2 rectangle[4];
	rectangle[0] = Vec2::ZERO;
	rectangle[1] = Vec2(0, visibleSize.height);
	rectangle[2] = visibleSize;
	rectangle[3] = Vec2(visibleSize.width, 0);

	rectOverlay->drawPolygon(rectangle, 4, GameScene::halfblack, 1, GameScene::halfblack);

	auto overlaySize = rectOverlay->getContentSize();
	auto margin = visibleSize.height * 0.1;

	auto ins1 = Sprite::create("instructions/TapRace_1.png");
	auto ins2 = Sprite::create("instructions/TapRace_2.png");

	auto scale = 0.4 * visibleSize.width / ins1->getContentSize().width;

	ins1->setPosition(Vec2(overlaySize.width / 2, overlaySize.height / 2 + ((ins1->getContentSize().height * scale / 2) + margin)));
	ins1->setScale(scale);
	ins2->setPosition(Vec2(overlaySize.width / 2, overlaySize.height / 2 - ((ins2->getContentSize().height * scale / 2) + margin)));
	ins2->setScale(scale);

	rectOverlay->addChild(ins1);
	rectOverlay->addChild(ins2);
	rectOverlay->setPosition(origin);
	this->addChild(rectOverlay);

	auto tabScreenLtn = EventListenerTouchOneByOne::create();

	tabScreenLtn->onTouchBegan = [](Touch* touch, Event* event) {
		return true;
	};

	tabScreenLtn->onTouchEnded = [=](Touch* touch, Event* event) {
		rectOverlay->removeAllChildren();
		this->scheduleUpdate();
		this->startGame();
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(tabScreenLtn, this);
}

void WetKitten::draw(Renderer* renderer, const Mat4& transform, bool transformUpdated) {

}

void WetKitten::update(float dt) {
	if (gameStatus != GAME_INPROGRESS) {
		return;
	}
	auto gameHeight = Director::getInstance()->getVisibleSize().height;
	auto gameWidht = Director::getInstance()->getVisibleSize().width;

	if (this->water1Position >gameHeight) this->water1Position = -gameHeight;
	if (this->water2Position > Director::getInstance()->getVisibleSize().height)this->water2Position = -gameHeight;

	this->water1->setPosition(Vec2(0, -this->water1Position));
	this->water2->setPosition(Vec2(0, -this->water2Position));
	this->water1Position++;
	this->water2Position++;



	for (int i = 0; i < numberOfPlayers; i++) {
		_ball[i]->setPositionX(px[i]);
		//if(_ball[i]->)
		if (px[i] < gameWidht + mouseWidth - 15) {
			px[i]+=2;
		}
	}

	if (this->frameTime <= 0) {
		this->frameTime = 10;
		this->frameIndex++;
		if (this->frameIndex >= 3) this->frameIndex = 0;
		for (int i = 0; i < numberOfPlayers; i++)
		_ball[i]->setBallImage(this->frameName[i][this->frameIndex], Rect(0, 0, mouseWidth,mouseHeight+(this->frameIndex == 1 ?5:0)));
	}
	this->frameTime--;


	if (this->boxTime <= 0) {
		this->boxTime = cocos2d::RandomHelper::random_int(20,25)*10;
		int numBox = cocos2d::RandomHelper::random_int(0, 4);
		bool *born = new bool[numBox];
		for (int i = 0; i < numBox; i++) born[i] = false;
		for (int i = 0; i < numBox; i++) {
			auto box = Ball::create();
			auto boxSprite = Sprite::create("item/Item_BlockUpper.png");
			auto sheetSize = boxSprite->getContentSize();
			box->setBallImage("item/Item_BlockUpper.png", Rect(0, 0, sheetSize.width, sheetSize.height));
			int indexPos = 0;
			//do {
				indexPos = cocos2d::RandomHelper::random_int(0, 4);
			//} while (born[indexPos] != false);
			//born[indexPos] = true;
			Vec2 p = Vec2(this->boxPosition[indexPos], gameHeight);
			box->setPosition(p);
			box->setRotation(45);
			this->addChild(box);
			this->boxs.push_back(box);
		}
	}

	this->boxTime--;
	int i = 0; 
	for (Ball* b : boxs) {
		b->setPositionY(b->getPositionY() - 1);
		if (b->getPositionY() < 0) {
			this->removeChild(b);
			b->release();
			boxs.erase(boxs.begin() + i);
		}
		i++;
	}
}	

//This method will be called on the Node entered.
void WetKitten::onEnter() {
	Node::onEnter();
}

void WetKitten::startGame() {
	GameScene::startGame(SHARED_COUNTDOWN_LENGTH);
}

void WetKitten::onPress(Ref* sender, GameButton::Widget::TouchEventType type) {
	if (gameStatus != GAME_INPROGRESS) {
		return;
	}
	switch (type)
	{
	case ui::Widget::TouchEventType::BEGAN:
		break;
	case ui::Widget::TouchEventType::ENDED: {
		auto button = static_cast<GameButton*>(sender);
		button->getBall()->moveNext();
		if (px[button->getPlayer()] > mouseWidth*2) {
			px[button->getPlayer()] -= 40;
			_ball[button->getPlayer()]->setPositionX(px[button->getPlayer()]);
		}
		else {
			 px[button->getPlayer()] = mouseWidth * 2;
			_ball[button->getPlayer()]->setPositionX(px[button->getPlayer()]);
		}
		SoundManager::instance()->playEffect(SOUND_FILE_TAPRACE_STEP);

		if (_score[button->getPlayer()] > TR_TAPS_REQUIRED) {
			endGame(button->getPlayer());
		}
		break;
	}
	default:
		break;
	}
}

cocos2d::Vector<SpriteFrame*> WetKitten::getAnimation(std::string imgscr, int count, int player, float width, float height)
{
	auto spritecache = SpriteFrameCache::getInstance();
	cocos2d::Vector<SpriteFrame*> animFrames;
	for (int i = 1; i <= count; i++)
	{
		auto spriteImg = Sprite::create(imgscr, Rect(width * i, height * player, width, height));
		spriteImg->setRotation(180);
		animFrames.pushBack(spriteImg->getSpriteFrame());
	}
	return animFrames;
}
