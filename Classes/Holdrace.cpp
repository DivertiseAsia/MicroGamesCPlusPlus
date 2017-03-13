//
//  Holdrace.cpp
//  microgames
//
//  Created by Work on 13/9/16.
//
//

#include "Holdrace.h"
#include "GLES-Render/B2DebugDrawLayer.h"

USING_NS_CC;


// on "init" you need to initialize your instance
bool Holdrace::init()
{
    //////////////////////////////
    // 1. super init first
	if (!GameScene::initWithColor(Color4B(SHARED_COLOR_BACKGROUND)))
    {
        return false;
    }

    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto screenSize = Director::getInstance()->getVisibleSize();
    
    
    Size winSize = Director::getInstance()->getWinSize();
    auto screenCenter = Vec2(winSize.width/2, winSize.height/2);
    
    // Add object to the scene here.
	Shared::instance()->setBackground("bg/BG_JumpyMouse.png",this);
    
    //Create drawNode and draw the race line
    _drawNode = DrawNode::create(10);    //Default line width
    _drawNode->drawLine(Vec2(screenCenter.x,0), Vec2(screenCenter.x,winSize.height), Color4F::GRAY);
    this->addChild(_drawNode);

    // Create balls and buttons
    for(int i=0;i<numberOfPlayers;i++){
        auto p = Vec2(screenCenter.x,screenSize.height);
        _ball[i] = Ball::create(Shared::instance()->getPlayerColor(i));
        _ball[i]->setPosition(p);
        _ball[i]->setFricition(Vec2(0,HR_BALL_FRICTION));
        _ball[i]->setTag(i);
        this->addChild(_ball[i]);
        
        _button[i] = GameButton::create();
        _button[i]->setPosition(Shared::instance()->getPlayerPosition(i));
        _button[i]->changeColor(Shared::instance()->getPlayerColor(i));
        _button[i]->setTag(i);  //Set the number to indicate button order.
        _button[i]->addTouchEventListener(CC_CALLBACK_2(Holdrace::onPress, this));
        _button[i]->setBall(_ball[i]);
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

    for(int i=0;i<numberOfPlayers;i++){
        if (!_moved[i])
            _ball[i]->moveNext();
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