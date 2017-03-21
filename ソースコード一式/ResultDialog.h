#pragma once
#include "cocos2d.h"
class ResultDialog : public cocos2d::Node
{
private:
	bool init(bool result);
public:
	static ResultDialog* create(bool result);
};