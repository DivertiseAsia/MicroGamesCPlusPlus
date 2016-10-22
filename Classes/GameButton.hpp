//
//  GameButton.hpp
//  microgames
//
//  Created by Work on 19/9/16.
//
//

#ifndef GameButton_hpp
#define GameButton_hpp

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Ball.hpp"

#define DEFAULT_BUTTON_RADIUS 100
#define DEFAULT_BUTTON_COLOR Color4F(1,0.5f,0.5f,0.8f)

USING_NS_CC;

class GameButton : public ui::Button{
public:
    GameButton(float radius=DEFAULT_BUTTON_RADIUS, Color4F color=DEFAULT_BUTTON_COLOR);
    
    CREATE_FUNC(GameButton);

    virtual bool init();
    
    void setRadius(float);
    void changeColor(Color4F color);
    CC_SYNTHESIZE(Ball*, _ball, Ball);
    CC_SYNTHESIZE(Touch*, _touch, Touch); //a property to hold touch

	void setPlayer(int player);
	int getPlayer();
private:
    float _radius;
    Color4F _color;
	int _player;
    SpriteFrame* createSpriteFrame() const;
};


#endif /* GameButton_hpp */
