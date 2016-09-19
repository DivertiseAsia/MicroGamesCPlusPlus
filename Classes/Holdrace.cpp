//
//  Holdrace.cpp
//  microgames
//
//  Created by Work on 13/9/16.
//
//

#include "Holdrace.h"
#include "SimpleAudioEngine.h"
#include "Ball.hpp"
#include "GameButton.hpp"
#include "GLES-Render/B2DebugDrawLayer.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

Scene* Holdrace::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = Holdrace::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool Holdrace::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Node::init() )
    {
        return false;
    }
    
    //auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    Size winSize = Director::getInstance()->getWinSize();
    auto screenCenter = Vec2(winSize.width/2, winSize.height/2);
    
    // Add object to the scene here.
    
    // Create balls
    //ball = Ball::create();
    //ball->setPosition(screenCenter);
    //this->addChild(ball);
    
    // Create buttons
    auto button = GameButton::create();
    button->setPosition(screenCenter);
    button->addTouchEventListener([&](Ref* sender, GameButton::Widget::TouchEventType type){
        log("any call at all?");
        switch (type)
        {
            case ui::Widget::TouchEventType::BEGAN:
                log("touch begin");
                break;
            case ui::Widget::TouchEventType::ENDED:
                log("clicked");
                break;
            default:
                break;
        }
    });
    this->addChild(button);
    
    
    //Debug Layer
    //this->addChild(B2DebugDrawLayer::create(this->getScene(), 1), 1000);
    
    this->setName("HoldraceSceneRoot");
    this->initTouchHandling();
    this->scheduleUpdate();
    
    return true;
}

void Holdrace::draw(Renderer* renderer, const Mat4& transform, bool transformUpdated){
}

void Holdrace::update(float dt){
    
    //ball->moveNext();
}

//This method will be called on the Node entered.
void Holdrace::onEnter(){
    Node::onEnter();
    //schedule(SEL_SCHEDULE(Holdrace::startGame), 1000);
    //this->scheduleOnce(schedule_selector(Holdrace::startGame), 1.0f);
}

void Holdrace::startGame(float dt){
    ball->setVelocity(Vec2(1,0));
}

void Holdrace::initTouchHandling(){
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
