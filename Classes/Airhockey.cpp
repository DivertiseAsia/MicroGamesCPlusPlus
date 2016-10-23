//
//  Airhockey.cpp
//  microgames
//
//  Created by Work on 19/10/16.
//
//

#include "Shared.h"
#include "Airhockey.hpp"
#include "Box2D/Box2D.h"
#include "GLES-Render/B2DebugDrawLayer.h"

// on "init" you need to initialize your instance
bool Airhockey::init()
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
    auto screenCenter = Vec2(winSize.width / 2, winSize.height / 2);
    
    
    b2Vec2 gravity = b2Vec2(0.0f, -0.2f);    //No gravity
    _world = new b2World(gravity);  //Create a physic world.
    
    
    //Create drawNode and draw the center line
    _drawNode = DrawNode::create(10);    //Default line width
    _drawNode->drawLine(Vec2(0, screenCenter.y), Vec2(winSize.width, screenCenter.y), Color4F::GRAY);
    this->addChild(_drawNode);
    
    Vec2 buttonOffset = Vec2(80,80);
    //Position should be based on visibleOrigin and visibleSize properties.
    auto buttonPos = {
        Vec2(origin.x+buttonOffset.x,origin.y + screenSize.height-buttonOffset.y),  //0
        Vec2(origin.x + screenSize.width - buttonOffset.x, origin.y+ buttonOffset.y),  //1
        origin+buttonOffset, //2
        origin + screenSize - buttonOffset}; //3
    auto colors = SHARED_COLOR_PLAYERS;
    
    scoreBottom = Label::createWithBMFont(SHARED_FONT_FILE_INGAME, "");
    scoreBottom->setBMFontSize(64);
    scoreBottom->setPosition(Vec2(50, screenCenter.y - 50));
    scoreBottom->setRotation(90);
    scoreBottom->setUserData(0); //Use UserData to store score
    this->addChild(scoreBottom, 1);
    
    scoreTop = Label::createWithBMFont(SHARED_FONT_FILE_INGAME, "");
    scoreTop->setBMFontSize(64);
    scoreTop->setPosition(Vec2(50, screenCenter.y + 50));
    scoreTop->setRotation(90);
    scoreTop->setUserData(0); //Use UserData to store score
    this->addChild(scoreTop, 1);
    updateScore();
    
    //create the walls
    auto drawNode = DrawNode::create();
    drawNode->setContentSize(cocos2d::Size(3, screenSize.height));
    drawNode->drawSolidRect(Vec2(0, 0), Vec2(3, screenSize.height), Color4F::RED);
    
    drawNode->setPosition(Vec2(0, 0));
    this->addChild(drawNode);
    drawNode = DrawNode::create();
    drawNode->setContentSize(cocos2d::Size(3, screenSize.height));
    drawNode->drawSolidRect(Vec2(0, 0), Vec2(3, screenSize.height), Color4F::RED);
    
    this->addChild(drawNode);
    
    //create the ball
    float offset = BALL_RESET_OFFSET_Y;
    if (cocos2d::rand_0_1() > 0.5) {
        offset = -BALL_RESET_OFFSET_Y;
    }
    _ball = Ball::create(colors.begin()[0]);
    _ball->setPosition(Vec2(screenCenter.x, screenCenter.y+offset));
    this->addChild(_ball);
    
    // Create _ballBody
    b2BodyDef ballBodyDef;
    ballBodyDef.type = b2_dynamicBody;
    auto ballPosInWorld = _ball->getPosition()/PTM_RATIO;
    ballBodyDef.position.Set(ballPosInWorld.x, ballPosInWorld.y);
    ballBodyDef.userData = _ball;
    _ballBody = _world->CreateBody(&ballBodyDef);
    
    // Define a shape for the _ballBody(tangible)
    b2CircleShape circle;
    circle.m_radius = _ball->getContentSize().width/2/PTM_RATIO;
    
    // Create shape definition and add to body
    b2FixtureDef ballShapeDef;
    ballShapeDef.shape = &circle;
    ballShapeDef.density = 1.0f;
    ballShapeDef.friction = 0.f;
    ballShapeDef.restitution = 1.0f;
    _ballBody->CreateFixture(&ballShapeDef);
    
    //////////////////////////////////////////
    //Create world bounds
    /*
    b2FixtureDef boxFixture;
    boxFixture.density = 0;
    boxFixture.friction = 0;
    boxFixture.restitution = 1;
    b2PolygonShape boxShape;
    boxShape.SetAsBox(screenSize.width/2 / PTM_RATIO, screenSize.height/2/ PTM_RATIO);
    boxFixture.shape = &boxShape;
    
    b2BodyDef boxBodyDef;
    boxBodyDef.type = b2BodyType::b2_staticBody;
    //Position has to be calculated by "visible origin" or "screenCenter".
    auto center= origin+screenSize/2;
    auto bottom = origin+Vec2(screenSize.width/2,-screenSize.height/2+10); //+10 for offset to be visible
    boxBodyDef.position.Set(bottom.x/PTM_RATIO,bottom.y/PTM_RATIO);
    auto boxBody1 = _world->CreateBody(&boxBodyDef);
    boxBody1->CreateFixture(&boxFixture);
    _boxBody=boxBody1;
    
    auto top = origin+Vec2(screenSize.width/2,screenSize.height+screenSize.height/2-10); //-10 for offset to be visible
    boxBodyDef.position.Set(top.x/PTM_RATIO, top.y/PTM_RATIO);
    _world->CreateBody(&boxBodyDef)->CreateFixture(&boxFixture);
    
    auto left = origin+Vec2(-screenSize.width/2+10,screenSize.height/2); //+10 for offset to be visible
    boxBodyDef.position.Set(left.x/PTM_RATIO, left.y/PTM_RATIO);
    _world->CreateBody(&boxBodyDef)->CreateFixture(&boxFixture);
    
    auto right = origin+Vec2(screenSize.width+screenSize.width/2-10,screenSize.height/2); //-10 for offset to be visible
    boxBodyDef.position.Set(right.x/PTM_RATIO, right.y/PTM_RATIO);
    _world->CreateBody(&boxBodyDef)->CreateFixture(&boxFixture);
    */

    //////////////////////////////////////////
    
    createWall();
    
    
    //create the controls and paddles
    for (int i = 0; i < numberOfPlayers; i++) {
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
        _button[i]->setRadius(50);
        _button[i]->setPlayer(i);
        _button[i]->setPosition(buttonPos.begin()[i]);
        _button[i]->setTag(i);  //Set the number to indicate button order.
        _button[i]->setActionTag(PADDLE_DROP);
        _button[i]->addTouchEventListener(CC_CALLBACK_2(Airhockey::onPress, this));
        //setSwallow to 'false' to pass touch event to gamescene
        _button[i]->setSwallowTouches(false);
        
        
        // Create _buttonBody
        b2BodyDef btnBodyDef;
        btnBodyDef.type = b2_dynamicBody;
        auto btnPosInWorld = _button[i]->getPosition()/PTM_RATIO;
        btnBodyDef.position.Set(btnPosInWorld.x, btnPosInWorld.y);
        btnBodyDef.userData = _button[i];
        auto btnBody = _world->CreateBody(&btnBodyDef);
        
        // Define a shape for the _ballBody(tangible)
        b2CircleShape circle;
        circle.m_radius = _button[i]->getContentSize().width/2/PTM_RATIO;
        
        // Create shape definition and add to body
        b2FixtureDef btnShapeDef;
        btnShapeDef.shape = &circle;
        btnShapeDef.density = 1.0f;
        btnShapeDef.friction = 0.f;
        btnShapeDef.restitution = 1.0f;
        btnBody->CreateFixture(&btnShapeDef);
        
        _button[i]->setUserData(btnBody);
        
        this->addChild(_button[i]);
    }
    
    //Debug Layer
