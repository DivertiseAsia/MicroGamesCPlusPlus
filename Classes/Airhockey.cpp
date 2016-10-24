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
    _screenOrigin = Director::getInstance()->getVisibleOrigin();
    _screenSize = Director::getInstance()->getVisibleSize();
    
    Size winSize = Director::getInstance()->getWinSize();
    _screenCenter = Vec2(winSize.width / 2, winSize.height / 2);
    
    _world = new b2World(GRAVITY);  //Create a physic world.
    
    //Create drawNode and draw the center line
    _drawNode = DrawNode::create(10);    //Default line width
    this->addChild(_drawNode);
    
    drawBoard();
    createWall();
    placePuck();
    
    Vec2 buttonOffset = Vec2(80,80);
    //Position should be based on visibleOrigin and visibleSize properties.
    auto buttonPos = {
        Vec2(_screenOrigin.x+buttonOffset.x,_screenOrigin.y + _screenSize.height-buttonOffset.y),  //0
        Vec2(_screenOrigin.x + _screenSize.width - buttonOffset.x, _screenOrigin.y+ buttonOffset.y),  //1
        _screenOrigin+buttonOffset, //2
        _screenOrigin + _screenSize - buttonOffset}; //3
    auto colors = SHARED_COLOR_PLAYERS;
    
    
    //create the controls and paddles
    for (int i = 0; i < numberOfPlayers; i++) {
        Color4F c;
        if (numberOfPlayers < 3 && i == SHARED_PLAYER3) {
            c = colors.begin()[SHARED_PLAYER2];
        }
        else if (numberOfPlayers < 4 && i == SHARED_PLAYER4) {
            c = colors.begin()[SHARED_PLAYER1];
        }
        else {
            c = colors.begin()[i];
        }
        
        addMallet(i, buttonPos.begin()[i], c);
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
                    //auto locationWorld = b2Vec2(touchPosition.x/PTM_RATIO, touchPosition.y/PTM_RATIO);
                    
                    b2MouseJointDef md;
                    md.bodyA = this->_boxBody;
                    md.bodyB = body;
                    md.target = body->GetWorldCenter();
                    md.collideConnected = true;
                    md.maxForce = 10000.0f * body->GetMass(); //more force, less bouncing effect.
                    
                    md.frequencyHz = 1000;
                    
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
    Vec2 origin = Director::getInstance()->getVisibleOrigin() * 1.0/PTM_RATIO;
    Size visibleSize = Director::getInstance()->getVisibleSize() * 1.0/PTM_RATIO;
    
    auto margin = 0.1;
    
    origin += Vec2(+margin,+margin);
    visibleSize = visibleSize - Size(margin*2,margin*2);
    
    // Create edges around the entire screen
    b2BodyDef boxBodyDef;
    boxBodyDef.position.Set(origin.x,origin.y);
    _boxBody = _world->CreateBody(&boxBodyDef);
    
    b2EdgeShape groundBox;
    b2FixtureDef groundBoxDef;
    groundBoxDef.shape = &groundBox;
    
    groundBox.Set(b2Vec2(0,0), b2Vec2(visibleSize.width, 0));    //bottom
    _boxBody->CreateFixture(&groundBoxDef);
    
    groundBox.Set(b2Vec2(0,0), b2Vec2(0, visibleSize.height));   //left
    _boxBody->CreateFixture(&groundBoxDef);
    
    groundBox.Set(b2Vec2(0, visibleSize.height), b2Vec2(visibleSize.width,visibleSize.height));//top
    _boxBody->CreateFixture(&groundBoxDef);
    
    groundBox.Set(b2Vec2(visibleSize.width, visibleSize.height), b2Vec2(visibleSize.width, 0)); //right
    _boxBody->CreateFixture(&groundBoxDef);
    
    
    groundBox.Set(b2Vec2(0, visibleSize.height/2), b2Vec2(visibleSize.width, visibleSize.height/2)); //center line
    groundBoxDef.filter.maskBits = ~BIT_MASK_PUCK;  //Allow PUCK to pass
    
    _boxBody->CreateFixture(&groundBoxDef);
}

void Airhockey::drawBoard(){
    //Draw Board frame
    _drawNode->drawRect(_screenOrigin+Vec2(2,2), Vec2(_screenSize)+_screenOrigin-Vec2(4,4), Color4F::GRAY);

    _drawNode->drawLine(Vec2(_screenOrigin.x, _screenCenter.y), Vec2(_screenSize.width+_screenOrigin.x, _screenCenter.y), Color4F::GRAY);   //Add center line
    
    addScores();
}

