//
//  Taprace.cpp
//  microgames
//
//  Created by Work on 13/9/16.
//
//

#include "Taprace.h"
#include "Ball.hpp"
#include "GameButton.hpp"
#include "GLES-Render/B2DebugDrawLayer.h"
#include "ui/CocosGUI.h"
#include "SoundManager.h"

USING_NS_CC;

// on "init" you need to initialize your instance
bool Taprace::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !GameScene::init() )
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
    
    //Position should be based on visibleOrigin and visibleSize properties.
    auto buttonPos = {
        Vec2(origin.x,origin.y+screenSize.height),  //0
        Vec2(origin.x+screenSize.width, origin.y),  //1
        origin, //2
        origin+screenSize}; //3
    auto colors = SHARED_COLOR_PLAYERS;

	auto ballspeed = screenSize.height / TAPS_REQUIRED;
    
    
    // Create balls
    for(int i=0;i<numberOfPlayers;i++){
        auto p = Vec2(screenCenter.x,screenSize.height);
        _ball[i] = Ball::create(colors.begin()[i]);
        _ball[i]->setPosition(p);
        _ball[i]->setVelocity(Vec2(0,-1*ballspeed));
        this->addChild(_ball[i]);
        
        _button[i] = GameButton::create();
        _button[i]->setPosition(buttonPos.begin()[i]);
        _button[i]->changeColor(colors.begin()[i]);
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
    this->initTouchHandling();
    this->scheduleUpdate();
	this->startGame(0);
    
    return true;
}

void Taprace::draw(Renderer* renderer, const Mat4& transform, bool transformUpdated){
    
}

void Taprace::update(float dt){
    
    //ball->moveNext();
}

//This method will be called on the Node entered.
void Taprace::onEnter(){
    Node::onEnter();
    //schedule(SEL_SCHEDULE(Taprace::startGame), 1000);
    //this->scheduleOnce(schedule_selector(Taprace::startGame), 1.0f);
}

void Taprace::startGame(float dt){
	gameStatus = GAME_INPROGRESS;
}

void Taprace::initTouchHandling(){
    auto listener1 = EventListenerTouchOneByOne::create();
    
    // trigger when you push down
    listener1->onTouchBegan = [](Touch* touch, Event* event){
        
        
        auto parentNode = static_cast<Sprite*>(event->getCurrentTarget());
        
        Vector<Node *> children = parentNode->getChildren();
        Point touchPosition = parentNode->convertTouchToNodeSpace(touch);
        for (auto iter = children.rbegin(); iter != children.rend(); ++iter) {
            Node *childNode = *iter;
            if (childNode->getBoundingBox().containsPoint(touchPosition)) {
                //childNode is the touched Node
                //do the stuff here
                log(">>%s",childNode->getName().c_str());
                return true;
            }
        }
        return false;
        //return true; // if you are consuming it
        log("TOUCH!!");
        
    };
    
    // trigger when moving touch
    listener1->onTouchMoved = [](Touch* touch, Event* event){
        log("MOVE");
    };
    
    // trigger when you let up
    listener1->onTouchEnded = [=](Touch* touch, Event* event){
        // your code
        log("TOUCH ENDED");
    };
    
    // Add listener
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, this);
    
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
			if (_score[button->getPlayer()] > TAPS_REQUIRED) {
				log("player %i should win", button->getPlayer());
				endGame(button->getPlayer());
			}
            break;
        }
        default:
            break;
    }
}
