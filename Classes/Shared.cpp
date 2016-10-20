#include "Shared.h"

USING_NS_CC;

Shared* Shared::minstance = NULL;

Shared * Shared::instance() {
	if (!minstance) {
		minstance = new Shared();
		minstance->colors = SHARED_COLOR_PLAYERS;
		minstance->debug = false;
		
		auto screenSize = Director::getInstance()->getWinSize();

		minstance->buttonPositions = {
			Vec2(0,screenSize.height),  //0
			Vec2(screenSize.width,0),  //1
			Vec2(0,0), //2
			Vec2(screenSize.width, screenSize.height) }; //3
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