#pragma once
#include "cocos2d.h"
#include "Mission.h"
class SelectScene :public cocos2d::Layer
{
private:
	Mission m_missionTable[9] =
	{
		{ Burnable,10 },{ Paper,10 },{ Carcass,5 },
		{ Bin,5 },{ Debris,20 },{ Raw,10 },
		{ Burnable,30 },{ Closet,4 },{ Raw,30 },
	};
	cocos2d::Sprite *m_gomi;
	bool init()override;
	void update(float delta)override;
	CREATE_FUNC(SelectScene);
public:	
	static cocos2d::Scene* createScene();
};