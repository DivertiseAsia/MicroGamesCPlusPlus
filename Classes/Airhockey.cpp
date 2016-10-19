//
//  Airhockey.cpp
//  microgames
//
//  Created by Work on 19/10/16.
//
//

#include "Airhockey.hpp"

// on "init" you need to initialize your instance
bool Airhockey::init()
{
    //////////////////////////////
    // 1. super init first
    if (!GameScene::init())
    {
        return false;
    }
    
    //http://www.cocos2d-x.org/wiki/Multi_resolution_support
    
    //auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto screenSize = Director::getInstance()->getVisibleSize();
    
    
    Size winSize = Director::getInstance()->getWinSize();
    auto screenCenter = Vec2(winSize.width / 2, winSize.height / 2);
    
    //Create drawNode and draw the center line
    _drawNode = DrawNode::create(10);    //Default line width
    _drawNode->drawLine(Vec2(0, screenCenter.y), Vec2(winSize.width, screenCenter.y), Color4F::GRAY);
    this->addChild(_drawNode);
    
    //Position should be based on visibleOrigin and visibleSize properties.
    auto buttonPos = {
        Vec2(origin.x,origin.y + screenSize.height),  //0
        Vec2(origin.x + screenSize.width, origin.y),  //1
        origin, //2
        origin + screenSize }; //3
    auto colors = SHARED_COLOR_PLAYERS;
    
    
    _score[TEAM_TOP] = 0;
    _score[TEAM_BOT] = 0;
    
    scoreBottom = Label::createWithBMFont(SHARED_FONT_FILE_INGAME, "");
    scoreBottom->setBMFontSize(64);
    scoreBottom->setPosition(Vec2(50, screenCenter.y - 50));
    scoreBottom->setRotation(90);
    this->addChild(scoreBottom, 1);
    
    scoreTop = Label::createWithBMFont(SHARED_FONT_FILE_INGAME, "");
    scoreTop->setBMFontSize(64);
    scoreTop->setPosition(Vec2(50, screenCenter.y + 50));
    scoreTop->setRotation(90);
    this->addChild(scoreTop, 1);
    updateScore();
    
    //create the walls
    auto drawNode = DrawNode::create();
    drawNode->setContentSize(cocos2d::Size(3, screenSize.height));
    drawNode->drawSolidRect(Vec2(0, 0), Vec2(3, screenSize.height), Color4F::RED);
    auto body = PhysicsBody::createBox(Size(3, screenSize.height), PhysicsMaterial(1,.95f,1));
    body->setDynamic(false);
    drawNode->addComponent(body);
    drawNode->setPosition(Vec2(0, 0));
    this->addChild(drawNode);
    drawNode = DrawNode::create();
    drawNode->setContentSize(cocos2d::Size(3, screenSize.height));
    drawNode->drawSolidRect(Vec2(0, 0), Vec2(3, screenSize.height), Color4F::RED);
    body = PhysicsBody::createBox(Size(3, screenSize.height), PhysicsMaterial(1, .95f, 1));
    //body->setCategoryBitmask(CAT_MASK_STAT);
    //body->setCollisionBitmask(CAT_MASK_STAT);
    body->setDynamic(false);
    drawNode->addComponent(body);
    this->addChild(drawNode);
    
    //create the ball
    float offset = BALL_RESET_OFFSET_Y;
    if (cocos2d::rand_0_1() > 0.5) {
        offset = -BALL_RESET_OFFSET_Y;
    }
    auto p = Vec2(screenCenter.x, screenCenter.y+offset);
    _ball = Ball::create(colors.begin()[0]);
    _ball->setPosition(p);
    
    body = PhysicsBody::createCircle(DEFAULT_BALL_RADIUS, PhysicsMaterial());
    body->setGravityEnable(true);
    //body->setCategoryBitmask(CAT_MASK_BALL);
    //body->setCollisionBitmask(CAT_MASK_PADDLE);
    _ball->addComponent(body);
    this->addChild(_ball);
    
    //create the controls and paddles
    for (int i = 0; i < SHARED_MAX_PLAYERS; i++) {
        _button[i] = GameButton::create();
        if (numberOfPlayers < 3 && i == SHARED_PLAYER3) {
            _button[i]->changeColor(colors.begin()[SHARED_PLAYER2]);
        }
        else if (numberOfPlayers < 4 && i == SHARED_PLAYER4) {
            _button[i]->changeColor(colors.begin()[SHARED_PLAYER1]);
        }
        else {
            _button[i]->changeColor(colors.begin()[i]);
        }
        _button[i]->setPlayer(i);
        _button[i]->setPosition(buttonPos.begin()[i]);
        _button[i]->setTag(i);  //Set the number to indicate button order.
        _button[i]->addTouchEventListener(CC_CALLBACK_2(Airhockey::onPress, this));
        
        
        body = PhysicsBody::createCircle(DEFAULT_BUTTON_RADIUS, PhysicsMaterial());
        //body->setCategoryBitmask(CAT_MASK_STAT);
        //body->setCollisionBitmask(CAT_MASK_STAT);
        body->setDynamic(false);
        _button[i]->addComponent(body);
        
        
        int yfix = 1;
        int xfix = 1;
        
        if (buttonPos.begin()[i].x > screenCenter.x) {
            xfix = -1;
        }
        if (buttonPos.begin()[i].y > screenCenter.y) {
            yfix = -1;
        }
        
        this->addChild(_button[i]);
    }
    
    //Debug Layer
    //this->addChild(B2DebugDrawLayer::create(this->getScene(), 1), 1000);
    
    this->setName("AirhockeySceneRoot");
    this->initTouchHandling();
    this->scheduleUpdate();
    this->startGame(3);
    
    return true;
}

void Airhockey::updateScore(){
    log("Update score--");
}

void Airhockey::initTouchHandling(){
    
}

void Airhockey::update(float dt){
    
}

void Airhockey::onPress(cocos2d::Ref* r, GameButton::Widget::TouchEventType t){
    
}

void Airhockey::onEnter(){
    
}

void Airhockey::startGame(float s){
    
}