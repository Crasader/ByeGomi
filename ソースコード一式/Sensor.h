#pragma once
#include "cocos2d.h"
#include "Box2D\Box2D.h"
class Sensor :public cocos2d::Node
{
private:
	b2Body* m_body;	
	bool init(b2World* world);
	void update(float delta)override;
	void setAnimation();
public:
	bool BeginContact(b2Contact* contact);	
	static Sensor* create(b2World* world);
};