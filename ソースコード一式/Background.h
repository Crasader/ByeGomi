#pragma once
#include "cocos2d.h"
#include "Box2D\Box2D.h"
class Background :public cocos2d::Node
{
private:
	b2World* m_world;
	b2Body* m_body;
	bool init(b2World* world);
	b2PolygonShape getShape(float x, float y, float w, float h);
	void addBody(b2Shape &shape);
public:	
	static Background* create(b2World* world);
};