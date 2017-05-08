//
//  Airhockey.cpp
//  microgames
//
//  Created by Work on 19/10/16.
//
//

#include "Shared.h"
#include "Airhockey.hpp"
#include "GameList.h"
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
	GameScene::gameType = GameType::AIRHOCKEY;
    
    //http://www.cocos2d-x.org/wiki/Multi_resolution_support
    
    //auto visibleSize = Director::getInstance()->getVisibleSize();
    _screenOrigin = Director::getInstance()->getVisibleOrigin();
    _screenSize = Director::getInstance()->getVisibleSize();
    
    Size winSize = Director::getInstance()->getWinSize();
    _screenCenter = Vec2(winSize.width / 2, winSize.height / 2);

	// Add object to the scene here.
	Shared::instance()->setBackground("bg/BG_CatHokey.png", this);
    
    _world = new b2World(b2Vec2(0.0f, 0.0f));  //Create a physic world.
    _world->SetContactListener(this);
    
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
    
    
    //create the controls and paddles
    for (int i = 0; i < numberOfPlayers; i++) {
        Color4F c;
        if (numberOfPlayers < 3 && i == SHARED_PLAYER3) {
			c = Shared::instance()->getPlayerColor(SHARED_PLAYER2);
        }
        else if (numberOfPlayers < 4 && i == SHARED_PLAYER4) {
            c = Shared::instance()->getPlayerColor(SHARED_PLAYER1);
        }
        else {
			c = Shared::instance()->getPlayerColor(i);
        }
        
        addMallet(i, buttonPos.begin()[i], c);
    }
    
    //Debug Layer
#ifdef DEBUG_MODE
    log("DEBUG MODE!!!");
    b2Draw *debugDraw = new GLESDebugDraw(AR_PTM_RATIO);
    debugDraw->SetFlags(GLESDebugDraw::e_shapeBit || GLESDebugDraw::e_jointBit);
    _world->SetDebugDraw(debugDraw);
    auto debugLayer = B2DebugDrawLayer::create(_world, AR_PTM_RATIO);
    debugLayer->setTag(12345);
    this->addChild(debugLayer, 1000);
#endif
    
    this->setName("AirhockeySceneRoot");
    this->scheduleUpdate();
    this->startGame(SHARED_COUNTDOWN_LENGTH);
    
    return true;
}

void Airhockey::updateScore(){
    if ((long)_scoreBottom->getUserData() >= AR_MAX_SCORE) {
        if (numberOfPlayers > 2)
        {
            int winners[] = AH_TEAM_BOT_PLAYERS;
            endGame(winners, 2);
        }
        else {
            endGame(SHARED_PLAYER2);
        }
    } else if ((long)_scoreTop->getUserData() >= AR_MAX_SCORE) {
        if (numberOfPlayers > 3)
        {
            int winners[] = AH_TEAM_TOP_PLAYERS;
            endGame(winners,2);
        }
        else {
            endGame(SHARED_PLAYER1);
        }
    }
    
    _scoreTop->setString(Shared::intToString((int)((long)_scoreTop->getUserData())));
    _scoreBottom->setString(Shared::intToString((int)((long)_scoreBottom->getUserData())));
}


void Airhockey::update(float dt){
    int positionIterations = 10;
    int velocityIterations = 10;
    
    _world->Step(dt, velocityIterations, positionIterations);
    
    for (b2Body *body = _world->GetBodyList(); body != NULL; body = body->GetNext()) {
        if (body->GetUserData())
        {
            auto sprite = (DrawNode*)body->GetUserData();
            sprite->setPosition(Vec2(body->GetPosition().x * AR_PTM_RATIO, body->GetPosition().y * AR_PTM_RATIO));
            sprite->setRotation(-1 * CC_RADIANS_TO_DEGREES(body->GetAngle()));
            
        }
    }
    _world->DrawDebugData();
    
    checkForGoal();
}

void Airhockey::onPress(cocos2d::Ref* sender, GameButton::Widget::TouchEventType type){
    auto button = static_cast<GameButton*>(sender);
    if (this->gameStatus!=GAME_INPROGRESS ||(button->getActionTag() == 10))return;
    
    switch (type)
    {
        case ui::Widget::TouchEventType::BEGAN:{
            button->setActionTag(AR_PADDLE_DRAG);
            break;
        }
        case ui::Widget::TouchEventType::ENDED:{
            if (button->getActionTag() != 5) { break; }//was never primed properly eg pressed before game start
            button->setActionTag(AR_PADDLE_DROP);            
            break;
        }
        default:
            break;
    }
}

