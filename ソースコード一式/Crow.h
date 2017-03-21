#pragma once
#include "cocos2d.h"
#include "Box2D\Box2D.h"
class Crow :public cocos2d::Node
{
private:
	b2World* m_world;
	b2Body* m_body;
	b2Body* m_gomi;
	float m_radian;
	b2Vec2 m_center;
	bool init(b2World* world);
	void update(float delta);
	bool createCrow();
	void setMove();
	void setEnd();
	void setSE();
	void createBody();
	void setEvent();
	void setRaw();
	void setCloset();
	void setSubGomi(int type);
	cocos2d::Sprite* createSprite();
public:
	bool BeginContact(b2Contact* contact);
	static Crow* create(b2World* world);
};