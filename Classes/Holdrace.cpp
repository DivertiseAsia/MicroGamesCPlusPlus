//
//  Holdrace.cpp
//  microgames
//
//  Created by Work on 13/9/16.
//
//

#include "Holdrace.h"
#include "GameList.h"
#include "GLES-Render/B2DebugDrawLayer.h"
#include <math.h>

USING_NS_CC;

#define OFFSET_Y_SCREEN 45
#define MAX_NUMBER_PLAYER 4
#define TIME_OFFSET 0.002

// on "init" you need to initialize your instance
bool Holdrace::init()
{
    //////////////////////////////
    // 1. super init first
	if (!GameScene::initWithColor(Color4B(SHARED_COLOR_BACKGROUND)))
    {
        return false;
    }
	GameScene::gameType = GameType::OVERSHOOT;

    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto screenSize = Director::getInstance()->getVisibleSize();    
    Size winSize = Director::getInstance()->getWinSize();
    
    // Add object to the scene here.
	Shared::instance()->setBackground("bg/BG_JumpyMouse.png",this);

    // Create flags, balls and buttons
	auto mouseDistance = winSize.width * 0.1;
	auto mouseSprite = Sprite::create("item/Animate_Mouse_120x180.png");
	auto sheetSize = mouseSprite->getContentSize();
	mouseWidth = sheetSize.width / 3;
	mouseHeight = sheetSize.height / 4;
	auto firstPositionX = winSize.width * 0.35;
	miceStep = std::vector<int>(numberOfPlayers, 1);

	float buttonScale = 0.5;

    for(int i=0;i<numberOfPlayers;i++){
		_drawNode = DrawNode::create();
		auto flagImg = Sprite::create("item/Item_Jumpy Mice_Flag.png");
		flagImg->setPosition(Vec2(firstPositionX + (i * mouseDistance), OFFSET_Y_SCREEN));
		_drawNode->addChild(flagImg);
		this->addChild(_drawNode);
        auto p = Vec2(firstPositionX + (i * mouseDistance), screenSize.height - OFFSET_Y_SCREEN);
        _ball[i] = Ball::create();
		_ball[i]->setBallImage("item/Animate_Mouse_120x180.png", Rect(0, mouseHeight * i, mouseWidth, mouseHeight));
		_ball[i]->setRotation(180);
        _ball[i]->setPosition(p);
        _ball[i]->setFricition(Vec2(0,HR_BALL_FRICTION));
        _ball[i]->setTag(i);
		_ball[i]->setUserData(&miceStep[i]);
        this->addChild(_ball[i]);
        
        _button[i] = GameButton::create();
		_button[i]->setPosition(Shared::instance()->getPlayerPosition(i));
		_button[i]->setAnchorPoint(Shared::instance()->getPlayerAnchor(i));
		if (screenSize.width / screenSize.height < 0.75 && buttonScale == 0.5)
			buttonScale = SHARED_BUTTON_WIDTH_PERCENT * screenSize.width / _button[i]->getContentSize().width;
		_button[i]->setScale(buttonScale);
        _button[i]->setTag(i);  //Set the number to indicate button order.
        _button[i]->addTouchEventListener(CC_CALLBACK_2(Holdrace::onPress, this));
        _button[i]->setBall(_ball[i]);
		_button[i]->setPlayer(i);
		_button[i]->changeFormat("Paw");
        this->addChild(_button[i]);
    }
    
    //Debug Layer
    //this->addChild(B2DebugDrawLayer::create(this->getScene(), 1), 1000);
    
    
    //Listen to ball stop event
    auto eventName = "ball_stop"; //***** You need this line or the listener will never get fired *****
    _eventDispatcher->addCustomEventListener(eventName, [=](EventCustom* event){
        auto ball = static_cast<Ball*>(event->getUserData());
        auto ballID = ball->getTag();
        log("ball-%i is stopped", ballID);
        _moved[ballID] = true;
        _moveCount++;
        if (_moveCount == numberOfPlayers){
            log("All players played");
            this->showWinner();
        }
    });
    
    this->setName("HoldraceSceneRoot");
    this->showInstruction();
    
    return true;
}

