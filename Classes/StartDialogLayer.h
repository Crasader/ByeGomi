#pragma once

#include "cocos2d.h"
#include "Mission.h"

class StartDialog : public cocos2d::Node
{
private:
	int m_stage;
	Mission m_mission;
	bool init(Mission mission, int stage);
public:
	static StartDialog* create(Mission mission, int stage);
};