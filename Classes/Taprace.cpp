//
//  Taprace.cpp
//  microgames
//
//  Created by Work on 13/9/16.
//
//

#include "Taprace.h"
#include "GameList.h"
#include "GLES-Render/B2DebugDrawLayer.h"
#include <vector>

USING_NS_CC;

#define OFFSET_Y_SCREEN 45
#define MAX_NUMBER_PLAYER 4

// on "init" you need to initialize your instance
bool Taprace::init()
{
    //////////////////////////////
    // 1. super init first
	if (!GameScene::initWithColor(Color4B(SHARED_COLOR_BACKGROUND)))
    {
        return false;
    }
	GameScene::gameType = GameType::TAPTAP;
    
    //http://www.cocos2d-x.org/wiki/Multi_resolution_support
    
    //auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto screenSize = Director::getInstance()->getVisibleSize();
    
    Size winSize = Director::getInstance()->getWinSize();
    auto screenCenter = Vec2(winSize.width/2, winSize.height/2);
    
	// Add object to the scene here.
	Shared::instance()->setBackground("bg/BG_TapRace.png", this);
    
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
	auto mouseSprite = Sprite::create("item/Animate_Mouse_120x180.png");
	auto sheetSize = mouseSprite->getContentSize();
	log("mouseSprite :::: ==> %d", sheetSize.width);
	mouseWidth = sheetSize.width / 3;
	mouseHeight = sheetSize.height / 4;
	auto firstPositionX = winSize.width * 0.35;
	miceStep = std::vector<int>(numberOfPlayers, 1);

	float buttonScale = 0.5;

    for(int i=0;i<numberOfPlayers;i++){
		auto p = Vec2(firstPositionX + (i * mouseDistance), screenSize.height - OFFSET_Y_SCREEN);
		_ball[i] = Ball::create();
		_ball[i]->setBallImage("item/Animate_Mouse_120x180.png", Rect(0, mouseHeight * i, mouseWidth, mouseHeight));
		_ball[i]->setRotation(180);
		_ball[i]->setPosition(p);
        _ball[i]->setVelocity(Vec2(0,-1*ballspeed));
        this->addChild(_ball[i]);
        
        _button[i] = GameButton::create();
		_button[i]->setPosition(Shared::instance()->getPlayerPosition(i));
		_button[i]->setAnchorPoint(Shared::instance()->getPlayerAnchor(i));
		if (screenSize.width / screenSize.height < 0.75 && buttonScale == 0.5)
			buttonScale = SHARED_BUTTON_WIDTH_PERCENT * screenSize.width / _button[i]->getContentSize().width;
		_button[i]->setScale(buttonScale);
		_button[i]->setTag(i);  //Set the number to indicate button order.
		_button[i]->setUserData(&miceStep[i]);
		_button[i]->addTouchEventListener(CC_CALLBACK_2(Taprace::onPress, this));
		_button[i]->setBall(_ball[i]);
		_button[i]->setPlayer(i);
		_button[i]->changeFormat("Paw");
		_score[i] = 0;
        
        this->addChild(_button[i]);
    }
	
    
    //Debug Layer
    //this->addChild(B2DebugDrawLayer::create(this->getScene(), 1), 1000);
    
    this->setName("TapraceSceneRoot");
    this->showInstruction();
    
    return true;
}

void Taprace::showInstruction() {
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

void Taprace::draw(Renderer* renderer, const Mat4& transform, bool transformUpdated){
    
}

void Taprace::update(float dt){
    
}

//This method will be called on the Node entered.
void Taprace::onEnter(){
    Node::onEnter();
}

void Taprace::startGame(){
	GameScene::startGame(SHARED_COUNTDOWN_LENGTH);
}

void Taprace::onPress(Ref* sender, GameButton::Widget::TouchEventType type){
	if (gameStatus != GAME_INPROGRESS) {
		return;
	}
    switch (type)
    {
        case ui::Widget::TouchEventType::BEGAN:
            break;
        case ui::Widget::TouchEventType::ENDED:{
            auto button = static_cast<GameButton*>(sender);
            button->getBall()->moveNext();
			_score[button->getPlayer()] ++;

			auto player = button->getPlayer();
			int* step = (int*)button->getUserData();
			button->getBall()->setBallImage("item/Animate_Mouse_120x180.png", Rect(mouseWidth * *step, mouseHeight * player, mouseWidth, mouseHeight));
			miceStep[player] = -*step + 1;
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

cocos2d::Vector<SpriteFrame*> Taprace::getAnimation(std::string imgscr, int count, int player, float width, float height)
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