void Holdrace::showInstruction() {
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
	auto margin = visibleSize.height * 0.01;

	auto ins1 = Sprite::create("instructions/JumpyMouse_1.png");
	auto ins2 = Sprite::create("instructions/JumpyMouse_2.png");
	auto ins3 = Sprite::create("instructions/JumpyMouse_3.png");

	auto scale = 0.3 * visibleSize.width / ins1->getContentSize().width;
	auto cenInsHeight = ins2->getContentSize().height * scale / 2;

	ins1->setPosition(Vec2(overlaySize.width / 2, overlaySize.height / 2 + ((ins1->getContentSize().height * scale / 2) + margin * 10 + cenInsHeight)));
	ins1->setScale(scale);
	ins2->setPosition(Vec2(overlaySize.width / 2, overlaySize.height / 2));
	ins2->setScale(scale);
	ins3->setPosition(Vec2(overlaySize.width / 2, overlaySize.height / 2 - ((ins3->getContentSize().height * scale / 2) + margin + cenInsHeight)));
	ins3->setScale(scale);

	rectOverlay->addChild(ins1);
	rectOverlay->addChild(ins2);
	rectOverlay->addChild(ins3);
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

void Holdrace::update(float dt){

	for (int i = 0; i < numberOfPlayers; i++) {
		if (!_moved[i]) {
			_ball[i]->moveNext();
			if (jumped[i]) {
				auto v = -_ball[i]->getVelocity().y;

				if (v < vmax[i] / 3)
					t[i] -= TIME_OFFSET;
				else
					t[i] += TIME_OFFSET;

				auto scale = 1 + vmax[i] * t[i] - 0.5 * pow(t[i], 2);
				scale = std::max<float>(1, scale);
				_ball[i]->setScale(scale);
			}
			else if (running[i] > 0){
				running[i] += 1;
				if (running[i] % 10 == 0) {
					int* step = (int*)_ball[i]->getUserData();
					_ball[i]->setBallImage("item/Animate_Mouse_120x180.png", Rect(mouseWidth * *step, mouseHeight * i, mouseWidth, mouseHeight));
					SoundManager::instance()->playEffect(SOUND_FILE_TAPRACE_STEP);
					miceStep[i] = -*step + 1;
				}
			}
		}
	}

}

//This method will be called on the Node entered.
void Holdrace::onEnter(){
    Node::onEnter();
}

void Holdrace::startGame(){
    GameScene::startGame(SHARED_COUNTDOWN_LENGTH);
	for (int i = 0; i < SHARED_MAX_PLAYERS; i++) {
		_moved[i] = false;
	}
    _moveCount = 0;
}

void Holdrace::onPress(Ref* sender, GameButton::Widget::TouchEventType type){
    auto button = static_cast<GameButton*>(sender);
	if (this->gameStatus != GAME_INPROGRESS || (button->getActionTag() == 10)) { return; }
    log("onPress->->");
    switch (type)
    {
        case ui::Widget::TouchEventType::BEGAN:{
			auto player = button->getPlayer();
			running[player] = 1;
            button->getBall()->setAcceleration(Vec2(0,-HR_BALL_ACCELERATION));
			button->setActionTag(5);//prime the button 
            break;
        }
        case ui::Widget::TouchEventType::ENDED:{
			if (button->getActionTag() != 5) { break; }//was never primed properly eg pressed before game start
            button->setActionTag(10);
			button->getBall()->setAcceleration(Vec2(0,0));
			auto player = button->getPlayer();
			jumped[player] = true;
			vmax[player] = -button->getBall()->getVelocity().y;
			button->getBall()->setBallImage("item/Animate_Mouse_120x180.png", Rect(mouseWidth * 2, mouseHeight * player, mouseWidth, mouseHeight));
            SoundManager::instance()->playEffect(SOUND_FILE_HOLDRACE_JUMP);
            break;
        }
        default:
            break;
    }
}

void Holdrace::showWinner(){
	float closest = Director::getInstance()->getVisibleSize().height;
	int winner = -1;
	for (int i = 0; i<numberOfPlayers; i++) {
		if (_ball[i]->getPositionY() < closest && _ball[i]->getPositionY() >= 0) {
			closest = _ball[i]->getPositionY();
			winner = i;
		}
	}
	this->endGame(winner);
}