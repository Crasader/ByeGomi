#pragma once
#include "cocos2d.h"
#include "Mission.h"
class Stage : public cocos2d::Node
{
private:
	int m_stage;
	Mission m_mission;	
	bool init(Mission mission, int stageNum);
public:	
	static Stage* create(Mission mission, int stageNum);
	void popupStartDialog();	
};