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
	}
	return minstance;
}

Vec2 Shared::getPlayerPosition(int player) {
	if (player >= 0 && player < SHARED_MAX_PLAYERS) {
		return buttonPositions[player];
	}

	return Vec2::ZERO;
}

Color4F Shared::getPlayerColor(int player) {
	if (player >= 0 && player < SHARED_MAX_PLAYERS) {
		return colors[player];
	}
	
	return Color4F::WHITE;
}

bool Shared::isDebugMode() {
	return debug;
}

void Shared::setDebugMode(bool val) {
	debug = val;
}