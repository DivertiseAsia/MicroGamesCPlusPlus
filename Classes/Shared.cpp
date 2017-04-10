#include "Shared.h"

USING_NS_CC;

Shared* Shared::minstance = NULL;

Shared * Shared::instance() {
	if (!minstance) {
		minstance = new Shared();
		minstance->colors = SHARED_COLOR_PLAYERS;
		minstance->debug = false;
		
		auto screenSize = Director::getInstance()->getVisibleSize();
		auto screenOrigin = Director::getInstance()->getVisibleOrigin();

		minstance->buttonPositions = {
			Vec2(screenOrigin.x,screenOrigin.y+screenSize.height),  //0
			Vec2(screenOrigin.x+screenSize.width,screenOrigin.y),  //1
			screenOrigin, //2
			(screenOrigin+screenSize) }; //3

		minstance->buttonAnchor = {
			Vec2(0, 1),  //0
			Vec2(1, 0),  //1
			screenOrigin, //2
			Vec2(1, 1) }; //3

	}
	return minstance;
}

Vec2 Shared::getPlayerPosition(int player) {
	if (player >= 0 && player < SHARED_MAX_PLAYERS) {
		return buttonPositions[player];
	}

	return Vec2::ZERO;
}

Vec2 Shared::getPlayerAnchor(int player) {
	if (player >= 0 && player < SHARED_MAX_PLAYERS) {
		return buttonAnchor[player];
	}

	return Vec2::ZERO;
}

Color4F Shared::getPlayerColor(int player) {
	if (player >= 0 && player < SHARED_MAX_PLAYERS) {
		return colors[player];
	}
	
	return Color4F::WHITE;
}

std::string Shared::intToString(int number) {
	std::stringstream out;
	out << number;
	return out.str();
}

void Shared::setBackground(std::string fname, Layer* layer)
{
	auto winSize = layer->getContentSize();
	Sprite* bg = Sprite::create(fname);
	auto scaleX = winSize.width / bg->getContentSize().width;
	auto scaleY = winSize.height / bg->getContentSize().height;
	bg->setScaleX(scaleX);
	bg->setScaleY(scaleY);
	bg->setAnchorPoint(Vec2(0, 0));
	bg->setPosition(Vec2(0, 0));
	layer->addChild(bg, 0);
}