void Airhockey::onGameStart(){
    this->initTouchHandling();
}

void Airhockey::initTouchHandling(){
    auto touchListener = EventListenerTouchAllAtOnce::create();

    touchListener->onTouchesBegan = [this](const std::vector<Touch *> & 	touches,
                                           Event * 	event){
		//if (gameStatus != GAME_INPROGRESS) {
		//	return;
		//}
        auto touchSurface = static_cast<Airhockey*>(event->getCurrentTarget());
        auto objects = touchSurface->getChildren();    //all objects in the SceneRoot
        
        //Loop through each touch
        for(auto iter=touches.rbegin(); iter != touches.rend(); ++iter){
            Touch* t = static_cast<Touch*>(*iter);
            Point touchPoint = touchSurface->convertTouchToNodeSpace(t); //re calculate position to the
            
            //Loop through each object on the scene
            for(auto obj = objects.rbegin(); obj != objects.rend(); obj++){
                GameButton* btn = dynamic_cast<GameButton*>(*obj);
                
                if (btn && btn->getBoundingBox().containsPoint(touchPoint)){
#ifdef DEBUG_MODE
                    log("Button %i is touched at position %.2f,%.2f", btn->getTag(), btn->getPosition().x, btn->getPosition().y);
#endif
                    
                    btn->setTouch(t);
                    
                    if (btn->getUserData()){
                        
                        auto body = (b2Body*)btn->getUserData();
                        
                        b2MouseJointDef md;
                        md.bodyA = this->_boxBody;
                        md.bodyB = body;
                        md.target = body->GetWorldCenter();
                        md.collideConnected = true;
                        md.maxForce = 10000.0f * body->GetMass(); //more force, less bouncing effect.
                        
                        md.frequencyHz = 1000;
                        
                        btn->setb2MouseJoint((b2MouseJoint *)_world->CreateJoint(&md));
                        body->SetAwake(true);
                    }
                }
            }
        }
        
    };
    
    
    touchListener->onTouchesMoved = [this](const std::vector<Touch *> & touches,
                                           Event * 	event){
        //Loop through each touch
        for(auto iter=touches.rbegin(); iter != touches.rend(); ++iter){
            Touch* t = static_cast<Touch*>(*iter);
            
            //For each button test it agianst the touch.
            for (int i=0;i<this->numberOfPlayers;i++){
                auto b = this->_button[i];
                
                //Test if the touch is snapped on the button
                if (b->getTouch() && b->getTouch() == t){
                    auto tap = t->getLocation();
                    
                    b2Vec2 pos = b2Vec2(tap.x/AR_PTM_RATIO,tap.y/AR_PTM_RATIO);
                    b->getb2MouseJoint()->SetTarget(pos);
                    break;
                }
            }
        }
    };
    
    touchListener->onTouchesEnded = [this](const std::vector<Touch *> & touches,
                                           Event * 	event){
        //Loop through each touch
        for(auto iter=touches.rbegin(); iter != touches.rend(); ++iter){
            Touch* t = static_cast<Touch*>(*iter);

            for (int i=0;i<this->numberOfPlayers;i++){
                auto b = this->_button[i];
                
                //Test if the touch is snapped on the button
                if (b->getTouch() && b->getTouch() == t){
                    b->setTouch(NULL);
                    if (b->getb2MouseJoint()){
                        _world->DestroyJoint(b->getb2MouseJoint());
                        b->setb2MouseJoint(NULL);
                        break;
                    }
                }
            }
        }
    };
    
    // Add listener
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    
}

void Airhockey::onEnter(){
    Node::onEnter();  //If you forgot this touch won't work.
}

void Airhockey::startGame(float s){
    GameScene::startGame(SHARED_COUNTDOWN_LENGTH);
}