#ifdef DEBUG_MODE
    log("DEBUG MODE!!!");
    b2Draw *debugDraw = new GLESDebugDraw(PTM_RATIO);
    debugDraw->SetFlags(GLESDebugDraw::e_shapeBit || GLESDebugDraw::e_jointBit);
    _world->SetDebugDraw(debugDraw);
    auto debugLayer = B2DebugDrawLayer::create(_world, PTM_RATIO);
    debugLayer->setTag(12345);
    this->addChild(debugLayer, 1000);
#endif
    
    this->setName("AirhockeySceneRoot");
    this->initTouchHandling();
    this->scheduleUpdate();
    this->startGame(3);
    
    return true;
}

void Airhockey::updateScore(){
    log("Update score--");
}


void Airhockey::update(float dt){
    int positionIterations = 10;
    int velocityIterations = 10;
    
    
    _world->Step(dt, velocityIterations, positionIterations);
    
    for (b2Body *body = _world->GetBodyList(); body != NULL; body = body->GetNext()) {
        if (body->GetUserData())
        {
            auto sprite = (DrawNode*)body->GetUserData();
            sprite->setPosition(Vec2(body->GetPosition().x * PTM_RATIO, body->GetPosition().y * PTM_RATIO));
            sprite->setRotation(-1 * CC_RADIANS_TO_DEGREES(body->GetAngle()));
            
        }
    }
    _world->DrawDebugData();
}

