#pragma once
#include "cocos2d.h"
#include "CountDown.h"
class PlayScene :public cocos2d::Layer
{
private:	
	CountDown* m_countDown;
	bool init()override;
	void update(float delta)override;
	CREATE_FUNC(PlayScene);
public:
	static cocos2d::Scene* createScene();
};