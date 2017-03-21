#include "Gomi.h"
#define PTM_RATIO 32.0f
using namespace cocos2d;
// 概要：初期化
bool Gomi::init(b2World * world)
{
	if (!Node::init())
		return false;
	// Worldを覚える
	m_world = world;
	return true;
}
// 概要：実体生成
// 引数：種類、大きさ
void Gomi::setEntity(int type, float scale)
{
	// それぞれの画像データ
	const std::string string[7] =
	{
		"gomi/normal.png","gomi/bottle.png",
		"gomi/raw.png",	"gomi/closet.png",
		"gomi/paper.png","gomi/glass.png",
		"gomi/crow.png",
	};	
	// Spriteの作成
	Sprite* spr = Sprite::create(string[type]);	
	spr->setScale(scale);
	spr->setTag(type);
	this->addChild(spr);
	// Bodyの作成
	createBody(spr);
}
// 概要：Bodyの生成
void Gomi::createBody(Sprite * spr)
{
	b2BodyDef bodyDef;	
	bodyDef.type = b2_dynamicBody;
	bodyDef.userData = spr;
	bodyDef.gravityScale = 0.5f;
	m_body = m_world->CreateBody(&bodyDef);
	b2FixtureDef fixtureDef;
	fixtureDef.density = 1.0f;
	fixtureDef.restitution = 0.2f;
	Vec2 size = spr->getContentSize() / 2 * spr->getScale();	
	if (spr->getTag() != CLOSET&&
		spr->getTag() != CROW)
	{
		b2CircleShape shape;
		spr->setAnchorPoint(Vec2(0.5f, 0.4f));
		shape.m_radius = size.x*0.85f / PTM_RATIO;
		fixtureDef.shape = &shape;
		m_body->CreateFixture(&fixtureDef);
	}
	else
	{
		b2PolygonShape shape;
		shape.SetAsBox(size.x / PTM_RATIO, size.y / PTM_RATIO);
		fixtureDef.shape = &shape;
		m_body->CreateFixture(&fixtureDef);
	}
}
// 概要：Gomi生成
Gomi * Gomi::create(b2World * world)
{
	Gomi* pRet = new(std::nothrow)Gomi();
	if (pRet && pRet->init(world))
	{
		pRet->autorelease();
		return pRet;
	}
	else 
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}