void Airhockey::onPress(cocos2d::Ref* sender, GameButton::Widget::TouchEventType type){
    auto button = static_cast<GameButton*>(sender);
    if (this->gameStatus!=GAME_INPROGRESS ||(button->getActionTag() == 10))return;
    
    switch (type)
    {
        case ui::Widget::TouchEventType::BEGAN:{
            //button->getBall()->setAcceleration(Vec2(0,-BALL_ACCELERATION));
            button->setActionTag(PADDLE_DRAG);
            break;
        }
        case ui::Widget::TouchEventType::ENDED:{
            if (button->getActionTag() != 5) { break; }//was never primed properly eg pressed before game start
            button->setActionTag(PADDLE_DROP);
            
            //SoundManager::instance()->playEffect(SOUND_FILE_INGAME_PRESS);
            
            break;
        }
        default:
            break;
    }
}
void Airhockey::initTouchHandling(){
    auto listener1 = EventListenerTouchOneByOne::create();
    
    // trigger when you push down
    listener1->onTouchBegan = [this](Touch* touch, Event* event){
        
        auto parentNode = static_cast<Sprite*>(event->getCurrentTarget());
        
        Vector<Node *> children = parentNode->getChildren();
        Point touchPosition = parentNode->convertTouchToNodeSpace(touch);
        for (auto iter = children.rbegin(); iter != children.rend(); ++iter) {
            GameButton* childNode = static_cast<GameButton*>(*iter);
            log("Locked! touch:%i", touch->getID());
            if (childNode->getTag()!= 12345 && childNode->getBoundingBox().containsPoint(touchPosition)) {
                //childNode is the touched Node
                log(">>%s",childNode->getName().c_str());
                childNode->setTouch(touch);
                
                if (childNode->getUserData()){
                    
                    log(">>CreateJoint!!!<<");
                    auto body = (b2Body*)childNode->getUserData();
                    auto locationWorld = b2Vec2(touchPosition.x/PTM_RATIO, touchPosition.y/PTM_RATIO);
                    
                    b2MouseJointDef md;
                    md.bodyA = this->_boxBody;
                    md.bodyB = body;
                    md.target = locationWorld;
                    md.collideConnected = true;
                    md.maxForce = 1000.0f * body->GetMass();
                    
                    _mouseJoint = (b2MouseJoint *)_world->CreateJoint(&md);
                    body->SetAwake(true);
                }
            }
        }
        
        return true;  //Return 'true' to enable onTouchMoved event
    };

    
    
    // trigger when moving touch
    listener1->onTouchMoved = [this](Touch* touch, Event* event){
        
        for(int i=0;i<this->numberOfPlayers;i++){
            auto b = this->_button[i];
            if (b->getTouch() != NULL && b->getTouch() == touch){
                log("moved! touch:%i", touch->getID());
                auto tap = touch->getLocation();
                
                if (b->getUserData()){
                    //auto body = (b2Body*)b->getUserData();
                    b2Vec2 pos = b2Vec2(tap.x/PTM_RATIO,tap.y/PTM_RATIO);
                    //body->SetTransform(pos, 0);
                    //auto f = touch->getLocation() - touch->getPreviousLocation();
                    //b2Vec2 force = b2Vec2(f.x, f.y);
                    //body->ApplyLinearImpulse(force, body->GetWorldCenter(),true);
                    //log("Apply force %.2f,%.2f", force.x, force.y);
                    _mouseJoint->SetTarget(pos);
                }else{
                    b->setPosition(tap);
                }
            }
        }
        
    };
    
    listener1->onTouchEnded = [this](Touch* touch, Event* event){
        for(int i=0;i<this->numberOfPlayers;i++){
            auto b = this->_button[i];
            if (b->getTouch() != NULL && b->getTouch() == touch){
                b->setTouch(NULL);
                log("UnLocked!touch:%i", touch->getID());
                if (_mouseJoint) {
                    _world->DestroyJoint(_mouseJoint);
                    _mouseJoint = NULL;
                }
            }
        }
        
    };
    
    // Add listener
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, this);
    
}

void Airhockey::onEnter(){
    Node::onEnter();  //If you forgot this touch won't work.
    //startGame();
}

void Airhockey::startGame(float s){
    GameScene::startGame(SHARED_COUNTDOWN_LENGTH);
}

void Airhockey::createWall() {
    //auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Size winSize = Director::getInstance()->getWinSize();
    auto screenCenter = Vec2(winSize.width / 2, winSize.height / 2);
    
    // Create edges around the entire screen
    b2BodyDef boxBodyDef;
    boxBodyDef.position.Set(0,0);
    _boxBody = _world->CreateBody(&boxBodyDef);
    
    b2EdgeShape groundBox;
    b2FixtureDef groundBoxDef;
    groundBoxDef.shape = &groundBox;
    
    groundBox.Set(b2Vec2(0,0), b2Vec2(winSize.width/PTM_RATIO, 0));
    _boxBody->CreateFixture(&groundBoxDef);
    
    groundBox.Set(b2Vec2(0,0), b2Vec2(0, winSize.height/PTM_RATIO));
    _boxBody->CreateFixture(&groundBoxDef);
    
    groundBox.Set(b2Vec2(0, winSize.height/PTM_RATIO), b2Vec2(winSize.width/PTM_RATIO,
                                                              winSize.height/PTM_RATIO));
    _boxBody->CreateFixture(&groundBoxDef);
    
    groundBox.Set(b2Vec2(winSize.width/PTM_RATIO, winSize.height/PTM_RATIO),
                  b2Vec2(winSize.width/PTM_RATIO, 0));
    _boxBody->CreateFixture(&groundBoxDef);
}
