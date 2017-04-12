//
//  SharedBall.cpp
//  microgames
//
//  Created by Work on 13/9/16.
//
//

#include "SharedBall.h"
#include "GameList.h"
#include "GLES-Render/B2DebugDrawLayer.h"

USING_NS_CC;

void SharedBall::createBall(Vec2 p, float radius, std::string fname) {
	_ball = DrawNode::create();
	_ball->setContentSize(cocos2d::Size(radius * 2, radius * 2));
	//_ball->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_ball->setPosition(p);
	auto ballImg = Sprite::create(fname);
	ballImg->setScale(0.4f);
	_ball->addChild(ballImg);
	this->addChild(_ball);

	b2FixtureDef ballFixture;
	ballFixture.density = SB_DEFAULT_DENSITY;
	ballFixture.friction = SB_DEFAULT_FRICTION;
	ballFixture.restitution = SB_DEFAULT_RESTITUTION;
	b2CircleShape ballShape;
	ballShape.m_radius = radius / SB_SCALE_RATIO;
	ballFixture.shape = &ballShape;
	b2BodyDef ballBodyDef;
	ballBodyDef.type = b2BodyType::b2_dynamicBody;
	ballBodyDef.userData = _ball;
	ballBodyDef.position.Set(_ball->getPosition().x / SB_SCALE_RATIO, _ball->getPosition().y / SB_SCALE_RATIO);

	ballBody = world->CreateBody(&ballBodyDef);
	ballBody->CreateFixture(&ballFixture);
	ballBody->SetBullet(true);
	ballBody->SetGravityScale(1);
}

void SharedBall::setupScore(int startScore, Vec2 position1, Vec2 position2) {
	_score[SB_TEAM_TOP] = startScore;
	_score[SB_TEAM_BOT] = startScore;

	scoreBottom = Label::createWithBMFont(SHARED_FONT_FILE, "");
	scoreBottom->setBMFontSize(64);
	scoreBottom->setPosition(position1);
	scoreBottom->setRotation(90);
	this->addChild(scoreBottom, 1);

	scoreTop = Label::createWithBMFont(SHARED_FONT_FILE, "");
	scoreTop->setBMFontSize(64);
	scoreTop->setPosition(position2);
	scoreTop->setRotation(90);
	this->addChild(scoreTop, 1);
	updateScore();
}

void SharedBall::onGameStart() {
	this->scheduleUpdate();
}

GameButton* SharedBall::addButtonForPlayer(int player, int game) {
	auto button = GameButton::create();
	std::string format = "Paw";
	if(game == GameType::PINBALL)
		format = "Blank";
	button->setPlayer(player);
	if (numberOfPlayers == 2 && player > SHARED_PLAYER2) {
		button->changeFormat("2" + format);
	}
	else if (numberOfPlayers == 3 && player > SHARED_PLAYER3) {
		button->changeFormat("3" + format);
	}
	else {
		button->changeFormat(format);
	}
	button->setScale(0.5);

	button->setPosition(Shared::instance()->getPlayerPosition(player));
	button->setAnchorPoint(Shared::instance()->getPlayerAnchor(player));
	button->setTag(player);  //Set the number to indicate button order.
	button->addTouchEventListener(CC_CALLBACK_2(SharedBall::onPress, this));
	this->addChild(button);

	b2FixtureDef circleFixture;
	circleFixture.density = SB_DEFAULT_DENSITY;
	circleFixture.friction = SB_DEFAULT_FRICTION;
	circleFixture.restitution = SB_DEFAULT_RESTITUTION;
    circleFixture.filter.categoryBits = SB_BITMASK_BUTTON;  //Assign the fixture to button category.
    b2CircleShape circleShape;
	circleShape.m_radius = DEFAULT_BUTTON_RADIUS / SB_SCALE_RATIO;
	circleFixture.shape = &circleShape;
	b2BodyDef buttonBodyDef;
	buttonBodyDef.type = b2BodyType::b2_staticBody;
	buttonBodyDef.userData = NULL;
	buttonBodyDef.position.Set(button->getPosition().x / SB_SCALE_RATIO, button->getPosition().y / SB_SCALE_RATIO);
    

	auto bb = world->CreateBody(&buttonBodyDef);
	bb->CreateFixture(&circleFixture);

	return button;
}

void SharedBall::onShake(cocos2d::Acceleration *acc, cocos2d::Event *event) {
	if (ballBody->GetLinearVelocity().Length() == 0) {
		ballBody->ApplyLinearImpulse(b2Vec2(acc->x*100, acc->y*100),ballBody->GetWorldCenter(),true);
	}
}