void Airhockey::createWall() {
    Vec2 origin = Director::getInstance()->getVisibleOrigin() * 1.0/AR_PTM_RATIO;
    Size visibleSize = Director::getInstance()->getVisibleSize() * 1.0/AR_PTM_RATIO;
    
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
    
    
    //Create goal lines
    auto goal_size = AR_GOAL_SIZE;// respect to visibleWidth
    auto left = (1-goal_size)/2*visibleSize.width;
    auto right = (1-(1-goal_size)/2)*visibleSize.width;
    groundBoxDef.userData = (void*)123;
    groundBox.Set(b2Vec2(left, 0.1f), b2Vec2(right, 0.1f)); //bottom goal
    _bottomGoal = _boxBody->CreateFixture(&groundBoxDef);
    
    groundBoxDef.userData = (void*)321;
    groundBox.Set(b2Vec2(left, visibleSize.height-0.1f), b2Vec2(right, visibleSize.height-0.1f)); //top goal
    _topGoal = _boxBody->CreateFixture(&groundBoxDef);
    
    
    //Draw center line
    groundBox.Set(b2Vec2(0, visibleSize.height/2), b2Vec2(visibleSize.width, visibleSize.height/2)); //center line
    groundBoxDef.filter.maskBits = ~AR_BIT_MASK_PUCK;  //Allow PUCK to pass
    
    _boxBody->CreateFixture(&groundBoxDef);
}

void Airhockey::drawBoard(){
    //Draw Board frame
    _drawNode->drawRect(_screenOrigin+Vec2(2,2), Vec2(_screenSize)+_screenOrigin-Vec2(4,4), Color4F::GRAY);

    //_drawNode->drawLine(Vec2(_screenOrigin.x, _screenCenter.y), Vec2(_screenSize.width+_screenOrigin.x, _screenCenter.y), Color4F::GRAY);   //Add center line
    
    auto goal_size = AR_GOAL_SIZE;// respect to visibleWidth
    auto left = (1-goal_size)/2*_screenSize.width;
    auto right = (1-(1-goal_size)/2)*_screenSize.width;
    
    _drawNode->drawLine(_screenOrigin+Vec2(left,2), _screenOrigin+Vec2(right,2), Color4F::BLACK);
    _drawNode->drawLine(_screenOrigin+Vec2(left,_screenSize.height-4), _screenOrigin+Vec2(right,_screenSize.height-4), Color4F::BLACK);
    
    
    addScores();
}

void Airhockey::addScores(){
    _scoreBottom = Label::createWithBMFont(SHARED_FONT_FILE, "");
    _scoreBottom->setBMFontSize(64);
    _scoreBottom->setPosition(Vec2(_screenOrigin.x+32, _screenCenter.y - 50));
    _scoreBottom->setRotation(90);
    _scoreBottom->setUserData(0); //Use UserData to store score
    this->addChild(_scoreBottom, 1);
    
    _scoreTop = Label::createWithBMFont(SHARED_FONT_FILE, "");
    _scoreTop->setBMFontSize(64);
    _scoreTop->setPosition(Vec2(_screenOrigin.x+32, _screenCenter.y + 50));
    _scoreTop->setRotation(90);
    _scoreTop->setUserData(0); //Use UserData to store score
    this->addChild(_scoreTop, 1);
    updateScore();
}

void Airhockey::placePuck(){
    //create the ball
    _ball = Ball::create();
	_ball->setBallImage("item/Item_Cat Hockey_Hockey.png");
    _ball->setPosition(Vec2(_screenCenter.x, _screenCenter.y));
    this->addChild(_ball);
    
    // Create _ballBody
    b2BodyDef ballBodyDef;
    ballBodyDef.type = b2_dynamicBody;
    auto ballPosInWorld = _ball->getPosition()/AR_PTM_RATIO;
    ballBodyDef.position.Set(ballPosInWorld.x, ballPosInWorld.y);
    ballBodyDef.userData = _ball;
    ballBodyDef.bullet = true; //Enable CCD
    _ballBody = _world->CreateBody(&ballBodyDef);
    
    // Define a shape for the _ballBody(tangible)
    b2CircleShape circle;
    circle.m_radius = _ball->getContentSize().width/2/AR_PTM_RATIO;
    
    // Create shape definition and add to body
    b2FixtureDef ballShapeDef;
    ballShapeDef.shape = &circle;
    ballShapeDef.density = 1.0f;
    ballShapeDef.friction = 0.9f;
    ballShapeDef.restitution = 0.5f;
    ballShapeDef.filter.categoryBits = AR_BIT_MASK_PUCK;   //Set the shape to belong to PUCK cat
    
    _ballFixture = _ballBody->CreateFixture(&ballShapeDef);
    _ballBody->SetLinearDamping(0.3f);  //How fast the ball losing its velocity
    _ballBody->SetAngularDamping(0.2f); //How fast it loses angular speed
}

