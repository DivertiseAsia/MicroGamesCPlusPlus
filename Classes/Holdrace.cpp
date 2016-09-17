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
#include "GLES-Render/B2DebugDrawLayer.h"

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
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    Size winSize = Director::getInstance()->getWinSize();
    auto screenCenter = Vec2(winSize.width/2, winSize.height/2);
    ball = Ball::create();
    
    // Add object to the scene here.
    ball->setPosition(screenCenter);
    this->addChild(ball);
    
    //Debug Layer
    //this->addChild(B2DebugDrawLayer::create(this->getScene(), 1), 1000);
    
    this->scheduleUpdate();
    
    return true;
}

void Holdrace::draw(Renderer* renderer, const Mat4& transform, bool transformUpdated){
}

void Holdrace::update(float dt){
    log("update");
    ball->moveNext();
}

void Holdrace::onEnter(){
    Node::onEnter();
    log("test onEnter");
}
