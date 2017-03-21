#pragma once
#include "cocos2d.h"
#include "Box2D\Box2D.h"
class Gomi:public cocos2d::Node
{
private:
	enum { NORMAL, BOTTLE, RAW, CLOSET , PAPER, GLASS, CROW};
	b2Body* m_body;
	b2World* m_world;
	bool init(b2World* world);
	void createBody(cocos2d::Sprite* spr);
public:
	// 概要：位置と角度の設定
	void setTransform(b2Vec2 pos, float angle) { m_body->SetTransform(pos, angle); }
	b2Body* getBody()const { return m_body; }
	// 概要：実体生成
	void setEntity(int type, float scale = 1.0f);	
	static Gomi* create(b2World* world);
};