void Airhockey::addMallet(int playerNo, Vec2 pos, Color4F color){
    _button[playerNo] = GameButton::create();
    auto m = _button[playerNo];
    
    m->setPlayer(playerNo);
    m->setPosition(pos);
	m->changeFormat("Hockey");
	m->setScale(0.5);
    m->setTag(playerNo);  //Set the number to indicate button order.
    m->setActionTag(AR_PADDLE_DROP);
    m->setPressedActionEnabled(false); // Disable zoom action on pressed
    m->setZoomScale(0.0f);
    m->addTouchEventListener(CC_CALLBACK_2(Airhockey::onPress, this));
    //setSwallow to 'false' to pass touch event to gamescene
    m->setSwallowTouches(false);
    
    
    // Create _buttonBody
    b2BodyDef btnBodyDef;
    btnBodyDef.type = b2_dynamicBody;
    auto btnPosInWorld = m->getPosition()/AR_PTM_RATIO;
    btnBodyDef.position.Set(btnPosInWorld.x, btnPosInWorld.y);
    btnBodyDef.userData = m;
    btnBodyDef.bullet = true; //Enable CCD
    auto btnBody = _world->CreateBody(&btnBodyDef);
    
    // Define a shape for the _ballBody(tangible)
    b2CircleShape circle;
    circle.m_radius = m->getContentSize().width/4/AR_PTM_RATIO;
    
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

void Airhockey::resetGame(float dt){
    _needReset = false;
    _world->ClearForces();
    _ball->setPosition(Vec2(_screenCenter.x, _screenCenter.y));
    auto ballPosInWorld = _ball->getPosition()/AR_PTM_RATIO;
    _ballBody->SetLinearVelocity(b2Vec2(0,0));
    _ballBody->SetAngularVelocity(0.f);
    _ballBody->SetTransform(b2Vec2(ballPosInWorld.x, ballPosInWorld.y),0);
    _ball->setVisible(true);
    _ball->runAction(Blink::create(0.5f, 2));
    this->scheduleUpdate();
}

void Airhockey::checkForGoal(){
    if(_needReset){
        _ball->setVisible(false);
        this->scheduleOnce(schedule_selector(Airhockey::resetGame), 0.3f);
        this->unscheduleUpdate();
    }
    _needReset = false;
}

void Airhockey::BeginContact(b2Contact* contact) {
    if (contact->GetFixtureA() == _ballFixture || contact->GetFixtureB() == _ballFixture){
        
        if (contact->GetFixtureA() == _topGoal || contact->GetFixtureA() == _bottomGoal){
            auto score = (contact->GetFixtureA() == _topGoal) ? _scoreBottom : _scoreTop;
            score->setUserData((void*)((long)score->getUserData()+1));
            updateScore();
            _needReset = true;
            SoundManager::instance()->playEffect(SOUND_FILE_WIN);

            b2WorldManifold worldManifold;
            contact->GetWorldManifold(&worldManifold);
            b2Vec2 vel1 = contact->GetFixtureA()->GetBody()->GetLinearVelocityFromWorldPoint( worldManifold.points[0] );
            b2Vec2 vel2 = contact->GetFixtureB()->GetBody()->GetLinearVelocityFromWorldPoint( worldManifold.points[0] );
            b2Vec2 impactVelocity = vel1 - vel2;
            if (impactVelocity.LengthSquared() > 5000){
                auto fireParticle = ParticleFlower::create();
                fireParticle->setPosition(_ball->getPosition());
                fireParticle->setLife(0.5f);
                fireParticle->setDuration(0.5f);
                this->addChild(fireParticle);
                fireParticle->setAutoRemoveOnFinish(true);
            }
#ifdef DEBUG_MODE
                log("Impact strength %.3f", impactVelocity.LengthSquared());
#endif
        } else {
            SoundManager::instance()->playEffect(SOUND_FILE_HOCKEY_PUCK);
            
        }
        
        
    }
}

void Airhockey::EndContact(b2Contact* contact) {
    
}
