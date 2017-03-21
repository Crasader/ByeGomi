#pragma once
#include "cocos2d.h"
#include "Box2D\Box2D.h"
class GomiSystem :public cocos2d::Layer
{
private:
	b2Body* m_bodyA;
	b2Body* m_bodyB;
	b2World* m_world;
	b2MouseJoint* m_mousejoint;
	bool init(b2World* world);
	void update(float delta)override;
	void createGomi();
	void fusionGomi();
	void destroyGomi(b2Body* &body);
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)override;
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)override;
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)override;
public:	
	bool BeginContact(b2Contact* contact);
	static GomiSystem* create(b2World* world);
};