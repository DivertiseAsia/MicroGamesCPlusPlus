//
//  GameButton.cpp
//  microgames
//
//  Created by Work on 19/9/16.
//
//

#include "GameButton.hpp"


GameButton::GameButton(float radius, Color4F color, int player, std::string format)
{
    _radius = radius;
    _color = color;
	_player = player;
	_format = format;
}

bool GameButton::init(){
    
    if (!ui::Button::init())
    {
        return false;
    }
    
	updateButton();
    
    this->setName("GameButton");

    return true;
}

void GameButton::changeColor(Color4F color){
    _color = color;
	updateButton();
}

void GameButton::changeFormat(std::string format){
	_format = format;
	updateButton();
}

void GameButton::updateButton()
{
	this->loadTextureNormal("button/Button_" + _format + std::to_string(_player + 1) + ".png");
	if(_format.compare("Hockey") != 0)
		this->loadTexturePressed("button/Button_" + _format + std::to_string(_player + 1) + "Pressed.png");
}

void GameButton::setRadius(float r){
    _radius = r;
}

void GameButton::setPlayer(int player)
{
	_player = player;
}

int GameButton::getPlayer()
{
	return _player;
}

SpriteFrame* GameButton::createSpriteFrame() const{
    //Create renderTexture
    auto renderTexture = RenderTexture::create(_radius*2, _radius*2, cocos2d::Texture2D::PixelFormat::RGBA8888);
    //Draw texture to Sprite
    renderTexture->beginWithClear(0, 0, 0, 0); // black
    auto drawPrimitive = DrawNode::create();
    drawPrimitive->retain();
    drawPrimitive->drawSolidCircle(Vec2(_radius,_radius), _radius, 3.1415968f, 360, _color);
#ifdef DEBUG
    drawPrimitive->drawRect( Vec2::ZERO+Vec2(1,1), Vec2(_radius*2-1,_radius*2-1), Color4F(1,0,1,0.5f)); //For debugging propose
#endif
    drawPrimitive->visit();
    renderTexture->end();
    renderTexture->retain();
    
    return renderTexture->getSprite()->getSpriteFrame();
}