void SharedBall::createWall(Vec2 position, float height) {
	auto drawNode = DrawNode::create();
	drawNode->setContentSize(cocos2d::Size(SB_WALL_WIDTH, height));
	drawNode->drawSolidRect(Vec2::ZERO, Vec2(SB_WALL_WIDTH, height), Color4F::GRAY);
	drawNode->setPosition(position);

	b2FixtureDef boxFixture;
	boxFixture.density = SB_DEFAULT_DENSITY;
	boxFixture.friction = SB_DEFAULT_FRICTION;
	boxFixture.restitution = SB_DEFAULT_RESTITUTION;
	b2PolygonShape boxShape;
	boxShape.SetAsBox(SB_WALL_WIDTH / 2 / SB_SCALE_RATIO, height / SB_SCALE_RATIO);
	boxFixture.shape = &boxShape;
	b2BodyDef boxBodyDef;
	boxBodyDef.type = b2BodyType::b2_staticBody;
	boxBodyDef.userData = drawNode;
	boxBodyDef.position.Set(drawNode->getPosition().x / SB_SCALE_RATIO, drawNode->getPosition().y / SB_SCALE_RATIO);

	auto boxBody1 = world->CreateBody(&boxBodyDef);
	boxBody1->CreateFixture(&boxFixture);
	
	this->addChild(drawNode);
}

void SharedBall::createBox(Vec2 position) {
	auto drawNode = DrawNode::create();
	drawNode->setContentSize(cocos2d::Size(SB_BOX_SIZE, SB_BOX_SIZE));
	drawNode->drawSolidRect(Vec2::ZERO, Vec2(SB_BOX_SIZE, SB_BOX_SIZE), Color4F::GRAY);;
	drawNode->setPosition(position);
	drawNode->setRotation(45);
	drawNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

	b2FixtureDef boxFixture;
	boxFixture.density = SB_DEFAULT_DENSITY;
	boxFixture.friction = SB_DEFAULT_FRICTION;
	boxFixture.restitution = SB_DEFAULT_RESTITUTION;
	b2PolygonShape boxShape;
	boxShape.SetAsBox(SB_BOX_SIZE / 2 / SB_SCALE_RATIO, SB_BOX_SIZE / 2 / SB_SCALE_RATIO);
	boxFixture.shape = &boxShape;
	b2BodyDef boxBodyDef;
	boxBodyDef.type = b2BodyType::b2_staticBody;
	boxBodyDef.userData = drawNode;
	boxBodyDef.angle = CC_DEGREES_TO_RADIANS(drawNode->getRotation());
	boxBodyDef.position.Set(drawNode->getPosition().x / SB_SCALE_RATIO, drawNode->getPosition().y / SB_SCALE_RATIO);

	auto boxBody1 = world->CreateBody(&boxBodyDef);
	boxBody1->CreateFixture(&boxFixture);

	this->addChild(drawNode);
}

void SharedBall::draw(Renderer* renderer, const Mat4& transform, bool transformUpdated) {
	Layer::draw();
#ifdef DEBUG_MODE
	world->DrawDebugData();
#endif //DEBUG_MODE
}

void SharedBall::update(float dt) {
	//this does nothing
	
}

//This method will be called on the Node entered.
void SharedBall::onEnter() {
	Node::onEnter();
	startGame();
}

void SharedBall::startGame() {
	GameScene::startGame(SHARED_COUNTDOWN_LENGTH);
}

void SharedBall::updateScore() {
	scoreTop->setString(Shared::intToString(_score[SB_TEAM_TOP]));
	scoreBottom->setString(Shared::intToString(_score[SB_TEAM_BOT]));
}

//to be overridden
void SharedBall::onPress(cocos2d::Ref *, GameButton::Widget::TouchEventType) {}

void SharedBall::BeginContact(b2Contact* contact) {
    SoundManager::instance()->playEffect(SOUND_FILE_HOCKEY_PUCK);
}

void SharedBall::EndContact(b2Contact* contact) {
    
}

void SharedBall::addScore(int team, int amount) {
	_score[team]+=amount;
	if (_score[SB_TEAM_BOT] >= SB_POINTS_TO_WIN) {
		if (numberOfPlayers > 2)
		{
			int winners[] = SB_TEAM_BOT_PLAYERS;
			endGame(winners, 2);
		}
		else {
			endGame(SHARED_PLAYER2);
		}
	}else if (_score[SB_TEAM_TOP] >= SB_POINTS_TO_WIN) {
		if (numberOfPlayers > 3)
		{
			int winners[] = SB_TEAM_TOP_PLAYERS;
			endGame(winners, 2);
		}
		else {
			endGame(SHARED_PLAYER1);
		}
	}
}

void SharedBall::updatePhysics(float dt) {
	int positionIterations = 10;
	int velocityIterations = 10;

	deltaTime = dt;
	world->Step(dt, velocityIterations, positionIterations);

	for (b2Body *body = world->GetBodyList(); body != NULL; body = body->GetNext()) {
		if (body->GetUserData())
		{
			DrawNode *sprite = (DrawNode *)body->GetUserData();
			sprite->setPosition(Vec2(body->GetPosition().x * SB_SCALE_RATIO, body->GetPosition().y * SB_SCALE_RATIO));
			sprite->setRotation(-1 * CC_RADIANS_TO_DEGREES(body->GetAngle()));
		}
	}
	world->ClearForces();
#ifdef DEBUG_MODE
	world->DrawDebugData();
#endif //DEBUG_MODE
}