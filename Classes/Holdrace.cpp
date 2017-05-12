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
#define MOUSE_OFFSET_X 20
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
    auto screenCenter = Vec2(winSize.width/2, winSize.height/2);
    
    // Add object to the scene here.
	Shared::instance()->setBackground("bg/BG_JumpyMouse.png",this);

    // Create flags, balls and buttons
	auto mouseSprite = Sprite::create("item/Animate_Mouse_120x180.png");
	auto sheetSize = mouseSprite->getContentSize();
	mouseWidth = sheetSize.width / 3;
	mouseHeight = sheetSize.height / 4;
	auto firstPositionX = screenCenter.x - ((4 * mouseWidth/2) / 2);
    for(int i=0;i<numberOfPlayers;i++){
		_drawNode = DrawNode::create();
		auto flagImg = Sprite::create("item/Item_Jumpy Mice_Flag.png");
		flagImg->setPosition(Vec2(firstPositionX + (i * ((mouseWidth / 2) + MOUSE_OFFSET_X)), OFFSET_Y_SCREEN));
		_drawNode->addChild(flagImg);
		this->addChild(_drawNode);
        auto p = Vec2(firstPositionX + (i * ((mouseWidth / 2) + MOUSE_OFFSET_X)), screenSize.height - OFFSET_Y_SCREEN);
        _ball[i] = Ball::create();
		_ball[i]->setBallImage("item/Animate_Mouse_120x180.png", Rect(0, mouseHeight * i, mouseWidth, mouseHeight));
		_ball[i]->setRotation(180);
        _ball[i]->setPosition(p);
        _ball[i]->setFricition(Vec2(0,HR_BALL_FRICTION));
        _ball[i]->setTag(i);
        this->addChild(_ball[i]);
        
        _button[i] = GameButton::create();
		_button[i]->setPosition(Shared::instance()->getPlayerPosition(i));
		_button[i]->setAnchorPoint(Shared::instance()->getPlayerAnchor(i));
		_button[i]->setScale(0.5);
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
    this->scheduleUpdate();
    
    return true;
}

void Holdrace::draw(Renderer* renderer, const Mat4& transform, bool transformUpdated){
    
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
		}
	}
}

//This method will be called on the Node entered.
void Holdrace::onEnter(){
    Node::onEnter();
    startGame();
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
            SoundManager::instance()->playEffect(SOUND_FILE_INGAME_PRESS);
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