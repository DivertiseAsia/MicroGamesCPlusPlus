//
//  Taprace.cpp
//  microgames
//
//  Created by Work on 13/9/16.
//
//

#include "Taprace.h"
#include "GLES-Render/B2DebugDrawLayer.h"

USING_NS_CC;

// on "init" you need to initialize your instance
bool Taprace::init()
{
    //////////////////////////////
    // 1. super init first
	if (!GameScene::initWithColor(Color4B(SHARED_COLOR_BACKGROUND)))
    {
        return false;
    }
    
    //http://www.cocos2d-x.org/wiki/Multi_resolution_support
    
    //auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto screenSize = Director::getInstance()->getVisibleSize();
    
    
    Size winSize = Director::getInstance()->getWinSize();
    auto screenCenter = Vec2(winSize.width/2, winSize.height/2);
    
    // Add object to the scene here.
    
    //Create drawNode and draw the race line
    _drawNode = DrawNode::create(10);    //Default line width
    _drawNode->drawLine(Vec2(screenCenter.x,0), Vec2(screenCenter.x,winSize.height), Color4F::GRAY);
    this->addChild(_drawNode);

	auto ballspeed = screenSize.height / TR_TAPS_REQUIRED;
    
    
    // Create balls
    for(int i=0;i<numberOfPlayers;i++){
        auto p = Vec2(screenCenter.x,screenSize.height);
        _ball[i] = Ball::create(Shared::instance()->getPlayerColor(i));
        _ball[i]->setPosition(p);
        _ball[i]->setVelocity(Vec2(0,-1*ballspeed));
        this->addChild(_ball[i]);
        
        _button[i] = GameButton::create();
        _button[i]->setPosition(Shared::instance()->getPlayerPosition(i));
        _button[i]->changeColor(Shared::instance()->getPlayerColor(i));
        _button[i]->setTag(i);  //Set the number to indicate button order.
        _button[i]->addTouchEventListener(CC_CALLBACK_2(Taprace::onPress,this));
        _button[i]->setBall(_ball[i]);
		_button[i]->setPlayer(i);
		_score[i] = 0;
        
        this->addChild(_button[i]);
    }
    
    //Debug Layer
    //this->addChild(B2DebugDrawLayer::create(this->getScene(), 1), 1000);
    
    this->setName("TapraceSceneRoot");
    this->scheduleUpdate();
    
    return true;
}

void Taprace::draw(Renderer* renderer, const Mat4& transform, bool transformUpdated){
    
}

void Taprace::update(float dt){
    
}

//This method will be called on the Node entered.
void Taprace::onEnter(){
    Node::onEnter();
	startGame();
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
            SoundManager::instance()->playEffect(SOUND_FILE_INGAME_PRESS);

			if (_score[button->getPlayer()] > TR_TAPS_REQUIRED) {
				endGame(button->getPlayer());
			}
            break;
        }
        default:
            break;
    }
}