void Airhockey::addScores(){
    _scoreBottom = Label::createWithBMFont(SHARED_FONT_FILE_INGAME, "");
    _scoreBottom->setBMFontSize(64);
    _scoreBottom->setPosition(Vec2(50, _screenCenter.y - 50));
    _scoreBottom->setRotation(90);
    _scoreBottom->setUserData(0); //Use UserData to store score
    this->addChild(_scoreBottom, 1);
    
    _scoreTop = Label::createWithBMFont(SHARED_FONT_FILE_INGAME, "");
    _scoreTop->setBMFontSize(64);
    _scoreTop->setPosition(Vec2(50, _screenCenter.y + 50));
    _scoreTop->setRotation(90);
    _scoreTop->setUserData(0); //Use UserData to store score
    this->addChild(_scoreTop, 1);
    updateScore();
}

void Airhockey::placePuck(){
    //create the ball
    float offset = BALL_RESET_OFFSET_Y;
    if (cocos2d::rand_0_1() > 0.5) {
        offset = -BALL_RESET_OFFSET_Y;
    }
    auto colors = SHARED_COLOR_PLAYERS;
    _ball = Ball::create(colors.begin()[0]);
    _ball->setPosition(Vec2(_screenCenter.x, _screenCenter.y+offset));
    this->addChild(_ball);
    
    // Create _ballBody
    b2BodyDef ballBodyDef;
    ballBodyDef.type = b2_dynamicBody;
    auto ballPosInWorld = _ball->getPosition()/PTM_RATIO;
    ballBodyDef.position.Set(ballPosInWorld.x, ballPosInWorld.y);
    ballBodyDef.userData = _ball;
    ballBodyDef.bullet = true; //Enable CCD
    _ballBody = _world->CreateBody(&ballBodyDef);
    
    // Define a shape for the _ballBody(tangible)
    b2CircleShape circle;
    circle.m_radius = _ball->getContentSize().width/2/PTM_RATIO;
    
    // Create shape definition and add to body
    b2FixtureDef ballShapeDef;
    ballShapeDef.shape = &circle;
    ballShapeDef.density = 1.0f;
    ballShapeDef.friction = 0.9f;
    ballShapeDef.restitution = 0.5f;
    ballShapeDef.filter.categoryBits = BIT_MASK_PUCK;   //Set the shape to belong to PUCK cat
    
    _ballBody->CreateFixture(&ballShapeDef);
    _ballBody->SetLinearDamping(0.3f);  //How fast the ball losing its velocity
    _ballBody->SetAngularDamping(0.2f); //How fast it loses angular speed
}

void Airhockey::addMallet(int playerNo, Vec2 pos, Color4F color){
    _button[playerNo] = GameButton::create();
    auto m = _button[playerNo];
    
    m->changeColor(color);
    m->setRadius(40);
    m->setPlayer(playerNo);
    m->setPosition(pos);
    m->setTag(playerNo);  //Set the number to indicate button order.
    m->setActionTag(PADDLE_DROP);
    m->setPressedActionEnabled(false); // Disable zoom action on pressed
    m->setZoomScale(0.0f);
    m->addTouchEventListener(CC_CALLBACK_2(Airhockey::onPress, this));
    //setSwallow to 'false' to pass touch event to gamescene
    m->setSwallowTouches(false);
    
    
    // Create _buttonBody
    b2BodyDef btnBodyDef;
    btnBodyDef.type = b2_dynamicBody;
    auto btnPosInWorld = m->getPosition()/PTM_RATIO;
    btnBodyDef.position.Set(btnPosInWorld.x, btnPosInWorld.y);
    btnBodyDef.userData = m;
    btnBodyDef.bullet = true; //Enable CCD
    auto btnBody = _world->CreateBody(&btnBodyDef);
    
    // Define a shape for the _ballBody(tangible)
    b2CircleShape circle;
    circle.m_radius = m->getContentSize().width/2/PTM_RATIO;
    
    // Create shape definition and add to body
    b2FixtureDef btnShapeDef;
    btnShapeDef.shape = &circle;
    btnShapeDef.density = 10.0f;
    btnShapeDef.friction = 1.0f;
    btnShapeDef.restitution = 0.5f;
    btnBody->CreateFixture(&btnShapeDef);
    btnBody->SetAngularDamping(0.6f);
    btnBody->SetLinearDamping(0.7f);
    
    m->setUserData(btnBody);
    
    this->addChild(m);
}
